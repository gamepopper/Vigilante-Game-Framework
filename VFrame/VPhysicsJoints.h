#pragma once
#ifndef VFRAME_NO_PHYSICS
#include "VBase.h"
struct cpConstraint;
class VPhysicsObject;

class VPhysicsJoints : public VBase
{
private:
	cpConstraint* constraint;
	unsigned int type = -1;

public:
	typedef VBase VSUPERCLASS;

	enum VPhysicsJointType
	{
		PIN,
		SLIDE,
		PIVOT,
		GROOVE,
		DAMPED,
		DAMPEDROTARY,
		ROTARYLIMIT,
		RATCHET,
		GEAR,
		SIMPLEMOTOR,
		NUMJOINTTYPES
	};

	VPhysicsJoints(VPhysicsJointType type, VPhysicsObject* ObjectA, VPhysicsObject* ObjectB);
	virtual void Destroy() override;

	sf::Vector2f GetAnchorA();
	void SetanchorA(sf::Vector2f value);

	sf::Vector2f GetAnchorB();
	void SetAnchorB(sf::Vector2f value);
		
	sf::Vector2f GetGrooveA();
	void SetGrooveA(sf::Vector2f value);
	sf::Vector2f GetGrooveB();
	void SetGrooveB(sf::Vector2f value);
	
	float GetDist();
	void SetDist(float value);
	
	float GetMin();
	void SetMin(float value);
	float GetMax();
	void SetMax(float value);

	float GetRestAngle();
	void SetRestAngle(float value);

	float GetRestLength();
	void SetRestLength(float value);
	float GetStiffness();
	void SetStiffness(float value);
	float GetDamping();
	void SetDamping(float value);

	float GetAngle();
	void SetAngle(float value);
	float GetPhase();
	void SetPhase(float value);
	float GetRatchet();
	void SetRatchet(float value);

	float GetRatio();
	void SetRatio(float value);

	float GetRate();
	void SetRate(float value);

};
#endif