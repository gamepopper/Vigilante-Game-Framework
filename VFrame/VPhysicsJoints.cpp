#include "VPhysicsJoints.h"

#ifndef VFRAME_NO_PHYSICS
#include "VObject.h"
#include <chipmunk/chipmunk.h>

VPhysicsJointBase::VPhysicsJointBase(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB)
{
	bodyA = BodyA;
	bodyB = BodyB;
}

void VPhysicsJointBase::Destroy()
{
	VSUPERCLASS::Destroy();
	cpConstraintFree(constraint);
}

void VPhysicsJointBase::Initialise(VPhysicsCPSpace* space)
{
	cpSpaceAddConstraint(space, constraint);
}

void VPhysicsJointBase::Deinitialise(VPhysicsCPSpace* space)
{
	cpSpaceRemoveConstraint(space, constraint);
}

sf::Vector2f VPhysicsJointBase::GetAnchorA()
{
	cpVect anchor;

	switch (type)
	{
	case PIN:
		anchor = cpPinJointGetAnchorA(constraint);
		break;
	case SLIDE:
		anchor = cpSlideJointGetAnchorA(constraint);
		break;
	case PIVOT:
		anchor = cpPivotJointGetAnchorA(constraint);
		break;
	case DAMPED:
		anchor = cpDampedSpringGetAnchorA(constraint);
		break;
	}

	return sf::Vector2f((float)anchor.x, (float)anchor.x);
}

void VPhysicsJointBase::SetAnchorA(sf::Vector2f value)
{
	cpVect v = ToCPVect(value);

	switch (type)
	{
	case PIN:
		cpPinJointSetAnchorA(constraint, v);
		break;
	case SLIDE:
		cpSlideJointSetAnchorA(constraint, v);
		break;
	case PIVOT:
		cpPivotJointSetAnchorA(constraint, v);
		break;
	case DAMPED:
		cpDampedSpringSetAnchorA(constraint, v);
		break;
	}
}

sf::Vector2f VPhysicsJointBase::GetAnchorB()
{
	cpVect anchor;

	switch (type)
	{
	case PIN:
		anchor = cpPinJointGetAnchorB(constraint);
		break;
	case SLIDE:
		anchor = cpSlideJointGetAnchorB(constraint);
		break;
	case PIVOT:
		anchor = cpPivotJointGetAnchorB(constraint);
		break;
	case GROOVE:
		anchor = cpGrooveJointGetAnchorB(constraint);
		break;
	case DAMPED:
		anchor = cpDampedSpringGetAnchorB(constraint);
		break;
	}

	return sf::Vector2f((float)anchor.x, (float)anchor.x);
}

void VPhysicsJointBase::SetAnchorB(sf::Vector2f value)
{
	cpVect v = ToCPVect(value);

	switch (type)
	{
	case PIN:
		cpPinJointSetAnchorB(constraint, v);
		break;
	case SLIDE:
		cpSlideJointSetAnchorB(constraint, v);
		break;
	case PIVOT:
		cpPivotJointSetAnchorB(constraint, v);
		break;
	case GROOVE:
		cpGrooveJointSetAnchorB(constraint, v);
		break;
	case DAMPED:
		cpDampedSpringSetAnchorB(constraint, v);
		break;
	}
}

sf::Vector2f VPhysicsJointBase::GetGrooveA()
{
	cpVect groove;

	switch (type)
	{
	case GROOVE:
		groove = cpGrooveJointGetGrooveA(constraint);
		break;
	}

	return sf::Vector2f((float)groove.x, (float)groove.x);
}

void VPhysicsJointBase::SetGrooveA(sf::Vector2f value)
{
	cpVect groove = ToCPVect(value);

	switch (type)
	{
	case GROOVE:
		cpGrooveJointSetGrooveA(constraint, groove);
		break;
	}
}

sf::Vector2f VPhysicsJointBase::GetGrooveB()
{
	cpVect groove;

	switch (type)
	{
	case GROOVE:
		groove = cpGrooveJointGetGrooveB(constraint);
		break;
	}

	return sf::Vector2f((float)groove.x, (float)groove.x);
}

