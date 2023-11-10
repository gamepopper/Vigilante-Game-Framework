#ifndef VFRAME_NO_3D

#ifndef NO_GLEW
#include <GL/glew.h>
#endif

#include "V3DObjModel.h"
#include "V3DShader.h"
#include "V3DLight.h"
#include "V3DCamera.h"
#include "V3DModel.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "depend/glm/glm.hpp" 
#include "depend/glm/gtx/transform.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "depend/tiny_obj_loader.h"

V3DObjModel::V3DObjModel(sf::Vector3f position, sf::Vector3f rotation, sf::Vector3f scale) :
	V3DObject(position, rotation, scale)
{
	material = new V3DMaterial();
}

V3DObjModel::V3DObjModel(float posX, float posY, float posZ,
	float rotX, float rotY, float rotZ,
	float scaleX, float scaleY, float scaleZ) :
	V3DObject(posX, posY, posZ, rotX, rotY, rotZ, scaleX, scaleY, scaleZ)
{
	material = new V3DMaterial();
}

void V3DObjModel::updateTransform()
{
	//S * R * T
	glm::mat4 matrix_pos = glm::translate(glm::vec3(Position.x, Position.y, Position.z));
	glm::mat4 matrix_scale = glm::scale(glm::vec3(Scale.x, Scale.y, Scale.z));
	// Represent each stored rotation as a different matrix, because 
	// we store angles. 
	// x  y  z 
	glm::mat4 matrix_rotX = glm::rotate(Rotation.x * (VFRAME_PI / 180.0f), glm::vec3(1, 0, 0));
	glm::mat4 matrix_rotY = glm::rotate(Rotation.y * (VFRAME_PI / 180.0f), glm::vec3(0, 1, 0));
	glm::mat4 matrix_rotZ = glm::rotate(Rotation.z * (VFRAME_PI / 180.0f), glm::vec3(0, 0, 1));
	// Create a rotation matrix. 
	// Multiply in reverse order it needs to be applied. 
	glm::mat4 matrix_rotation = matrix_rotZ * matrix_rotY * matrix_rotX;
	// Apply transforms in reverse order they need to be applied in. 
	transform = matrix_pos * matrix_rotation * matrix_scale;
}

