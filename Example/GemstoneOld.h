#pragma once
#include "../VFrame/VObject.h"
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

class GemstoneOld : public VObject
{
protected:
	sf::VertexArray vertices;
	sf::Texture texture;

	static sf::Vector3f scaleVector3f(sf::Vector3f Point, sf::Vector3f Scale);
	static sf::Vector3f rotateYVector3f(sf::Vector3f Point, float angle, sf::Vector3f Origin);
	static sf::Vector3f reflectVector3f(sf::Vector3f incident, sf::Vector3f normal);
	static sf::Vector3f crossVector3f(sf::Vector3f a, sf::Vector3f b);
	static float dotVector3f(sf::Vector3f a, sf::Vector3f b);

	virtual void updateTransform() override;

public:
	typedef VObject VSUPERCLASS;
	sf::RenderStates RenderState = sf::RenderStates::Default;
	sf::Transformable transformable;
	sf::Color ambient = sf::Color::White;

	GemstoneOld(float x = 0.0f, float y = 0.0f) : VObject(x, y)
	{
#if _DEBUG
		DebugColor = sf::Color::Yellow;
#endif
		vertices.setPrimitiveType(sf::Triangles);
	}
	GemstoneOld(sf::Vector2f position) : VObject(position)
	{
#if _DEBUG
		DebugColor = sf::Color::Yellow;
#endif
		vertices.setPrimitiveType(sf::Triangles);
	}
	virtual ~GemstoneOld()
	{
		Destroy();
	}

	virtual void Destroy()
	{
		VSUPERCLASS::Destroy();
		vertices.resize(0);
	}

	void LoadTexture(sf::String filename);
	void LoadTexture(sf::Texture &texture);
	bool Generate(std::vector<sf::Vector2f> &coordinates, float symmetry, sf::Vector3f &scale, float slice = 0, float angleOffset = 0, unsigned int seed = 0, float randomVertVar = 0, float rotateY = 0);

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

