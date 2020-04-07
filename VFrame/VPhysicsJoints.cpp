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

VPhysicsCPConstraint * VPhysicsJointBase::GetConstraint()
{
	return constraint;
}

VPhysicsCPBody* VPhysicsJointBase::GetBodyA()
{
	return cpConstraintGetBodyA(constraint);
}

VPhysicsCPBody* VPhysicsJointBase::GetBodyB()
{
	return cpConstraintGetBodyB(constraint);
}

void VPhysicsJointBase::SetMaxForce(float Max)
{
	cpConstraintSetMaxForce(constraint, Max);
}

float VPhysicsJointBase::GetMaxForce()
{
	return (float)cpConstraintGetMaxForce(constraint);
}

void VPhysicsJointBase::SetErrorBias(float Bias)
{
	cpConstraintSetErrorBias(constraint, Bias);
}

float VPhysicsJointBase::GetErrorBias()
{
	return (float)cpConstraintGetErrorBias(constraint);
}

void VPhysicsJointBase::SetMaxBias(float Bias)
{
	cpConstraintSetMaxBias(constraint, Bias);
}

float VPhysicsJointBase::GetMaxBias()
{
	return (float)cpConstraintGetMaxBias(constraint);
}

void VPhysicsJointBase::SetCollideBodies(bool value)
{
	cpConstraintSetCollideBodies(constraint, value);
}

bool VPhysicsJointBase::DoesCollideBodies()
{
	return cpConstraintGetCollideBodies(constraint);
}

float VPhysicsJointBase::GetImpulse()
{
	return (float)cpConstraintGetImpulse(constraint);
}

VPhysicsAnchorBase::VPhysicsAnchorBase(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB) : VPhysicsJointBase(BodyA, BodyB)
{
}

sf::Vector2f VPhysicsAnchorBase::GetAnchorA()
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

void VPhysicsAnchorBase::SetAnchorA(sf::Vector2f value)
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

sf::Vector2f VPhysicsAnchorBase::GetAnchorB()
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

void VPhysicsAnchorBase::SetAnchorB(sf::Vector2f value)
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

sf::Vector2f VPhysicsGrooveJoint::GetGrooveA()
{
	return ToSFVector(cpGrooveJointGetGrooveA(constraint));
}

void VPhysicsGrooveJoint::SetGrooveA(sf::Vector2f value)
{
	cpVect groove = ToCPVect(value);
	cpGrooveJointSetGrooveA(constraint, groove);
}

sf::Vector2f VPhysicsGrooveJoint::GetGrooveB()
{
	return ToSFVector(cpGrooveJointGetGrooveB(constraint));
}

void VPhysicsGrooveJoint::SetGrooveB(sf::Vector2f value)
{
	cpVect groove = ToCPVect(value);
	cpGrooveJointSetGrooveB(constraint, groove);
}

float VPhysicsPinJoint::GetDist()
{
	return (float)cpPinJointGetDist(constraint);
}

void VPhysicsPinJoint::SetDist(float value)
{
	cpPinJointSetDist(constraint, value);
}

VPhysicsMinMaxBase::VPhysicsMinMaxBase(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB) : VPhysicsAnchorBase(BodyA, BodyB)
{
}

float VPhysicsMinMaxBase::GetMin()
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

void VPhysicsMinMaxBase::SetMin(float value)
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

float VPhysicsMinMaxBase::GetMax()
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

void VPhysicsMinMaxBase::SetMax(float value)
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

float VPhysicsDampedRotarySpring::GetRestAngle()
{
	return (float)cpDampedRotarySpringGetRestAngle(constraint);
}

void VPhysicsDampedRotarySpring::SetRestAngle(float value)
{
	cpDampedRotarySpringSetRestAngle(constraint, value);
}

float VPhysicsDampedSpring::GetRestLength()
{
	return (float)cpDampedSpringGetRestLength(constraint);
}

void VPhysicsDampedSpring::SetRestLength(float value)
{
	cpDampedSpringSetRestLength(constraint, value);
}

VPhysicsDampedBase::VPhysicsDampedBase(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB) : VPhysicsAnchorBase(BodyA, BodyB)
{
}

float VPhysicsDampedBase::GetStiffness()
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

void VPhysicsDampedBase::SetStiffness(float value)
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

float VPhysicsDampedBase::GetDamping()
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

void VPhysicsDampedBase::SetDamping(float value)
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

float VPhysicsRatchetJoint::GetAngle()
{
	return (float)cpRatchetJointGetAngle(constraint);
}

void VPhysicsRatchetJoint::SetAngle(float value)
{
	cpRatchetJointSetAngle(constraint, value);
}

VPhysicsPhaseBase::VPhysicsPhaseBase(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB) : VPhysicsJointBase(BodyA, BodyB)
{
}

float VPhysicsPhaseBase::GetPhase()
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

void VPhysicsPhaseBase::SetPhase(float value)
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

float VPhysicsRatchetJoint::GetRatchet()
{
	return (float)cpRatchetJointGetRatchet(constraint);
}