void VPhysicsJointBase::SetGrooveB(sf::Vector2f value)
{
	cpVect groove = ToCPVect(value);

	switch (type)
	{
	case GROOVE:
		cpGrooveJointSetGrooveB(constraint, groove);
		break;
	}
}

float VPhysicsJointBase::GetDist()
{
	float dist = 0.0f;

	switch (type)
	{
	case PIN:
		dist = (float)cpPinJointGetDist(constraint);
		break;
	}

	return dist;
}

void VPhysicsJointBase::SetDist(float value)
{
	switch (type)
	{
	case PIN:
		cpPinJointSetDist(constraint, value);
		break;
	}
}

float VPhysicsJointBase::GetMin()
{
	float min = 0.0f;

	switch (type)
	{
	case SLIDE:
		min = (float)cpSlideJointGetMin(constraint);
		break;
	case ROTARYLIMIT:
		min = (float)cpRotaryLimitJointGetMin(constraint);
		break;
	}

	return min;
}

void VPhysicsJointBase::SetMin(float value)
{
	switch (type)
	{
	case SLIDE:
		cpSlideJointSetMin(constraint, value);
		break;
	case ROTARYLIMIT:
		cpRotaryLimitJointSetMin(constraint, value);
		break;
	}
}

float VPhysicsJointBase::GetMax()
{
	float max = 0.0f;

	switch (type)
	{
	case SLIDE:
		max = (float)cpSlideJointGetMax(constraint);
		break;
	case ROTARYLIMIT:
		max = (float)cpRotaryLimitJointGetMax(constraint);
		break;
	}

	return max;
}

void VPhysicsJointBase::SetMax(float value)
{
	switch (type)
	{
	case SLIDE:
		cpSlideJointSetMax(constraint, value);
		break;
	case ROTARYLIMIT:
		cpRotaryLimitJointSetMax(constraint, value);
		break;
	}
}

float VPhysicsJointBase::GetRestAngle()
{
	float angle = 0.0f;

	switch (type)
	{
	case DAMPEDROTARY:
		angle = (float)cpDampedRotarySpringGetRestAngle(constraint);
		break;
	}

	return angle;
}

void VPhysicsJointBase::SetRestAngle(float value)
{
	switch (type)
	{
	case DAMPEDROTARY:
		cpDampedRotarySpringSetRestAngle(constraint, value);
		break;
	}
}

float VPhysicsJointBase::GetRestLength()
{
	float length = 0.0f;

	switch (type)
	{
	case DAMPED:
		length = (float)cpDampedSpringGetRestLength(constraint);
		break;
	}

	return length;
}

void VPhysicsJointBase::SetRestLength(float value)
{
	switch (type)
	{
	case DAMPED:
		cpDampedSpringSetRestLength(constraint, value);
		break;
	}
}

float VPhysicsJointBase::GetStiffness()
{
	float stiffness = 0.0f;

	switch (type)
	{
	case DAMPED:
		stiffness = (float)cpDampedSpringGetStiffness(constraint);
		break;
	case DAMPEDROTARY:
		stiffness = (float)cpDampedRotarySpringGetStiffness(constraint);
		break;
	}

	return stiffness;
}

void VPhysicsJointBase::SetStiffness(float value)
{
	switch (type)
	{
	case DAMPED:
		cpDampedSpringSetStiffness(constraint, value);
		break;
	case DAMPEDROTARY:
		cpDampedRotarySpringSetStiffness(constraint, value);
		break;
	}
}

float VPhysicsJointBase::GetDamping()
{
	float damping = 0.0f;

	switch (type)
	{
	case DAMPED:
		damping = (float)cpDampedSpringGetDamping(constraint);
		break;
	case DAMPEDROTARY:
		damping = (float)cpDampedRotarySpringGetDamping(constraint);
		break;
	}

	return damping;
}

void VPhysicsJointBase::SetDamping(float value)
{
	switch (type)
	{
	case DAMPED:
		cpDampedSpringSetDamping(constraint, value);
		break;
	case DAMPEDROTARY:
		cpDampedRotarySpringSetDamping(constraint, value);
		break;
	}
}

