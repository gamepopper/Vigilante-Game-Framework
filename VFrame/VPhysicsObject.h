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
cpVect ToCPVect(const sf::Vector2f& v);
sf::Vector2f ToSFVector(const cpVect& v);

class VPhysicsObject : public VBase
{
protected:
	VObject* baseObject;
	cpBody* body;
	cpShape* shape;
	unsigned int getPolyVertCount;

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
	VObjectShape shapeType;
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