#include "V3DObjModel.h"
#include "VGlobal.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "depend/tiny_obj_loader.h"

void V3DObjModel::updateTransform()
{
	//S * R * T
	glTranslatef(Position.x, Position.y, Position.z);
	glRotatef(Rotation.z, 0.0f, 0.0f, 1.0f);
	glRotatef(Rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(Rotation.y, 0.0f, 1.0f, 0.0f);
	glScalef(Scale.x, Scale.y, Scale.z);
}

bool V3DObjModel::LoadModelData(const char* filename)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::string err;

	modelData.clear();
	materials.clear();
	textures.clear();

	std::string path = GetBaseDir(filename);
#ifdef _WIN32
	path += "\\";
#else
	path += "/";
#endif

	bool r = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, path.c_str());

	if (r)
	{
		materials.push_back(tinyobj::material_t());
		tinyobj::InitMaterial(&materials.back());

		for (unsigned int i = 0; i < materials.size(); i++)
		{
			tinyobj::material_t* mp = &materials[i];

			if (mp->diffuse_texname.length() > 0)
			{
				if (textures.find(mp->diffuse_texname) == textures.end())
				{
					sf::Texture tex;
					if (!tex.loadFromFile(mp->diffuse_texname))
					{
						VLog("Unable to find texture %s", mp->diffuse_texname);
						return false;
					}

					textures.insert(std::make_pair(mp->diffuse_texname, tex));
				}
			}
		}

		for (unsigned int s = 0; s < shapes.size(); s++)
		{
			V3DModelData md;
			std::vector<float> &vb = md.vb; // pos(3float), normal(3float), color(3float)
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
				for (unsigned int i = 0; i < 3; i++) 
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
					vb.push_back(-v[k][1]);
					vb.push_back(v[k][2]);
					vb.push_back(n[k][0]);
					vb.push_back(-n[k][1]);
					vb.push_back(n[k][2]);
					// Combine normal and diffuse to get color.
					float c[3] = {
						diffuse[0],
						diffuse[1],
						diffuse[2]
					};
					float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
					if (len2 > 0.0f) {
						float len = sqrtf(len2);

						c[0] /= len;
						c[1] /= len;
						c[2] /= len;
					}
					vb.push_back(c[0] * 0.5f + 0.5f);
					vb.push_back(c[1] * 0.5f + 0.5f);
					vb.push_back(c[2] * 0.5f + 0.5f);

					vb.push_back(tc[k][0]);
					vb.push_back(tc[k][1]);
				}
			}
			
			md.triangleCount = 0;

			if (vb.size() > 0)
			{
				md.triangleCount = vb.size() / (3 + 3 + 3 + 2) / 3; // 3:vtx, 3:normal, 3:col, 2:texcoord
			}

			// OpenGL viewer does not support texturing with per-face material.
			if (shapes[s].mesh.material_ids.size() > 0 && shapes[s].mesh.material_ids.size() > s) 
			{
				// Base case
				md.materialID = shapes[s].mesh.material_ids[s];
			}
			else 
			{
				md.materialID = materials.size() - 1; // = ID for default material.
			}

			modelData.push_back(md);
		}
	}
	else
	{
		VLog("%s failed to load", filename);
	}

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

	float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
	if (len2 > 0.0f) {
		float len = sqrtf(len2);

		N[0] /= len;
		N[1] /= len;
	}
}

void V3DObjModel::Destroy()
{
	VSUPERCLASS::Destroy();
	modelData.clear();
	modelData.shrink_to_fit();
	materials.clear();
	materials.shrink_to_fit();

}

void V3DObjModel::Draw(sf::RenderTarget& RenderTarget)
{
	GLsizei stride = (3 + 3 + 3 + 2) * sizeof(float);

	// Enable position and texture coordinates vertex components
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (unsigned int i = 0; i < modelData.size(); i++)
	{
		V3DModelData &md = modelData[i];
		std::vector<float> &vb = md.vb;

		glVertexPointer(3, GL_FLOAT, stride, vb.data());
		glNormalPointer(GL_FLOAT, stride, vb.data() + 3);
		glColorPointer(3, GL_FLOAT, stride, vb.data() + 6);
		glTexCoordPointer(2, GL_FLOAT, stride, vb.data() + 9);

		if ((md.materialID < materials.size()))
		{
			tinyobj::material_t &mat = materials[md.materialID];

			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

			glColor4f(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], mat.dissolve);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.specular);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat.shininess);

			if (mat.dissolve < 1.0f)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			else
			{
				glDisable(GL_BLEND);
			}

			std::string diffuse_texname = mat.diffuse_texname;
			if (textures.find(diffuse_texname) != textures.end())
			{
				glEnable(GL_TEXTURE_2D);
				sf::Texture::bind(&textures[diffuse_texname]);
			}
		}

		// Draw the model
		glPushMatrix();
		VSUPERCLASS::Draw(RenderTarget);
		glDrawArrays(GL_TRIANGLES, 0, 3 * md.triangleCount);
		glPopMatrix();
		sf::Texture::bind(NULL);
	}
}