float VPhysicsJointBase::GetAngle()
{
	float angle = 0.0f;

	switch (type)
	{
	case RATCHET:
		angle = (float)cpRatchetJointGetAngle(constraint);
		break;
	}

	return angle;
}

void VPhysicsJointBase::SetAngle(float value)
{
	switch (type)
	{
	case RATCHET:
		cpRatchetJointSetAngle(constraint, value);
		break;
	}
}

float VPhysicsJointBase::GetPhase()
{
	float phase = 0.0f;

	switch (type)
	{
	case RATCHET:
		phase = (float)cpRatchetJointGetPhase(constraint);
		break;
	case GEAR:
		phase = (float)cpGearJointGetPhase(constraint);
		break;
	}

	return phase;
}

void VPhysicsJointBase::SetPhase(float value)
{
	switch (type)
	{
	case RATCHET:
		cpRatchetJointSetPhase(constraint, value);
		break;
	case GEAR:
		cpGearJointSetPhase(constraint, value);
		break;
	}
}

float VPhysicsJointBase::GetRatchet()
{
	float ratchet = 0.0f;

	switch (type)
	{
	case RATCHET:
		ratchet = (float)cpRatchetJointGetRatchet(constraint);
		break;
	}

	return ratchet;
}

void VPhysicsJointBase::SetRatchet(float value)
{
	switch (type)
	{
	case RATCHET:
		cpRatchetJointSetRatchet(constraint, value);
		break;
	}
}

float VPhysicsJointBase::GetRatio()
{
	float ratio = 0.0f;

	switch (type)
	{
	case GEAR:
		ratio = (float)cpGearJointGetRatio(constraint);
		break;
	}

	return ratio;
}

void VPhysicsJointBase::SetRatio(float value)
{
	switch (type)
	{
	case GEAR:
		cpGearJointSetRatio(constraint, value);
		break;
	}
}

float VPhysicsJointBase::GetRate()
{
	float rate = 0.0f;

	switch (type)
	{
	case SIMPLEMOTOR:
		rate = (float)cpSimpleMotorGetRate(constraint);
		break;
	}

	return rate;
}

void VPhysicsJointBase::SetRate(float value)
{
	switch (type)
	{
	case SIMPLEMOTOR:
		cpSimpleMotorSetRate(constraint, value);
		break;
	}
}

VPhysicsPinJoint::VPhysicsPinJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f AnchorA, sf::Vector2f AnchorB) : VPhysicsJointBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), AnchorA, AnchorB);
}

VPhysicsPinJoint::VPhysicsPinJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB) : VPhysicsJointBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, AnchorA, AnchorB);
}

void VPhysicsPinJoint::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB)
{
	type = PIN;
	constraint = cpPinJointNew(BodyA, BodyB, ToCPVect(AnchorA), ToCPVect(AnchorB));
}

VPhysicsSlideJoint::VPhysicsSlideJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float Min, float Max) : VPhysicsJointBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), AnchorA, AnchorB, Min, Max);
}

VPhysicsSlideJoint::VPhysicsSlideJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float Min, float Max) : VPhysicsJointBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, AnchorA, AnchorB, Min, Max);
}

void VPhysicsSlideJoint::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float Min, float Max)
{
	type = SLIDE;
	constraint = cpSlideJointNew(BodyA, BodyB, ToCPVect(AnchorA), ToCPVect(AnchorB), Min, Max);
}

VPhysicsPivotJoint::VPhysicsPivotJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f Pivot) : VPhysicsJointBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), Pivot);
}

VPhysicsPivotJoint::VPhysicsPivotJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f Pivot) : VPhysicsJointBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, Pivot);
}

void VPhysicsPivotJoint::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f Pivot)
{
	type = PIVOT;
	constraint = cpPivotJointNew(BodyA, BodyB, ToCPVect(Pivot));
}

