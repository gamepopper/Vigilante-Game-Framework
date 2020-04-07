/**
* @file    VPhysicsJoints.h
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
* Objects that represent joints/constrants in physics objects.
*/

#pragma once
#ifndef VFRAME_NO_PHYSICS
#include "VBase.h"
#include "VPhysicsObject.h"

///Accessor to cpConstraint struct.
typedef struct cpConstraint VPhysicsCPConstraint;

///The base class for all types of physics joints. The class itself does not set up the joint itself, but makes it possible to add to the physics world and set properties of one.
class VPhysicsJointBase : public VBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VBase VSUPERCLASS;

	///Default constructor.
	VPhysicsJointBase() = default;
	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	*/
	VPhysicsJointBase(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB);

	virtual void Destroy() override;

	/**
	* Initialises the physics joint.
	* @param space The physics space to initialise the joint to.
	*/
	void Initialise(VPhysicsCPSpace* space);

	/**
	* Deinitises and removes the physics joint from the world.
	* @param space The physics space to remove the joint from.
	*/
	void Deinitialise(VPhysicsCPSpace* space);

	///@return The constraint that makes up the joint.
	VPhysicsCPConstraint* GetConstraint();
	///@return The first body in the joint.
	VPhysicsCPBody* GetBodyA();
	///@return The second body in the joint.
	VPhysicsCPBody* GetBodyB();

	///@param Max The maximum force that the constraint can use to act on the two bodies. Defaults to INFINITY.
	void SetMaxForce(float Max);
	///@return The maximum force that the constraint can use to act on the two bodies. Defaults to INFINITY.
	float GetMaxForce();

	///@param Bias The percentage of joint error that remains unfixed after a second. This works exactly the same as the collision bias property of a space, but applies to fixing error (stretching) of joints instead of overlapping collisions.
	void SetErrorBias(float Bias);
	///@return The percentage of joint error that remains unfixed after a second. This works exactly the same as the collision bias property of a space, but applies to fixing error (stretching) of joints instead of overlapping collisions.
	float GetErrorBias();

	///@param Bias The maximum speed at which the constraint can apply error correction. Defaults to INFINITY.
	void SetMaxBias(float Bias);
	///@return The maximum speed at which the constraint can apply error correction. Defaults to INFINITY.
	float GetMaxBias();

	///@param value When two bodies collide, Chipmunk ignores the collisions if this property is set to false on any constraint that connects the two bodies. Defaults to true. This can be used to create a chain that self collides, but adjacent links in the chain do not collide.
	void SetCollideBodies(bool value);
	///@return When two bodies collide, Chipmunk ignores the collisions if this property is set to false on any constraint that connects the two bodies. Defaults to true. This can be used to create a chain that self collides, but adjacent links in the chain do not collide.
	bool DoesCollideBodies();
	///@return The most recent impluse that has been applied. Divide by the timestep to get the force.
	float GetImpulse();

protected:
	///The types of Physics joints that can be set.
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

	///The Type of joint the object is. Used for Get/Set function calls.
	unsigned int type = -1;
	///The constraint object.
	VPhysicsCPConstraint* constraint = NULL;

private:
	VPhysicsCPBody* bodyA = NULL;
	VPhysicsCPBody* bodyB = NULL;
};

///A Physics Joint Base class that provideds functions for getting and setting Anchor points. Used in Pin, Slide, Pivot or Damped type.
class VPhysicsAnchorBase : public VPhysicsJointBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsJointBase VSUPERCLASS;

	///Default constructor.
	VPhysicsAnchorBase() = default;
	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	*/
	VPhysicsAnchorBase(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB);

	///@return The anchor point of body A.
	sf::Vector2f GetAnchorA();
	/**
	* Sets the anchor point of body A in world space.
	* @param value The new Anchor A value.
	*/
	void SetAnchorA(sf::Vector2f value);

	///@return The anchor point of body B.
	sf::Vector2f GetAnchorB();
	/**
	* Sets the anchor point of body B in world space.
	* @param value The new Anchor B value.
	*/
	void SetAnchorB(sf::Vector2f value);
};