bool V3DObjModel::LoadModelData(const char* filename)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::string err;

	modelData.clear();
	materials.clear();
	textures.clear();

	minimum.x = FLT_MAX;
	minimum.y = FLT_MAX;
	minimum.z = FLT_MAX;
	maximum.x = -FLT_MAX;
	maximum.y = -FLT_MAX;
	maximum.z = -FLT_MAX;

	std::string path = GetBaseDir(filename);
	path += "/";

	bool r = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, path.c_str());

	if (r)
	{
		materials.push_back(tinyobj::material_t());
		tinyobj::InitMaterial(&materials.back());

		for (unsigned int i = 0; i < materials.size(); ++i)
		{
			tinyobj::material_t* mp = &materials[i];

			if (mp->diffuse_texname != "")
			{
				if (textures.find(mp->diffuse_texname) == textures.end())
				{
					sf::Texture tex;
					if (!tex.loadFromFile(path + mp->diffuse_texname))
					{
						VLog("Unable to find texture %s", mp->diffuse_texname.c_str());
						return false;
					}

					textures.insert(std::make_pair(mp->diffuse_texname, tex));
				}
			}
		}

		for (unsigned int s = 0; s < shapes.size(); s++)
		{
			V3DModelData md;
			std::vector<float> vb; // pos(3float), normal(3float), color(4float)
			for (unsigned int f = 0; f < shapes[s].mesh.indices.size() / 3; f++)
			{
				tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
				tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
				tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

				int currentMatID = shapes[s].mesh.material_ids[f];
				
				if ((currentMatID < 0) || (currentMatID >= static_cast<int>(materials.size()))) 
				{
					// Invaid material ID. Use default material.
					currentMatID = materials.size() - 1; // Default material is added to the last item in `materials`.
				}

				float diffuse[3];
				for (unsigned int i = 0; i < 3; ++i) 
				{
					diffuse[i] = materials[currentMatID].diffuse[i];
				}

				float tc[3][2];
				if (attrib.texcoords.size() > 0) 
				{
					tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
					tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
					tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
					tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
					tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
					tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
				}
				else 
				{
					tc[0][0] = 0.0f;
					tc[0][1] = 0.0f;
					tc[1][0] = 0.0f;
					tc[1][1] = 0.0f;
					tc[2][0] = 0.0f;
					tc[2][1] = 0.0f;
				}

				float v[3][3];
				for (int k = 0; k < 3; k++)
				{
					int f0 = idx0.vertex_index;
					int f1 = idx1.vertex_index;
					int f2 = idx2.vertex_index;

					v[0][k] = attrib.vertices[3 * f0 + k];
					v[1][k] = attrib.vertices[3 * f1 + k];
					v[2][k] = attrib.vertices[3 * f2 + k];
				}

				float n[3][3];
				if (attrib.normals.size() > 0) 
				{
					int f0 = idx0.normal_index;
					int f1 = idx1.normal_index;
					int f2 = idx2.normal_index;
					for (int k = 0; k < 3; k++) 
					{
						n[0][k] = attrib.normals[3 * f0 + k];
						n[1][k] = attrib.normals[3 * f1 + k];
						n[2][k] = attrib.normals[3 * f2 + k];
					}
				}
				else 
				{
					// compute geometric normal
					CalcNormal(n[0], v[0], v[1], v[2]);
					n[1][0] = n[0][0];
					n[1][1] = n[0][1];
					n[1][2] = n[0][2];
					n[2][0] = n[0][0];
					n[2][1] = n[0][1];
					n[2][2] = n[0][2];
				}

				for (int k = 0; k < 3; k++) 
				{
					vb.push_back(v[k][0]);
					vb.push_back(v[k][1]);
					vb.push_back(v[k][2]);
					vb.push_back(-n[k][0]);
					vb.push_back(n[k][1]);
					vb.push_back(-n[k][2]);
					// Combine normal and diffuse to get color.
					float c[3] = {
						diffuse[0],
						diffuse[1],
						diffuse[2]
					};
					/*float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
					if (len2 > 0.0f) {
						float len = sqrtf(len2);

						c[0] /= len;
						c[1] /= len;
						c[2] /= len;
					}*/
					vb.push_back(c[0]);
					vb.push_back(c[1]);
					vb.push_back(c[2]);
					vb.push_back(1.0f);

					vb.push_back(tc[k][0]);
					vb.push_back(tc[k][1]);
				}
			}
			
			md.triangleCount = 0;

			if (vb.size() > 0)
			{
				md.triangleCount = vb.size() / (3 + 3 + 4 + 2) / 3; // 3:vtx, 3:normal, 3:col, 2:texcoord
				glCheck(glGenVertexArrays(1, &md.vao));
				glCheck(glBindVertexArray(md.vao));
				glCheck(glGenBuffers(1, &md.vb));

				auto stride = (3 + 3 + 4 + 2) * sizeof(float);
				auto normalOffset = 3 * sizeof(float);
				auto colorOffset = normalOffset + (3 * sizeof(float));
				auto texCoordOffset = colorOffset + (4 * sizeof(float));

				glCheck(glBindBuffer(GL_ARRAY_BUFFER, md.vb));
				glCheck(glBufferData(GL_ARRAY_BUFFER, md.triangleCount * 3 * stride, vb.data(), GL_STATIC_DRAW));
				glCheck(glEnableVertexAttribArray(static_cast<GLuint>(V3DVertexAttribute::Position)));
				glCheck(glVertexAttribPointer(static_cast<GLuint>(V3DVertexAttribute::Position), 3, GL_FLOAT, GL_FALSE, stride, 0));
				glCheck(glEnableVertexAttribArray(static_cast<GLuint>(V3DVertexAttribute::Normal)));
				glCheck(glVertexAttribPointer(static_cast<GLuint>(V3DVertexAttribute::Normal), 3, GL_FLOAT, GL_TRUE, stride, (void*)normalOffset));
				glCheck(glEnableVertexAttribArray(static_cast<GLuint>(V3DVertexAttribute::Color)));
				glCheck(glVertexAttribPointer(static_cast<GLuint>(V3DVertexAttribute::Color), 4, GL_FLOAT, GL_FALSE, stride, (void*)colorOffset));
				glCheck(glEnableVertexAttribArray(static_cast<GLuint>(V3DVertexAttribute::TexCoord)));
				glCheck(glVertexAttribPointer(static_cast<GLuint>(V3DVertexAttribute::TexCoord), 2, GL_FLOAT, GL_FALSE, stride, (void*)texCoordOffset));
				glCheck(glBindVertexArray(0));
			}

			// OpenGL viewer does not support texturing with per-face material.
			if (shapes[s].mesh.material_ids.size() > 0) 
			{
				// Base case
				md.materialID = shapes[s].mesh.material_ids[shapes[s].mesh.material_ids.size() - 1];
			}
			else 
			{
				md.materialID = materials.size() - 1; // = ID for default material.
			}

			modelData.push_back(md);

			for (unsigned int v = 0; v < vb.size(); v += 10)
			{
				minimum.x = std::fminf(vb[v + 0], minimum.x);
				maximum.x = std::fmaxf(vb[v + 0], maximum.x);
				minimum.y = std::fminf(vb[v + 1], minimum.y);
				maximum.y = std::fmaxf(vb[v + 1], maximum.y);
				minimum.z = std::fminf(vb[v + 2], minimum.z);
				maximum.z = std::fmaxf(vb[v + 2], maximum.z);
			}
		}
	}
	else
	{
		VLog("%s failed to load", filename);
	}

	sf::Vector3f Size;
	Size.x = maximum.x - minimum.x;
	Size.y = maximum.y - minimum.y;
	Size.z = minimum.z - maximum.z;

	Origin.x = minimum.x + (Size.x / 2.0f);
	Origin.y = minimum.y + (Size.y / 2.0f);
	Origin.z = maximum.z + (Size.z / 2.0f);

	Radius = Size.x;
	Radius = std::fminf(Radius, Size.y);
	Radius = std::fminf(Radius, Size.z);

	Radius /= 2.0f;

	return r;
}

