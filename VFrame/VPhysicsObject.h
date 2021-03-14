/**
* @file    VPhysicsObject.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2020 Tim Stoddard
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

///Accessor to cpVect struct.
typedef struct cpVect cpVect;
///Accessor to cpBody struct.
typedef struct cpBody VPhysicsCPBody;
///Accessor to cpShape struct.
typedef struct cpShape VPhysicsCPShape;
///Accessor to cpSpace struct.
typedef struct cpSpace VPhysicsCPSpace;
///Accessor to cpArbiter struct.
typedef struct cpArbiter VPhysicsCPArbiter;
///Accessor to cpCollisionHandler struct.
typedef struct cpCollisionHandler VPhysicsCollision;
///Accessor to cpShapeFilter struct.
typedef struct cpShapeFilter VPhysicsCollisionFilter;

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
	///Used to call parent class functions when they are overrided in class.
	typedef VBase VSUPERCLASS;

	///The shape of the collision area of the physics object.
	enum VObjectShape : unsigned char
	{
		CIRCLE,
		BOX,
		LINE,
		CUSTOM
	};

	///The physics type of the physics object.
	enum VObjectType : unsigned char
	{
		DYNAMIC,
		KINEMATIC,
		STATIC
	};

	enum VPhysicsLock
	{
		XPOS = 1 << 0,
		XVEL = 1 << 1,
		YPOS = 1 << 2,
		YVEL = 1 << 3,
		ANGLE = 1 << 4,
		ANGLEVEL = 1 << 5,
	};

	unsigned int Lock = 0;

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
	///A fast collision filter, defaults to allowing collisions from all objects.
	cpShapeFilter* filter;

public:
	/**
	* @param Object The game object to apply physics towards.
	* @param BodyType The type of physics body to set the physics object to.
	* @param Shape The type of collision shape the object will have. Default is BOX.
	* @param Verts If the shape set is LINE or CUSTOM, this parameter will define the shape in local space (relative to the top-left of the game object).
	*/
	VPhysicsObject(VObject* Object, VObjectType BodyType, VObjectShape Shape = VObjectShape::BOX, std::vector<sf::Vector2f> Verts = {});

	/**
	* Initialises the physics object.
	* @param space The physics space to initialise the object to.
	*/
	void Initialise(VPhysicsCPSpace* space);

	///@return Gets the base game object that physics is being applied to.
	VObject* GetBaseObject();
	
	/**
	* Deinitises and removes the physics object from the world.
	* @param space The physics space to remove the object from.
	*/
	void Deinitialise(VPhysicsCPSpace* space);

	///Destroys the physics data of the object.
	virtual void Destroy() override;
	
	/**
	* Updates the physics object.
	* @param dt The delta time between the previous and current frame.
	*/
	virtual void Update(float dt) override;

	///@return Gets the physics body as a void pointer (so it can be cast to a cpBody if necessary).
	VPhysicsCPBody* GetBody();
	
	///@return Gets the physics shape as a void pointer (so it can be cast to a cpShape if necessary).
	VPhysicsCPShape* GetShape();

	//No direct way to change the shape of the physics object without replacing it.
	//void SetShapeType(VObjectShape shape);
	///@return The type of shape of the physics object.
	VObjectShape GetShapeType();

	/**
	* Update the Collisionfilter.
	* @param group Group id, for filtering out all shapes that have the same id. No Group = 0
	* @param category Category id, for other objects to filter out if their category is in the mask.
	* @param mask Mask for objects to filter out by a specific category.
	*/
	void UpdateCollisionFilter(unsigned int group = 0, unsigned int category = -1, unsigned int mask = -1);
	
	/**
	* Set the physics body of the object.
	* @param bodyType The new phyiscs body type.
	*/
	void SetBodyType(VObjectType bodyType);
	///@return The tyoe of physics body of the physics object.
	VObjectType GetBodyType();

	///@return The density of the physics shape.
	float GetDensity();

	/**
	* Sets the shape density.
	* @param density The new density value of the physics shape.
	*/
	void SetDensity(float density);

	///@return The friction of the physics shape.
	float GetFriction();
	
	/**
	* Sets the shape friction.
	* @param friction The new friction value of the physics shape.
	*/
	void SetFriction(float friction);

	///@return The surface velocity of the physics shape.
	sf::Vector2f GetSurfaceVelocity();

	/**
	* Sets the surface velocity of the physics shape.
	* @param surfaceVelocity The new surface velocity of the physics shape.
	*/
	void SetSurfaceVelocity(sf::Vector2f surfaceVelocity);

	///@return The force of the physics body.
	sf::Vector2f GetForce();

	/**
	* Sets the force of the physics body.
	* @param force The new force value of the physics body.
	*/
	void SetForce(sf::Vector2f force);

	///@return The torque of the physics body.
	float GetTorque();
	
	/**
	* Sets the torque of the physics body.
	* @param torque The new torque of the physics body.
	*/
	void SetTorque(float torque);

	/**
	* Applys a force towards the phyics body at a point in world space.
	* @param force The force to apply.
	* @param point The point in world space to apply the force to.
	*/
	void ApplyForceAtWorldPoint(sf::Vector2f force, sf::Vector2f point);

	/**
	* Applys a force towards the phyics body at a point in local space.
	* @param force The force to apply.
	* @param point The point in local space to apply the force to.
	*/
	void ApplyForceAtLocalPoint(sf::Vector2f force, sf::Vector2f point);

	/**
	* Applys an impluse towards the phyics body at a point in world space.
	* @param impulse The impulse to apply.
	* @param point The point in world space to apply the force to.
	*/
	void ApplyImpulseAtWorldPoint(sf::Vector2f impulse, sf::Vector2f point);

	/**
	* Applys an impulse towards the phyics body at a point in local space.
	* @param impulse The impulse to apply.
	* @param point The point in local space to apply the force to.
	*/
	void ApplyImpulseAtLocalPoint(sf::Vector2f impulse, sf::Vector2f point);
};
#endif