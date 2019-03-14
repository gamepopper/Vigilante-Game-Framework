#include "VPhysicsJoints.h"

#ifndef VFRAME_NO_PHYSICS
#include "VObject.h"
#include <chipmunk/chipmunk.h>

#include "VPhysicsObject.h"

VPhysicsJoints::VPhysicsJoints(VPhysicsJointType type, VPhysicsObject* ObjectA, VPhysicsObject* ObjectB)
{
	this->type = type;

	switch (type)
	{
	case PIN:
		constraint = cpPinJointNew(static_cast<cpBody*>(ObjectA->GetBody()), static_cast<cpBody*>(ObjectA->GetBody()), cpVect(), cpVect());
		break;
	case SLIDE:
		constraint = cpSlideJointNew(static_cast<cpBody*>(ObjectA->GetBody()), static_cast<cpBody*>(ObjectA->GetBody()), cpVect(), cpVect(), 0.0f, 1.0f);
		break;
	case PIVOT:
		constraint = cpPivotJointNew(static_cast<cpBody*>(ObjectA->GetBody()), static_cast<cpBody*>(ObjectA->GetBody()), cpVect());
		break;
	case GROOVE:
		constraint = cpGrooveJointNew(static_cast<cpBody*>(ObjectA->GetBody()), static_cast<cpBody*>(ObjectA->GetBody()), cpVect(), cpVect(), cpVect());
		break;
	case DAMPED:
		constraint = cpDampedSpringNew(static_cast<cpBody*>(ObjectA->GetBody()), static_cast<cpBody*>(ObjectA->GetBody()), cpVect(), cpVect(), 1.0f, 1.0f, 1.0f);
		break;
	case DAMPEDROTARY:
		constraint = cpDampedRotarySpringNew(static_cast<cpBody*>(ObjectA->GetBody()), static_cast<cpBody*>(ObjectA->GetBody()), 0.0f, 0.0f, 1.0f);
		break;
	case ROTARYLIMIT:
		constraint = cpRotaryLimitJointNew(static_cast<cpBody*>(ObjectA->GetBody()), static_cast<cpBody*>(ObjectA->GetBody()), 0.0f, 1.0f);
		break;
	case RATCHET:
		constraint = cpRatchetJointNew(static_cast<cpBody*>(ObjectA->GetBody()), static_cast<cpBody*>(ObjectA->GetBody()), 1.0f, 1.0f);
		break;
	case GEAR:
		constraint = cpGearJointNew(static_cast<cpBody*>(ObjectA->GetBody()), static_cast<cpBody*>(ObjectA->GetBody()), 0.0f, 1.0f);
		break;
	case SIMPLEMOTOR:
		constraint = cpSimpleMotorNew(static_cast<cpBody*>(ObjectA->GetBody()), static_cast<cpBody*>(ObjectA->GetBody()), 1.0f);
		break;
	}
}

void VPhysicsJoints::Destroy()
{
	VSUPERCLASS::Destroy();
	cpConstraintFree(constraint);
}

sf::Vector2f VPhysicsJoints::GetAnchorA()
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

void VPhysicsJoints::SetanchorA(sf::Vector2f value)
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

sf::Vector2f VPhysicsJoints::GetAnchorB()
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

void VPhysicsJoints::SetAnchorB(sf::Vector2f value)
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

sf::Vector2f VPhysicsJoints::GetGrooveA()
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

void VPhysicsJoints::SetGrooveA(sf::Vector2f value)
{
	cpVect groove = ToCPVect(value);

	switch (type)
	{
	case GROOVE:
		cpGrooveJointSetGrooveA(constraint, groove);
		break;
	}
}

sf::Vector2f VPhysicsJoints::GetGrooveB()
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

void VPhysicsJoints::SetGrooveB(sf::Vector2f value)
{
	cpVect groove = ToCPVect(value);

	switch (type)
	{
	case GROOVE:
		cpGrooveJointSetGrooveB(constraint, groove);
		break;
	}
}

float VPhysicsJoints::GetDist()
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

void VPhysicsJoints::SetDist(float value)
{
	switch (type)
	{
	case PIN:
		cpPinJointSetDist(constraint, value);
		break;
	}
}

float VPhysicsJoints::GetMin()
{
	float min = 0.0f;

	switch (type)
	{
	case ROTARYLIMIT:
		min = (float)cpRotaryLimitJointGetMin(constraint);
		break;
	}

	return min;
}

void VPhysicsJoints::SetMin(float value)
{
	switch (type)
	{
	case ROTARYLIMIT:
		cpRotaryLimitJointSetMin(constraint, value);
		break;
	}
}

float VPhysicsJoints::GetMax()
{
	float max = 0.0f;

	switch (type)
	{
	case ROTARYLIMIT:
		max = (float)cpRotaryLimitJointGetMax(constraint);
		break;
	}

	return max;
}

void VPhysicsJoints::SetMax(float value)
{
	switch (type)
	{
	case ROTARYLIMIT:
		cpRotaryLimitJointSetMax(constraint, value);
		break;
	}
}

float VPhysicsJoints::GetRestAngle()
{
	float angle = 0.0f;

	switch (type)
	{
	case DAMPED:
		angle = (float)cpDampedRotarySpringGetRestAngle(constraint);
		break;
	}

	return angle;
}

void VPhysicsJoints::SetRestAngle(float value)
{
	switch (type)
	{
	case DAMPED:
		cpDampedRotarySpringSetRestAngle(constraint, value);
		break;
	}
}

float VPhysicsJoints::GetRestLength()
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

void VPhysicsJoints::SetRestLength(float value)
{
	switch (type)
	{
	case DAMPED:
		cpDampedSpringSetRestLength(constraint, value);
		break;
	}
}

float VPhysicsJoints::GetStiffness()
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

void VPhysicsJoints::SetStiffness(float value)
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

float VPhysicsJoints::GetDamping()
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

void VPhysicsJoints::SetDamping(float value)
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

float VPhysicsJoints::GetAngle()
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

void VPhysicsJoints::SetAngle(float value)
{
	switch (type)
	{
	case RATCHET:
		cpRatchetJointSetAngle(constraint, value);
		break;
	}
}

float VPhysicsJoints::GetPhase()
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

void VPhysicsJoints::SetPhase(float value)
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

float VPhysicsJoints::GetRatchet()
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

void VPhysicsJoints::SetRatchet(float value)
{
	switch (type)
	{
	case RATCHET:
		cpRatchetJointSetRatchet(constraint, value);
		break;
	}
}

float VPhysicsJoints::GetRatio()
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

void VPhysicsJoints::SetRatio(float value)
{
	switch (type)
	{
	case GEAR:
		cpGearJointSetRatio(constraint, value);
		break;
	}
}

float VPhysicsJoints::GetRate()
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

void VPhysicsJoints::SetRate(float value)
{
	switch (type)
	{
	case SIMPLEMOTOR:
		cpSimpleMotorSetRate(constraint, value);
		break;
	}
}

#endif