std::string V3DObjModel::GetBaseDir(const std::string &filepath) 
{
	if (filepath.find_last_of("/\\") != std::string::npos)
		return filepath.substr(0, filepath.find_last_of("/\\"));
	return "";
}

void V3DObjModel::CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) 
{
	float v10[3];
	v10[0] = v1[0] - v0[0];
	v10[1] = v1[1] - v0[1];
	v10[2] = v1[2] - v0[2];

	float v20[3];
	v20[0] = v2[0] - v0[0];
	v20[1] = v2[1] - v0[1];
	v20[2] = v2[2] - v0[2];

	N[0] = v20[1] * v10[2] - v20[2] * v10[1];
	N[1] = v20[2] * v10[0] - v20[0] * v10[2];
	N[2] = v20[0] * v10[1] - v20[1] * v10[0];

	float len2 = (N[0] * N[0]) + (N[1] * N[1]) + (N[2] * N[2]);
	if (len2 > 0.0f) 
	{
		float len = sqrtf(len2);

		N[0] /= len;
		N[1] /= len;
		N[2] /= len;
	}
}

void V3DObjModel::UpdateShader(V3DShader* shader, V3DCamera* camera)
{
	if (textures.size() == 0)
		V3DModel::GenerateDefaultTexture();
	
	if (camera)
	{
		glm::mat4 viewProj = camera->PVMatrix() * transform;
		shader->UpdateUniform(UniformType::TransformPVM, &viewProj[0][0]);
		shader->UpdateUniform(UniformType::TransformVM, &camera->ViewMatrix()[0][0]);
		shader->UpdateUniform(UniformType::TransformM, &transform[0][0]);
	}
	else
	{
		glm::mat4 identity;
		shader->UpdateUniform(UniformType::TransformPVM, &transform[0][0]);
		shader->UpdateUniform(UniformType::TransformVM, &identity[0][0]);
		shader->UpdateUniform(UniformType::TransformM, &transform[0][0]);
	}

	this->shader = shader;
}

void V3DObjModel::Destroy()
{
	VSUPERCLASS::Destroy();

	for (unsigned int i = 0; i < modelData.size(); ++i)
	{
		glCheck(glDeleteBuffers(1, &modelData[i].vb));
		glCheck(glDeleteVertexArrays(1, &modelData[i].vao));
	}

	modelData.clear();
	modelData.shrink_to_fit();
	materials.clear();
	materials.shrink_to_fit();

	delete material;
}

void V3DObjModel::Draw(sf::RenderTarget& RenderTarget)
{
	VSUPERCLASS::Draw(RenderTarget);

	for (unsigned int i = 0; i < modelData.size(); ++i)
	{
		V3DModelData &md = modelData[i];

		if ((md.materialID < materials.size()))
		{
			tinyobj::material_t &mat = materials[md.materialID];

			//Set texture
			std::string diffuse_texname = mat.diffuse_texname;
			if (textures.find(diffuse_texname) == textures.end())
				glCheck(glBindTexture(GL_TEXTURE_2D, V3DModel::DefaultTexture));
			else
				sf::Texture::bind(&textures[diffuse_texname]);

			material->Specular.x = mat.specular[0];
			material->Specular.y = mat.specular[1];
			material->Specular.z = mat.specular[2];
			material->Shininess = mat.shininess;

			shader->UpdateUniform(UniformType::Material, material);
		}

		// Draw the model
		glCheck(glBindVertexArray(md.vao));
		glCheck(glDrawArrays(GL_TRIANGLES, 0, 3 * md.triangleCount));

		glCheck(glBindVertexArray(0));
		glCheck(glBindTexture(GL_TEXTURE_2D, 0));
	}
}
#endif