///A Physics Joint Base class that provideds functions for getting and setting Minimum and Maximum value functions. Used in Slide and RotaryLimit Joints.
class VPhysicsMinMaxBase : public VPhysicsAnchorBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsAnchorBase VSUPERCLASS;

	///Default constructor.
	VPhysicsMinMaxBase() = default;
	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	*/
	VPhysicsMinMaxBase(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB);

	///@return The minimum value.
	float GetMin();
	/**
	* Sets the minimum value for Slide and RotaryLimit Joints.
	* @param value The new minimum value.
	*/
	void SetMin(float value);

	///@return The maximum value for Slide and RotaryLimit Joints.
	float GetMax();
	/**
	* Sets the maximum value for Slide and RotaryLimit Joints.
	* @param value The new maximum value.
	*/
	void SetMax(float value);
};

///A Physics Joint Base class that provideds functions for getting and setting values related to Damped Spring and Damped Rotary Spring properties.
class VPhysicsDampedBase : public VPhysicsAnchorBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsAnchorBase VSUPERCLASS;

	///Default constructor.
	VPhysicsDampedBase() = default;
	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	*/
	VPhysicsDampedBase(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB);

	///&return The stiffness value of both damped joints.
	float GetStiffness();
	/**
	* Sets the stiffness of damped points.
	* @param value The new stiffness value.
	*/
	void SetStiffness(float value);

	///&return The damping value of both damped joints.
	float GetDamping();
	/**
	* Sets the damping of damped points.
	* @param value The new damping value.
	*/
	void SetDamping(float value);
};

///A Physics Joint Base class that provideds functions for getting and setting the Phase value. Used in Ratchet and Gear Joints.
class VPhysicsPhaseBase : public VPhysicsJointBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsJointBase VSUPERCLASS;

	///Default constructor.
	VPhysicsPhaseBase() = default;
	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	*/
	VPhysicsPhaseBase(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB);

	///&return The phase value of Ratchet and Gear joints.
	float GetPhase();
	/**
	* Sets the phase value of Ratchet and Gear joints.
	* @param value The new phase value.
	*/
	void SetPhase(float value);
};

///The Pin Joint, where bodies are linked like they are attached a bar or pin.
class VPhysicsPinJoint : public VPhysicsAnchorBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsAnchorBase VSUPERCLASS;

	/**
	* The distance between the two anchor points is measured when the joint is created. If you want to set a specific distance, use the setter function to override it.
	* @param ObjectA The first physics object to apply the joint to its body.
	* @param ObjectB The second physics object to apply the joint to its body.
	* @param AnchorA The anchor point of the first body.
	* @param AnchorB The anchor point of the second body.
	*/
	VPhysicsPinJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f AnchorA, sf::Vector2f AnchorB);

	/**
	* The distance between the two anchor points is measured when the joint is created. If you want to set a specific distance, use the setter function to override it.
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	* @param AnchorA The anchor point of the first body.
	* @param AnchorB The anchor point of the second body.
	*/
	VPhysicsPinJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB);

	///@return The distance between the two bodies set in the Pin Joint, returns 0 on other joint types.
	float GetDist();
	/**
	* Sets the distance between the two bodies in a Pin Joint, it's calculated automatically on initialisation but can be called here to set manually.
	* @param value The new distance value.
	*/
	void SetDist(float value);

private:
	void Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB);
};

///The SLide Joint, similar to Pin Joints but there is a min/max distance as opposed to a single fixed distance. Could be used for approzimate ropes.
class VPhysicsSlideJoint : public VPhysicsMinMaxBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsMinMaxBase VSUPERCLASS;

	/**
	* @param ObjectA The first physics object to apply the joint to its body.
	* @param ObjectB The second physics object to apply the joint to its body.
	* @param AnchorA The anchor point of the first body.
	* @param AnchorB The anchor point of the second body.
	* @param Min The minimum allowed distance between the two bodies.
	* @param Max The maximum allowed distance between the two bodies.
	*/
	VPhysicsSlideJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float Min, float Max);

	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	* @param AnchorA The anchor point of the first body.
	* @param AnchorB The anchor point of the second body.
	* @param Min The minimum allowed distance between the two bodies.
	* @param Max The maximum allowed distance between the two bodies.
	*/
	VPhysicsSlideJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float Min, float Max);

private:
	void Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float Min, float Max);
};

