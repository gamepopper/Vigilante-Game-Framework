#pragma once
#include "V3DObject.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <memory>

struct V3DMaterial
{
	GLfloat Colour[4];
	GLfloat Specular[4];
	GLfloat Shininess;
};

class V3DModel : public V3DObject
{
protected:
	virtual void updateTransform();

	sf::Texture texture;
	std::unique_ptr<V3DMaterial> material;

	std::vector<GLfloat> modelData;
	std::vector<unsigned int> modelIndices;
	int vertexArrayOffset = 0;
	int normalArrayOffset = -1;
	int textureArrayOffset = -1;
	int colourArrayOffset = -1;
	int dataLineLength = 3;

public:
	typedef V3DObject VSUPERCLASS;

	V3DModel(sf::Vector3f position, sf::Vector3f rotation, sf::Vector3f scale) :
		V3DObject(position, rotation, scale)
	{
        #if WIN32
		material = std::make_unique<V3DMaterial>();
		#else
		material = std::unique_ptr<V3DMaterial>(new V3DMaterial());
		#endif
	}

	V3DModel(float posX = 0, float posY = 0, float posZ = 0,
		float rotX = 0, float rotY = 0, float rotZ = 0,
		float scaleX = 1, float scaleY = 1, float scaleZ = 1) :
		V3DObject(posX, posY, posZ, rotX, rotY, rotZ, scaleX, scaleY, scaleZ)
	{
		#if WIN32
		material = std::make_unique<V3DMaterial>();
		#else
		material = std::unique_ptr<V3DMaterial>(new V3DMaterial());
		#endif
	}

	/*
	Loads model from single array.
	vertexPos: Point where Vertex data offsets.
	normalPos: Point where Normal data offsets.
	texturePos: Point where Texture UV data offsets.
	colourPos: Point where colour data offset.
	Setting to -1 ignores parameter.
	*/
	bool LoadModelData(const std::vector<GLfloat>& data, int vertexPos, int normalPos = -1, int texturePos = -1, int colourPos = -1);
	/*
	Loads model indices, used if model array requires index rendering.
	*/
	bool LoadModelIndices(const std::vector<unsigned int>& data);
	/*Loads texture from filepath string*/
	bool LoadTexture(const sf::String& filename, bool mipmap = false);
	/*Loads texture from sf::Texture object*/
	bool LoadTexture(const sf::Texture& texture, bool mipmap = false);
	
	/*Sets material data of model*/
	void SetMaterial(sf::Color Colour, sf::Color Specular, float Shininess);

	virtual void Destroy();
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

