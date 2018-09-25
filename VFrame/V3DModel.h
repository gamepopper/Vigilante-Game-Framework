/**
* @file    V3DModel.h
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
* The basic 3D model class.
*/

#pragma once

#ifndef VFRAME_NO_3D
#include "V3DObject.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <vector>
#include <memory>

#include "depend/glm/vec2.hpp"
#include "depend/glm/vec3.hpp"
#include "depend/glm/vec4.hpp"
#include "depend/glm/matrix.hpp"

///Vertex attributes for shaders and the input vertex array.
enum class V3DVertexAttribute { Position, Normal, Color, TexCoord, COUNT };

///The 3D vertex structure for V3DModel.
struct V3DVertex
{
	V3DVertex() {}

	/**
	* @param pos Vertex Position.
	*/
	V3DVertex(const glm::vec3& pos) : position(pos), color(1,1,1,1) {}

	/**
	* @param pos Vertex Position.
	* @param norm Vertex Normal.
	*/
	V3DVertex(const glm::vec3& pos, const glm::vec3& norm) : position(pos), normal(norm), color(1, 1, 1, 1) {}

	/**
	* @param pos Vertex Position.
	* @param tex Vertex Texture Coordinate.
	*/
	V3DVertex(const glm::vec3& pos, const glm::vec2& tex) : position(pos), color(1, 1, 1, 1), texCoord(tex) {}

	/**
	* @param pos Vertex Position.
	* @param tint Vertex Colour.
	*/
	V3DVertex(const glm::vec3& pos, const glm::vec4& tint) : position(pos), color(tint) {}

	/**
	* @param pos Vertex Position.
	* @param norm Vertex Normal.
	* @param tex Vertex Texture Coordinate.
	*/
	V3DVertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex) : position(pos), normal(norm), color(1, 1, 1, 1), texCoord(tex) {}

	/**
	* @param pos Vertex Position.
	* @param norm Vertex Normal.
	* @param tint Vertex Colour.
	*/
	V3DVertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec4& tint) : position(pos), normal(norm), color(tint) {}

	/**
	* @param pos Vertex Position.
	* @param tint Vertex Colour.
	* @param tex Vertex Texture Coordinate.
	*/
	V3DVertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec4& tint) : position(pos), texCoord(tex), color(tint) {}

	/**
	* @param pos Vertex Position.
	* @param norm Vertex Normal.
	* @param tint Vertex Colour.
	* @param tex Vertex Texture Coordinate.
	*/
	V3DVertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec4& tint, const glm::vec2& tex) : position(pos), normal(norm), color(tint), texCoord(tex) {}

	///Position Vertex
	glm::vec3 position;
	///Normal Vertex (for lighting)
	glm::vec3 normal;
	///Color Vertex (for colour tint)
	glm::vec4 color;
	///Texture Coordinate Vertex (for Textures).
	glm::vec2 texCoord;
};

///A 3D Vertex Array is a list of 3D vertices.
typedef std::vector<V3DVertex> V3DVertexArray;

struct V3DMaterial;
///Basic 3D Model with a single texture.
class V3DModel : public V3DObject
{
protected:
	virtual void updateTransform();
	///Vertex Array Object ID.
	unsigned int vao;
	///Vertex Buffer Object for Vertices.
	unsigned int vertexVBO;
	///Vertex Buffer Object for Indices.
	unsigned int indexVBO;
	///Depending on input, the amount of vertices or indices that are needed to be drawn for this object.
	unsigned int drawCount;
	///Single texture for this model.
	sf::Texture texture;
	///Model Matrix
	glm::mat4 transform;

public:
	///Used to call parent class functions when they are overrided in class.
	typedef V3DObject VSUPERCLASS;
	
	///Surface Material for Lighting.
	V3DMaterial* Material;

	/**
	* @param position Position of the sprite.
	* @param rotation Angle of the object.
	* @param scale Scale of the object.
	*/
	V3DModel(sf::Vector3f position, sf::Vector3f rotation, sf::Vector3f scale);

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
	V3DModel(float posX = 0, float posY = 0, float posZ = 0,
		float rotX = 0, float rotY = 0, float rotZ = 0,
		float scaleX = 1, float scaleY = 1, float scaleZ = 1);

	/**
	* Loads the model from arrays.
	* @param vertexArray The list of vertices to load into the model.
	* @param indexArray The list of indices to load for the model. If model only has vertex data then this is ignored for loading and rendering the model.
	*/
	bool LoadModelData(const V3DVertexArray& vertexArray, const std::vector<unsigned int>& indexArray = std::vector<unsigned int>());

	/**
	* Loads a texture for the 3D model. If no texture is loaded than the default texture is used.
	* @param filename The file path and name of the texture file.
	*/
	bool LoadTexture(const sf::String& filename);

	/**
	* Loads a texture for the 3D model. If no texture is loaded than the default texture is used.
	* @param tex The sf::Texture object that is being used as the texture.
	*/
	bool LoadTexture(const sf::Texture& tex);

	virtual void UpdateShader(V3DShader* shader, V3DCamera* camera);

	virtual void Destroy();
	virtual void Draw(sf::RenderTarget& RenderTarget) override;

	///A generated 1x1 White texture. If no texture is loaded, then this will be used.
	static unsigned int DefaultTexture;
	///Function to generate default texture.
	static void GenerateDefaultTexture();
};
#endif