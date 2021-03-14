/**
* @file    VPhysicsGroup.h
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
* Group class that processes the VPhysicsObjects.
*/

#pragma once

#ifndef VFRAME_NO_PHYSICS
#include "VGroup.h"
#include "VPhysicsObject.h"
#include "VPhysicsJoints.h"

class VPhysicsArbiter
{
private:
	VPhysicsCPArbiter* arb;

public:
	VPhysicsArbiter(VPhysicsCPArbiter* arbiter);

	///@return The number of contacts between the two object, when in doubt assume there will be a maximum of two.
	int GetCount();

	///@return The friction coefficient calculated on PRESOLVE.
	float GetFriction();

	///@return The normal the collision between both objects.
	sf::Vector2f GetNormal();

	/**
	* @param contact The contact to test, max can be obtained from GetCount(), when in doubt assume there will be two contacts maximum.
	* @return The depth of the contact.
	*/
	float GetDepth(int contact);

	/**
	* @param contact The contact to test, max can be obtained from GetCount(), when in doubt assume there will be two contacts maximum.
	* @return The position of point A.
	*/
	sf::Vector2f GetPointA(int contact);

	/**
	* @param contact The contact to test, max can be obtained from GetCount(), when in doubt assume there will be two contacts maximum.
	* @return The position of point B.
	*/
	sf::Vector2f GetPointB(int contact);

	///@return The restitution value from the contact.
	float GetRestitution();

	///@return The surface velocity of the contact calculated at PRESOLVE.
	sf::Vector2f GetSurfaceVelocity();

	///@return Returns true if this collision will be ignored due to collision filters.
	bool Ignore();

	///@return Return true if this is the first contact between the two shapes.
	bool IsFirstContact();

	///@return Return true during SEPARATE if the type of callback was invoked due to an object removal.
	bool IsRemoval();

	///&return The combined impulse to resolve the collision.
	sf::Vector2f GetTotalImpulse();

	///@return The calculated energy loss in a collision including static, but not dynamic friction. This should only be called from POSTSOLVE.
	float GetTotalKE();

	///@param friction The new friction coefficient calculated on PRESOLVE.
	void SetFriction(float friction);

	///@param restitution The new restitution from the contact.
	void SetRestitution(float restitution);

	///@param velocity The new surface velocity of the contact calculated at PRESOLVE.
	void SetSurfaceVelocity(sf::Vector2f velocity);
};

///A special VGroup that sets up a physics world and updates VPhysicsObjects. It doesn't handle rendering except for debugging, nor does it update the VObject.
class VPhysicsGroup : public VGroup
{
private:
	float timestep = 0.0f;
	VPhysicsCPSpace* space;
	VPhysicsCollision* collisionHandler;
	
	struct VPhysicsObjectCollisionCallbackHelper;
	std::vector<VPhysicsObjectCollisionCallbackHelper> objectCallbackHelperList;

	VPhysicsObject* getObjectFromBody(cpBody* body);
	
	struct VPhysicsConstraintCallbackHelper;
	std::vector<VPhysicsConstraintCallbackHelper> constraintCallbackHelperList;
	VPhysicsJointBase* getJointFromConstraint(cpConstraint* constraint);

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VGroup VSUPERCLASS;

	///@param Amount The fixed length of the group. If 0, then the VGroup has no limit in size.
	VPhysicsGroup(unsigned int Amount = 0);

	/**
	* This functions adds a VPhysicsObject to the group. This should be used instead of the Add function unless you want to set up a VPhysicsObject yourself.
	* @param Object The game object you wish to apply physics to.
	* @param BodyType This sets what type of physics body the object should have, see VObjectType for which types.
	* @param Shape This sets what shape the collider should have.
	* @param Verts If Shape is set to either LINE or SHAPE, these set the points of the shape. These should be set relative to the top left of the shape.
	* @return VPhysicsObject created from this function, or nullptr if creation failed.
	*/
	VPhysicsObject* AddObject(VObject* Object, VPhysicsObject::VObjectType BodyType, VPhysicsObject::VObjectShape Shape = VPhysicsObject::BOX, std::vector<sf::Vector2f> Verts = {});

