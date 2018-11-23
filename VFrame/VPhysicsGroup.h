#pragma once

#ifndef VFRAME_NO_PHYSICS
#include "VGroup.h"
#include "VPhysicsObject.h"

struct VPhysicsArbiter
{
	sf::Vector2f PointA;
	sf::Vector2f PointB;
};

struct CollisionCallbackHelper;
class VPhysicsGroup : public VGroup
{
protected:
	cpSpace* space;
	cpCollisionHandler* collisionHandler;
	
	std::vector<CollisionCallbackHelper> callbackHelperList;

	VPhysicsObject* getObjectFromBody(cpBody* body);

public:
	typedef VGroup VSUPERCLASS;

	VPhysicsGroup(unsigned int Amount = 0);
	
	VPhysicsObject* AddObject(VObject* Object, VPhysicsObject::VObjectType BodyType, VPhysicsObject::VObjectShape Shape = VPhysicsObject::BOX, std::vector<sf::Vector2f> Verts = {});
	bool RemoveObject(VObject* Object);
	
	virtual void Destroy() override;
	virtual void Update(float dt) override;

	int GetIterations();
	void SetIterations(int Iterations);

	sf::Vector2f GetGravity();
	void SetGravity(sf::Vector2f Gravity);

	float GetDamping();
	void SetDamping(float Damping);

	float GetIdleSpeedThreshold();
	void SetIdleSpeedThreshold(float IdleSpeedThreshold);

	float GetSleepTimeThreshold();
	void SetSleepTimeThreshold(float SleepTimeThreshold);

	float GetCollisionSlop();
	void SetCollisionSlop(float CollisionSlop);

	float GetCollisionBias();
	void SetCollisionBias(float CollisionBias);

	unsigned int GetCollisionPersistence();
	void SetCollisionPersistence(unsigned int CollisionPersistence);

	enum VPhysicsCallbackType
	{
		BEGIN,
		PRESOLVE,
		POSTSOLVE,
		SEPARATE,
	};

	void SetCollisionCallback(VObject* a, VObject* b, const std::function<bool(VPhysicsObject*, VPhysicsObject*)>& callback, VPhysicsCallbackType type, bool persist = false);
	bool ProcessCallback(cpArbiter *arb, cpSpace *space, VPhysicsCallbackType type);

	//void* GetUserData();
	//void SetUserData(void* userData);

#ifdef _DEBUG
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
#endif

};
#endif