///The Pivot Joint, holds two anchors together like a swivel.
class VPhysicsPivotJoint : public VPhysicsAnchorBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsAnchorBase VSUPERCLASS;

	/**
	* @param ObjectA The first physics object to apply the joint to its body.
	* @param ObjectB The second physics object to apply the joint to its body.
	* @param Pivot Tthe point in world coordinates of the pivot. Because the pivot location is given in world coordinates, you must have the bodies moved into the correct positions already. Alternatively you can specify the joint based on a pair of anchor points, but make sure you have the bodies in the right place as the joint will fix itself as soon as you start simulating the space.
	*/
	VPhysicsPivotJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f Pivot);

	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	* @param Pivot Tthe point in world coordinates of the pivot. Because the pivot location is given in world coordinates, you must have the bodies moved into the correct positions already. Alternatively you can specify the joint based on a pair of anchor points, but make sure you have the bodies in the right place as the joint will fix itself as soon as you start simulating the space.
	*/
	VPhysicsPivotJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f Pivot);

private:
	void Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f Pivot);
};

///The Groove Joint, similar to pivot joints but the B anchor is a line that the pivot can slide.
class VPhysicsGrooveJoint : public VPhysicsJointBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsJointBase VSUPERCLASS;

	/**
	* @param ObjectA The first physics object to apply the joint to its body.
	* @param ObjectB The second physics object to apply the joint to its body.
	* @param GrooveA Point A of the groove, local to the first body.
	* @param GrooveB Point B of the groove, local to the first body.
	* @param AnchorB The location of the pivot attached to the second body, local to said body.
	*/
	VPhysicsGrooveJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f GrooveA, sf::Vector2f GrooveB, sf::Vector2f AnchorB);

	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	* @param GrooveA Point A of the groove, local to the first body.
	* @param GrooveB Point B of the groove, local to the first body.
	* @param AnchorB The location of the pivot attached to the second body, local to said body.
	*/
	VPhysicsGrooveJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f GrooveA, sf::Vector2f GrooveB, sf::Vector2f AnchorB);

	///@return The groove value of body A. Returns zero if joint is not a Groove type.
	sf::Vector2f GetGrooveA();
	/**
	* Sets the groove value of body A in world space. Ignored if joint is not a Groove type.
	* @param value The new Groove A value.
	*/
	void SetGrooveA(sf::Vector2f value);

	///@return The groove value of body B. Returns zero if joint is not a Groove type.
	sf::Vector2f GetGrooveB();
	/**
	* Sets the groove value of body B in world space. Ignored if joint is not a Groove type.
	* @param value The new Groove B value.
	*/
	void SetGrooveB(sf::Vector2f value);

private:
	void Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f GrooveA, sf::Vector2f GrooveB, sf::Vector2f AnchorB);
};

///Damped Springs
class VPhysicsDampedSpring : public VPhysicsDampedBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsDampedBase VSUPERCLASS;

	/**
	* @param ObjectA The first physics object to apply the joint to its body.
	* @param ObjectB The second physics object to apply the joint to its body.
	* @param AnchorA The anchor point of the first body.
	* @param AnchorB The anchor point of the second body.
	* @param RestLength The distance the spring wants to be at rest.
	* @param Stiffness The stiffness of the spring constant based on Young's Modulous.
	* @param Damping How soft to make the damping of the spring.
	*/
	VPhysicsDampedSpring(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float RestLength, float Stiffness, float Damping);

	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	* @param AnchorA The anchor point of the first body.
	* @param AnchorB The anchor point of the second body.
	* @param RestLength The distance the spring wants to be at rest.
	* @param Stiffness The stiffness of the spring constant based on Young's Modulous.
	* @param Damping How soft to make the damping of the spring.
	*/
	VPhysicsDampedSpring(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float RestLength, float Stiffness, float Damping);

	///&return The rest length for Damped Spring Joints.
	float GetRestLength();
	/**
	* Sets the rest length for Damped Spring Joints.
	* @param value The new rest length value.
	*/
	void SetRestLength(float value);

private:
	void Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, sf::Vector2f AnchorA, sf::Vector2f AnchorB, float RestLength, float Stiffness, float Damping);
};

///Damped Rotary Springs, similar to Damped Springs but angular.
class VPhysicsDampedRotarySpring : public VPhysicsDampedBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsDampedBase VSUPERCLASS;

	/**
	* @param ObjectA The first physics object to apply the joint to its body.
	* @param ObjectB The second physics object to apply the joint to its body.
	* @param RestAngle The relatvie angle in radians that the bodies want to have.
	* @param Stiffness The stiffness of the spring constant based on Young's Modulous.
	* @param Damping How soft to make the damping of the spring.
	*/
	VPhysicsDampedRotarySpring(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float RestAngle, float Stiffness, float Damping);

	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	* @param RestAngle The relatvie angle in radians that the bodies want to have.
	* @param Stiffness The stiffness of the spring constant based on Young's Modulous.
	* @param Damping How soft to make the damping of the spring.
	*/
	VPhysicsDampedRotarySpring(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float RestAngle, float Stiffness, float Damping);

	///&return The rest angle for Damped Rotary Spring Joints.
	float GetRestAngle();
	/**
	* Sets the rest angle for Damped Rotary Spring Joints.
	* @param value The new rest angle value.
	*/
	void SetRestAngle(float value);