	/**
	* Removes the VPhysicsObject from the group based on it's attached VObject and destroys it, useful if you no longer have the VPhysicsObject at hand.
	* @param Object The game object that you wish to remove the physics from.
	*/
	bool RemoveObject(VObject* Object);

	/**
	* This functions adds a VPhysicsJointBase object to the physics world.
	* @param Joint The Physics Joint to add into the scene, it must be already set up with both physics objects.
	* @return VPhysicsJointBase created from this function, or nullptr if creation failed.
	*/
	VPhysicsJointBase* AddJoint(VPhysicsJointBase* Joint);

	/**
	* Removes the VPhysicsJointBase from the group and destroys it.
	* @param Joint The physics joint you wish to be removed.
	*/
	bool RemoveJoint(VPhysicsJointBase* Joint);
	
	///Destroys all VPhysicsObjects, this has no effect on the VObject that's attached to them.
	virtual void Destroy() override;

	/**
	* Updates all the VPhysicsObjects.
	* @param dt Delta Time
	*/
	virtual void Update(float dt) override;

	///@return The accuracy of the collision iterative solver.
	int GetIterations();
	///@param Iterations The accuracy of the collision iterative solver in number of iterations.
	void SetIterations(int Iterations);

	///@return The gravity set for both axis in pixels per second (e.g. sf::Vector(0.0f, 910.0f is down). Default is 0.
	sf::Vector2f GetGravity();
	///@param Gravity The gravity set for both axis in pixels per second (e.g. sf::Vector(0.0f, 910.0f is down).
	void SetGravity(sf::Vector2f Gravity);

	///@return The general damping applied when objects collide. Default is 1.
	float GetDamping();
	///@param Damping The general damping applied when objects collide
	void SetDamping(float Damping);

	///@return The speed threshold to determine when a physics object should be idle. Default value is 0.
	float GetIdleSpeedThreshold();
	///@param IdleSpeedThreshold The speed threshold to determine when a physics object should be idle.
	void SetIdleSpeedThreshold(float IdleSpeedThreshold);

	///@return The time threshold to determine when a physics object should sleep. Default value is 0.
	float GetSleepTimeThreshold();
	///@param SleepTimeThreshold The time threshold to determine when a physics object should sleep.
	void SetSleepTimeThreshold(float SleepTimeThreshold);

	///@return The amount of overlap between shapes that is allowed. Default is 0.1.
	float GetCollisionSlop();
	///@param CollisionSlop The amount of overlap between shapes that is allowed.
	void SetCollisionSlop(float CollisionSlop);

	///@return The bias percentage of overlap remaining unfixed after a second. Default is calculated as (1 - 0.1)^60
	float GetCollisionBias();
	///@param CollisionBias The bias percentage of overlap remaining unfixed after a second.
	void SetCollisionBias(float CollisionBias);

	///@return The number of frames/steps to keep collision solutions. Default is 3.
	unsigned int GetCollisionPersistence();
	///@param CollisionPersistence The number of frames/steps to keep collision solutions.
	void SetCollisionPersistence(unsigned int CollisionPersistence);

	///@return A pointer to the static body of the physics world, useful if you want to pin a physics object to the scene itself.
	VPhysicsCPBody* GetBody();

	/**
	* Helper function for getting the physics object using the base object.
	* @param object The base VObject to compare.
	* @return The physics object, NULL if no physics object could be found.
	*/
	VPhysicsObject* FindPhysicsObject(VObject* object);

	///The types of collision callback.
	enum VPhysicsCallbackType
	{
		///Two shapes have just touched. Callbacks of this type can return a false boolean value to ignore the collision.
		BEGIN,
		///The shapes are touching. Callbacks of this type can return a false boolean value to ignore the collision.
		PRESOLVE,
		///The shapes are touching and have been processed.
		POSTSOLVE,
		///Two shapes are no longer touching for the first time.
		SEPARATE,
	};

