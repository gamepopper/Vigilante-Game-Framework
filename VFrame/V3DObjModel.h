/**
* @file    V3DObjModel.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2018 Tim Stoddard
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* @section DESCRIPTION
*
* The basic 3D model class that loads in models using the OBJ model format (using tinyobjloader).
*/

#pragma once
#ifndef VFRAME_NO_3D
#include "V3DObject.h"
#include "depend/tiny_obj_loader.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <map>
#include <memory>

#include "depend/glm/matrix.hpp"

#include "V3DLight.h"
///3D Model Class for Loading and Rendering models in the .OBJ file format using TinyOBJLoader.
class V3DObjModel : public V3DObject
{
protected:
	///The structure of each individual shape.
	struct V3DModelData
	{
		///Vertex Buffer ID.
		unsigned int vb;
		///Vertex Array Object ID.
		unsigned int vao;
		///Number of Triangles in this model.
		int triangleCount;
		///Material ID of this mode.
		size_t materialID;
	};

	virtual void updateTransform();
	///Get the directory of the file.
	static std::string GetBaseDir(const std::string &filepath);
	///Calculate the normal from three vertices.
	static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]);

	///List of models to shape.
	std::vector<V3DModelData> modelData;
	///List of materials 
	std::vector<tinyobj::material_t> materials;
	///List of textures.
	std::map<sf::String, sf::Texture> textures;
	///Model Matrix.
	glm::mat4 transform;
	///Material Data for rendering.
	V3DMaterial* material;
	///Pointer to shader so I can update the material for each model.
	V3DShader* shader;

public:
	///Used to call parent class functions when they are overrided in class.
	typedef V3DObject VSUPERCLASS;

	/**
	* @param position Position of the sprite.
	* @param rotation Angle of the object.
	* @param scale Scale of the object.
	*/
	V3DObjModel(sf::Vector3f position, sf::Vector3f rotation, sf::Vector3f scale);

	/**
	* @param posX X position of the object.
	* @param posY Y position of the object.
	* @param posZ Z position of the object.
	* @param rotX X angle of the object.
	* @param rotY Y angle of the object.
	* @param rotZ Z angle of the object.
	* @param scaleX X scale of the object.
	* @param scaleY Y scale of the object.
	* @param scaleZ Z scale of the object.
	*/
	V3DObjModel(float posX = 0, float posY = 0, float posZ = 0,
		float rotX = 0, float rotY = 0, float rotZ = 0,
		float scaleX = 1, float scaleY = 1, float scaleZ = 1);

	/**
	Loads Object Model from filepath.
	Any material and texture files should be placed relative to the .obj file.
	*/
	bool LoadModelData(const char* filename);
	virtual void UpdateShader(V3DShader* shader, V3DCamera* camera);

	virtual void Destroy();
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};
#endif