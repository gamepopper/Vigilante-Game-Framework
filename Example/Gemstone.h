#pragma once
#include "../VFrame/V3DModel.h"
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

class Gemstone : public V3DModel
{
protected:
	static sf::Vector3f rotateYVector3f(sf::Vector3f Point, float angle, sf::Vector3f Origin);
	static sf::Vector3f reflectVector3f(sf::Vector3f incident, sf::Vector3f normal);
	static sf::Vector3f crossVector3f(sf::Vector3f a, sf::Vector3f b);
	static float dotVector3f(sf::Vector3f a, sf::Vector3f b);

	virtual void updateTransform();
	sf::Vector3f scale;

public:
	typedef V3DModel VSUPERCLASS;

	Gemstone(float posX = 0, float posY = 0, float posZ = 0,
		float rotX = 0, float rotY = 0, float rotZ = 0,
		float scaleX = 1, float scaleY = 1, float scaleZ = 1) :
		V3DModel(posX, posY, posZ, rotX, rotY, rotZ, scaleX, scaleY, scaleZ)
	{

	}
	Gemstone(sf::Vector3f position, sf::Vector3f rotation, sf::Vector3f scale) :
		V3DModel(position, rotation, scale)
	{

	}
	virtual ~Gemstone()
	{
		Destroy();
	}

	virtual void Destroy()
	{
		VSUPERCLASS::Destroy();
	}

	bool Generate(std::vector<sf::Vector2f> &coordinates, float symmetry, sf::Vector3f &scale, float slice = 0, float angleOffset = 0, unsigned int seed = 0, float randomVertVar = 0, float rotateY = 0);
};