	/**
	* Collision callbacks act slightly differently from VCollision.
	* Here you can set the collision between specific objects, their callback, when the callback should be applied and whether you want the collision to be active forever or deactivated after being called once.
	* @param a The first object to test collisions with for the callback.
	* @param b The second object to test collisions with for the callback. Setting this to nullptr will apply the callback between object a and anything else.
	* @param callback The function to call if the collision takes place, the two parameters are VPhysicsObject so you can adjust the collision properties and access the VObject at the same time.
	* @param type The type of collision callback to set (see VPhysicsCallbackType).
	* @param persist If true, this callback will remain active for the duration of the space, otherwise this callback will be removed after one call. This way you aren't required to call it on each update.
	*/
	void SetCollisionCallback(VObject* a, VObject* b, const std::function<bool(VPhysicsObject*, VPhysicsObject*, VPhysicsArbiter*)>& callback, VPhysicsCallbackType type, bool persist = false);

	/**
	* Collision callbacks act slightly differently from VCollision.
	* Here you can set the collision between specific objects, their callback, when the callback should be applied and whether you want the collision to be active forever or deactivated after being called once.
	* @param a The first object to test collisions with for the callback.
	* @param b The second object to test collisions with for the callback. Setting this to nullptr will apply the callback between object a and anything else.
	* @param callback The function to call if the collision takes place, the two parameters are VPhysicsObject so you can adjust the collision properties and access the VObject at the same time.
	* @param type The type of collision callback to set (see VPhysicsCallbackType).
	* @param persist If true, this callback will remain active for the duration of the space, otherwise this callback will be removed after one call. This way you aren't required to call it on each update.
	*/
	void SetCollisionCallback(VPhysicsObject* a, VPhysicsObject* b, const std::function<bool(VPhysicsObject*, VPhysicsObject*, VPhysicsArbiter*)>& callback, VPhysicsCallbackType type, bool persist = false);

	/**
	* Here you can set callbacks for the pre and post solve conditions of a joint.
	* @param joint The joint to test the callback with.
	* @param callback The function to call if the joint is applied, the parameters are the VPhysicsJointBase and two VPhysicsObject so you can modify the joints or the objects.
	* @param type The type of callback to set (see VPhysicsCallbackType). BEGIN and SEPARATE are ignored.
	* @param persist If true, this callback will remain active for the duration of the space, otherwise this callback will be removed after one call. This way you aren't required to call it on each update.
	*/
	void SetConstraintCallback(VPhysicsJointBase* joint, const std::function<void(VPhysicsJointBase*, VPhysicsObject*, VPhysicsObject*)>& callback, VPhysicsCallbackType type, bool persist = false);

	/**
	* This function is called to process all the callbacks, this should only be overridden if you want to manipulate the collision response directly.
	* @param arb The collision arbiter, which essentially holds all the collision response infomation between two shapes.
	* @param space The Physics world the collision took place in (always the one created in VPhysicsGroup).
	* @param type The type of callback to process.
	* @return A boolean value for the begin and presolve callbacks.
	*/
	virtual bool ProcessCallback(VPhysicsCPArbiter* arb, VPhysicsCPSpace* space, VPhysicsCallbackType type);

	/**
	* This function is called to process all the callbacks, this should only be overridden if you want to manipulate the collision response directly.
	* @param constraint The collision arbiter, which essentially holds all the collision response infomation between two shapes.
	* @param space The Physics world the collision took place in (always the one created in VPhysicsGroup).
	* @param type The type of callback to process.
	*/
	virtual void ProcessCallback(VPhysicsCPConstraint* constraint, VPhysicsCPSpace* space, VPhysicsCallbackType type);

#ifdef _DEBUG
	/**
	* Draw the debug visual aides for the physics objects.
	@param
	*/
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
#endif

};
#endif
