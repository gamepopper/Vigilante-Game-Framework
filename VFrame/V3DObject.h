#pragma once
#include "VBase.h"

#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class V3DShader;
class V3DCamera;
class V3DObject : public VBase
{
protected:
	void updateMotion(float dt);
	virtual void updateTransform() {};

public:
	typedef VBase VSUPERCLASS;

	sf::Vector3f Position;
	sf::Vector3f Rotation;
	sf::Vector3f Scale;
	sf::Vector3f Last;
	
	sf::Vector3f AngleVelocity;
	sf::Vector3f AngleAcceleration;
	sf::Vector3f AngleDrag;
	sf::Vector3f AngleMax;
	
	bool Moves = true;
	sf::Vector3f Velocity;
	sf::Vector3f Acceleration;
	sf::Vector3f Drag;
	sf::Vector3f MaxVelocity;

	float Health = 100;

	V3DObject(sf::Vector3f position, sf::Vector3f rotation, sf::Vector3f scale) : VBase()
	{
		Position = position;
		Rotation = rotation;
		Scale = scale;
		MaxVelocity = sf::Vector3f(10000, 10000, 10000);
		AngleMax = sf::Vector3f(10000, 10000, 10000);
		type = VType::OBJECT;
	}

	V3DObject(float posX = 0, float posY = 0, float posZ = 0, 
			float rotX = 0, float rotY = 0, float rotZ = 0, 
			float scaleX = 1, float scaleY = 1, float scaleZ = 1) : VBase()
	{
		Position = sf::Vector3f(posX, posY, posZ);
		Rotation = sf::Vector3f(rotX, rotY, rotZ);
		Scale = sf::Vector3f(scaleX, scaleY, scaleZ);
		MaxVelocity = sf::Vector3f(10000, 10000, 10000);
		AngleMax = sf::Vector3f(10000, 10000, 10000);
		type = VType::OBJECT;
	}

	/*
	Resets object
	newPos: Sets Position with sf::Vector3f.
	*/
	virtual void Reset(sf::Vector3f newPos);
	/*
	Resets object
	x: Sets Position.x.
	y: Sets Position.y.
	z: Sets Position.z.
	*/
	virtual void Reset(float x, float y, float z);

	virtual void Hurt(float damage);

	virtual void UpdateShader(V3DShader* shader, V3DCamera* camera);
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

