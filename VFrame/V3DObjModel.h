#pragma once
#include "V3DObject.h"
#include "depend/tiny_obj_loader.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <map>

class V3DObjModel : public V3DObject
{
protected:
	struct V3DModelData
	{
		std::vector<float> vb;
		int triangleCount;
		size_t materialID;
	};

	virtual void updateTransform();
	static std::string GetBaseDir(const std::string &filepath);
	static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]);

	std::vector<V3DModelData> modelData;
	std::vector<tinyobj::material_t> materials;
	std::map<sf::String, sf::Texture> textures;

public:
	typedef V3DObject VSUPERCLASS;
	V3DObjModel(sf::Vector3f position, sf::Vector3f rotation, sf::Vector3f scale) :
		V3DObject(position, rotation, scale)
	{

	}

	V3DObjModel(float posX = 0, float posY = 0, float posZ = 0,
		float rotX = 0, float rotY = 0, float rotZ = 0,
		float scaleX = 1, float scaleY = 1, float scaleZ = 1) :
		V3DObject(posX, posY, posZ, rotX, rotY, rotZ, scaleX, scaleY, scaleZ)
	{

	}

	/*
	Loads Object Model from filepath.
	Any material and texture files should be placed relative to the .obj file.
	*/
	bool LoadModelData(const char* filename);

	virtual void Destroy();
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