void VPhysicsRatchetJoint::SetRatchet(float value)
{
	cpRatchetJointSetRatchet(constraint, value);
}

float VPhysicsGearJoint::GetRatio()
{
	return (float)cpGearJointGetRatio(constraint);
}

void VPhysicsGearJoint::SetRatio(float value)
{
	cpGearJointSetRatio(constraint, value);
}

float VPhysicsSimpleMotor::GetRate()
{
	return (float)cpSimpleMotorGetRate(constraint);
}

void VPhysicsSimpleMotor::SetRate(float value)
{
	cpSimpleMotorSetRate(constraint, value);
}

VPhysicsPinJoint::VPhysicsPinJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f AnchorA, sf::Vector2f AnchorB) : VPhysicsAnchorBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), AnchorA, AnchorB);
}

VPhysicsPinJoint::VPhysicsPinJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB) : VPhysicsAnchorBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, AnchorA, AnchorB);
}

void VPhysicsPinJoint::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB)
{
	type = PIN;
	constraint = cpPinJointNew(BodyA, BodyB, ToCPVect(AnchorA), ToCPVect(AnchorB));
}

VPhysicsSlideJoint::VPhysicsSlideJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float Min, float Max) : VPhysicsMinMaxBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), AnchorA, AnchorB, Min, Max);
}

VPhysicsSlideJoint::VPhysicsSlideJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float Min, float Max) : VPhysicsMinMaxBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, AnchorA, AnchorB, Min, Max);
}

void VPhysicsSlideJoint::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float Min, float Max)
{
	type = SLIDE;
	constraint = cpSlideJointNew(BodyA, BodyB, ToCPVect(AnchorA), ToCPVect(AnchorB), Min, Max);
}

VPhysicsPivotJoint::VPhysicsPivotJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f Pivot) : VPhysicsAnchorBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), Pivot);
}

VPhysicsPivotJoint::VPhysicsPivotJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f Pivot) : VPhysicsAnchorBase(BodyA, BodyB)
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

VPhysicsDampedSpring::VPhysicsDampedSpring(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float RestLength, float Stiffness, float Damping) : VPhysicsDampedBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), AnchorA, AnchorB, RestLength, Stiffness, Damping);
}

VPhysicsDampedSpring::VPhysicsDampedSpring(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float RestLength, float Stiffness, float Damping) : VPhysicsDampedBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, AnchorA, AnchorB, RestLength, Stiffness, Damping);
}

void VPhysicsDampedSpring::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float RestLength, float Stiffness, float Damping)
{
	type = DAMPED;
	constraint = cpDampedSpringNew(BodyA, BodyB, ToCPVect(AnchorA), ToCPVect(AnchorB), RestLength, Stiffness, Damping);
}

VPhysicsDampedRotarySpring::VPhysicsDampedRotarySpring(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float RestLength, float Stiffness, float Damping) : VPhysicsDampedBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), RestLength, Stiffness, Damping);
}

VPhysicsDampedRotarySpring::VPhysicsDampedRotarySpring(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float RestLength, float Stiffness, float Damping) : VPhysicsDampedBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, RestLength, Stiffness, Damping);
}

void VPhysicsDampedRotarySpring::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float RestLength, float Stiffness, float Damping)
{
	type = DAMPEDROTARY;
	constraint = cpDampedRotarySpringNew(BodyA, BodyB, RestLength, Stiffness, Damping);
}

VPhysicsRotaryLimitJoint::VPhysicsRotaryLimitJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float Min, float Max) : VPhysicsMinMaxBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), Min, Max);
}

VPhysicsRotaryLimitJoint::VPhysicsRotaryLimitJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Min, float Max) : VPhysicsMinMaxBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, Min, Max);
}

void VPhysicsRotaryLimitJoint::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Min, float Max)
{
	type = ROTARYLIMIT;
	constraint = cpRotaryLimitJointNew(BodyA, BodyB, Min, Max);
}

VPhysicsRatchetJoint::VPhysicsRatchetJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float Phase, float Ratchet) : VPhysicsPhaseBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), Phase, Ratchet);
}

VPhysicsRatchetJoint::VPhysicsRatchetJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Phase, float Ratchet) : VPhysicsPhaseBase(BodyA, BodyB)
{
	Setup(BodyA, BodyB, Phase, Ratchet);
}

void VPhysicsRatchetJoint::Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Phase, float Ratchet)
{
	type = RATCHET;
	constraint = cpRatchetJointNew(BodyA, BodyB, Phase, Ratchet);
}

VPhysicsGearJoint::VPhysicsGearJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float Phase, float Ratio) : VPhysicsPhaseBase(ObjectA->GetBody(), ObjectB->GetBody())
{
	Setup(ObjectA->GetBody(), ObjectB->GetBody(), Phase, Ratio);
}

VPhysicsGearJoint::VPhysicsGearJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Phase, float Ratio) : VPhysicsPhaseBase(BodyA, BodyB)
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