private:
	void Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float RestLength, float Stiffness, float Damping);
};

///The Rotary Limit Joint
class VPhysicsRotaryLimitJoint : public VPhysicsMinMaxBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsMinMaxBase VSUPERCLASS;

	/**
	* @param ObjectA The first physics object to apply the joint to its body.
	* @param ObjectB The second physics object to apply the joint to its body.
	* @param Min The minimum angle in radians.
	* @param Max The maximum angle in radians.
	*/
	VPhysicsRotaryLimitJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float Min, float Max);

	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	* @param Min The minimum angle in radians.
	* @param Max The maximum angle in radians.
	*/
	VPhysicsRotaryLimitJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Min, float Max);

private:
	void Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Min, float Max);
};

///The Ratchet Joint, a rotary ratchet similar to a socket wrench.
class VPhysicsRatchetJoint : public VPhysicsPhaseBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsPhaseBase VSUPERCLASS;

	/**
	* @param ObjectA The first physics object to apply the joint to its body.
	* @param ObjectB The second physics object to apply the joint to its body.
	* @param Phase The initial offset to use when deciding where the ratchet angles are.
	* @param Ratchet The distance between “clicks”.
	*/
	VPhysicsRatchetJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float Phase, float Ratchet);

	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	* @param Phase The initial offset to use when deciding where the ratchet angles are.
	* @param Ratchet The distance between “clicks”.
	*/
	VPhysicsRatchetJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Phase, float Ratchet);

	///&return The angle value of Ratchet joint.
	float GetAngle();
	/**
	* Sets the angle value of Ratchet joint.
	* @param value The new angle value.
	*/
	void SetAngle(float value);

	///&return The ratchet (distance between clicks) value of Ratchet joints.
	float GetRatchet();
	/**
	* Sets the ratchet (distance between clicks) value of Ratchet joints.
	* @param value The new ratchet value.
	*/
	void SetRatchet(float value);

private:
	void Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Phase, float Ratchet);
};

///The Gear Joint, Maintain a specific angular velocity ratio between bodies.
class VPhysicsGearJoint : public VPhysicsPhaseBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsPhaseBase VSUPERCLASS;

	/**
	* @param ObjectA The first physics object to apply the joint to its body.
	* @param ObjectB The second physics object to apply the joint to its body.
	* @param Phase The initial angular offset of the two bodies.
	* @param Ratio The angular velocity ratio, always measured in absolute terms.
	*/
	VPhysicsGearJoint(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float Phase, float Ratio);

	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	* @param Phase The initial angular offset of the two bodies.
	* @param Ratio The angular velocity ratio, always measured in absolute terms.
	*/
	VPhysicsGearJoint(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Phase, float Ratio);

	///&return The ratio value of Gear joints.
	float GetRatio();
	/**
	* Sets the ratio value of Gear joints.
	* @param value The new ratio value.
	*/
	void SetRatio(float value);

private:
	void Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Phase, float Ratio);
};

///The Simple Motor, Maintain a specific angular relative velocity between bodies.
class VPhysicsSimpleMotor : public VPhysicsJointBase
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VPhysicsJointBase VSUPERCLASS;

	/**
	* @param ObjectA The first physics object to apply the joint to its body.
	* @param ObjectB The second physics object to apply the joint to its body.
	* @param Rate The desired relative angular velocity.
	*/
	VPhysicsSimpleMotor(VPhysicsObject* ObjectA, VPhysicsObject* ObjectB, float Rate);

	/**
	* @param BodyA The first physics body to apply the joint to.
	* @param BodyB The second physics body to apply the joint to.
	* @param Rate The desired relative angular velocity.
	*/
	VPhysicsSimpleMotor(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Rate);

	///&return The simple motor rate value of Simple Motor joints.
	float GetRate();
	/**
	* Sets the simple motor rate value of Simple Motor joints.
	* @param value The new rate value.
	*/
	void SetRate(float value);

private:
	struct VPhysicsConstraintCallbackHelper;
	void Setup(VPhysicsCPBody* BodyA, VPhysicsCPBody* BodyB, float Rate);
};

#endif