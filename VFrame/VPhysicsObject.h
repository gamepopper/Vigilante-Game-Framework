/**
* @file    VVPhysicsObject.h
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
* A special object that represents the physics body that responds to the world and collisions using a specified shape.
*/

#pragma once

#ifndef VFRAME_NO_PHYSICS
#include "VBase.h"
#include <vector>
class VObject;
struct cpBody;
struct cpShape;
struct cpSpace;
struct cpArbiter;
struct cpCollisionHandler;

typedef struct cpVect cpVect;
typedef void * cpDataPointer;

/**
* Helper function to convert a sf::Vector2f to a cpVect.
* @param v The sf::Vector2f to convert.
* @return the resulting cpVect.
*/
cpVect ToCPVect(const sf::Vector2f& v);

/**
* Helper function to convert a cpVect sf::Vector2f.
* @param v The cpVect to convert.
* @return the resulting sf::Vector2f.
*/
sf::Vector2f ToSFVector(const cpVect& v);

///A special object class that represents the physics body that responds to the world and collisions using a specified shape, should be used by adding to VPhysicsGroup.
class VPhysicsObject : public VBase
{
public:
	typedef VBase VSUPERCLASS;

	enum VObjectShape
	{
		CIRCLE,
		BOX,
		LINE,
		CUSTOM
	};

	enum VObjectType
	{
		KINEMATIC,
		DYNAMIC,
		STATIC
	};

protected:
	///The object to apply the angle and position from the physics body.
	VObject* baseObject;
	///The physics body.
	cpBody* body;
	///The shape of the physics body.
	cpShape* shape;
	///The amount of vertices of the shape if of type CUSTOM.
	unsigned int getPolyVertCount;
	///The type of shape of the physics object.
	VObjectShape shapeType;
	///The type of body of the physics object.
	VObjectType bodyType;

public:
	VPhysicsObject(VObject* Object, VObjectType BodyType, VObjectShape Shape = VObjectShape::BOX, std::vector<sf::Vector2f> Verts = {});

	bool LockX = false;
	bool LockY = false;
	bool LockAngle = false;

	void Initialise(cpSpace* space);
	VObject* GetBaseObject();
	void Deinitialise(cpSpace* space);

	virtual void Destroy() override;
	virtual void Update(float dt) override;

	void* GetBody();
	void* GetShape();

	float GetDensity();
	void SetDensity(float density);

	float GetFriction();
	void SetFriction(float friction);

	sf::Vector2f GetSurfaceVelocity();
	void SetSurfaceVelocity(sf::Vector2f surfaceVelocity);

	sf::Vector2f GetForce();
	void SetForce(sf::Vector2f force);

	float GetTorque();
	void SetTorque(float torque);

	void ApplyForceAtWorldPoint(sf::Vector2f force, sf::Vector2f point);
	void ApplyForceAtLocalPoint(sf::Vector2f force, sf::Vector2f point);

	void ApplyImpulseAtWorldPoint(sf::Vector2f impulse, sf::Vector2f point);
	void ApplyImpulseAtLocalPoint(sf::Vector2f impulse, sf::Vector2f point);
};
#endif