VPhysicsGrooveJoint::VPhysicsGrooveJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f GrooveA, sf::Vector2f GrooveB, sf::Vector2f AnchorB) : VPhysicsJointBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), GrooveA, GrooveB, AnchorB);
}

VPhysicsGrooveJoint::VPhysicsGrooveJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f GrooveA, sf::Vector2f GrooveB, sf::Vector2f AnchorB) : VPhysicsJointBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, GrooveA, GrooveB, AnchorB);
}

void VPhysicsGrooveJoint::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f GrooveA, sf::Vector2f GrooveB, sf::Vector2f AnchorB)
{
	type = GROOVE;
	constraint = cpGrooveJointNew(BodyA, BodyB, ToCPVect(GrooveA), ToCPVect(GrooveB), ToCPVect(AnchorB));
}

VPhysicsDampedSpring::VPhysicsDampedSpring(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float RestLength, float Stiffness, float Damping) : VPhysicsJointBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), AnchorA, AnchorB, RestLength, Stiffness, Damping);
}

VPhysicsDampedSpring::VPhysicsDampedSpring(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float RestLength, float Stiffness, float Damping) : VPhysicsJointBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, AnchorA, AnchorB, RestLength, Stiffness, Damping);
}

void VPhysicsDampedSpring::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float RestLength, float Stiffness, float Damping)
{
	type = DAMPED;
	constraint = cpDampedSpringNew(BodyA, BodyB, ToCPVect(AnchorA), ToCPVect(AnchorB), RestLength, Stiffness, Damping);
}

VPhysicsDampedRotarySpring::VPhysicsDampedRotarySpring(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float RestLength, float Stiffness, float Damping) : VPhysicsJointBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), RestLength, Stiffness, Damping);
}

VPhysicsDampedRotarySpring::VPhysicsDampedRotarySpring(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float RestLength, float Stiffness, float Damping) : VPhysicsJointBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, RestLength, Stiffness, Damping);
}

void VPhysicsDampedRotarySpring::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float RestLength, float Stiffness, float Damping)
{
	type = DAMPEDROTARY;
	constraint = cpDampedRotarySpringNew(BodyA, BodyB, RestLength, Stiffness, Damping);
}

VPhysicsRotaryLimitJoint::VPhysicsRotaryLimitJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float Min, float Max) : VPhysicsJointBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), Min, Max);
}

VPhysicsRotaryLimitJoint::VPhysicsRotaryLimitJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Min, float Max) : VPhysicsJointBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, Min, Max);
}

void VPhysicsRotaryLimitJoint::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Min, float Max)
{
	type = ROTARYLIMIT;
	constraint = cpRotaryLimitJointNew(BodyA, BodyB, Min, Max);
}

VPhysicsRatchetJoint::VPhysicsRatchetJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float Phase, float Ratchet) : VPhysicsJointBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), Phase, Ratchet);
}

VPhysicsRatchetJoint::VPhysicsRatchetJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Phase, float Ratchet) : VPhysicsJointBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, Phase, Ratchet);
}

void VPhysicsRatchetJoint::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Phase, float Ratchet)
{
	type = RATCHET;
	constraint = cpRatchetJointNew(BodyA, BodyB, Phase, Ratchet);
}

VPhysicsGearJoint::VPhysicsGearJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float Phase, float Ratio) : VPhysicsJointBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), Phase, Ratio);
}

VPhysicsGearJoint::VPhysicsGearJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Phase, float Ratio) : VPhysicsJointBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, Phase, Ratio);
}

void VPhysicsGearJoint::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Phase, float Ratio)
{
	type = GEAR;
	constraint = cpGearJointNew(BodyA, BodyB, Phase, Ratio);
}

VPhysicsSimpleMotor::VPhysicsSimpleMotor(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float Rate) : VPhysicsJointBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), Rate);
}

VPhysicsSimpleMotor::VPhysicsSimpleMotor(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Rate) : VPhysicsJointBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, Rate);
}

void VPhysicsSimpleMotor::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Rate)
{
	type = SIMPLEMOTOR;
	constraint = cpSimpleMotorNew(BodyA, BodyB, Rate);
}

#endif
