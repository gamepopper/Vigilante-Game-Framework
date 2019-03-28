/**
* @file    VVPhysicsGroup.h
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
* Group class that processes the VPhysicsObjects.
*/

#pragma once

#ifndef VFRAME_NO_PHYSICS
#include "VGroup.h"
#include "VPhysicsObject.h"

///A simple struct to represent the arbiter, a pair of colliding shapes.
struct VPhysicsArbiter
{
	sf::Vector2f PointA;
	sf::Vector2f PointB;
};

struct CollisionCallbackHelper;

///A special VGroup that sets up a physics world and updates VPhysicsObjects. It doesn't handle rendering except for debugging, nor does it update the VObject.
class VPhysicsGroup : public VGroup
{
private:
	float timestep = 0.0f;

protected:
	cpSpace* space;
	cpCollisionHandler* collisionHandler;
	
	std::vector<CollisionCallbackHelper> callbackHelperList;

	VPhysicsObject* getObjectFromBody(cpBody* body);

public:
	typedef VGroup VSUPERCLASS;

	///@param Amount The fixed length of the group. If 0, then the VGroup has no limit in size.
	VPhysicsGroup(unsigned int Amount = 0);
	
	/**
	* This functions adds an VPhysicsObject to the group. This should be used instead of the Add function unless you want to set up a VPhysicsObject yourself.
	* @param Object The game object you wish to apply physics to.
	* @param BodyType This sets what type of physics body the object should have, see VObjectType for which types.
	* @param Shape This sets what shape the collider should have.
	* @param Verts If Shape is set to either LINE or SHAPE, these set the points of the shape. These should be set relative to the top left of the shape.
	* @return VPhysicsObject created from this function, or nullptr if creation failed.
	*/
	VPhysicsObject* AddObject(VObject* Object, VPhysicsObject::VObjectType BodyType, VPhysicsObject::VObjectShape Shape = VPhysicsObject::BOX, std::vector<sf::Vector2f> Verts = {});
	
	/**
	* Removes the VPhysicsObject from the group based on it's attached VObject, useful if you no longer have the VPhysicsObject at hand.
	* @param Object The game object you wish to apply physics to.
	*/
	bool RemoveObject(VObject* Object);
	
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
	///@params The number of frames/steps to keep collision solutions.
	void SetCollisionPersistence(unsigned int CollisionPersistence);

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
	* @param The type of collision callback to set (see VPhysicsCallbackType).
	* @param persist If true, this callback will remain active for the duration of the space, otherwise this callback will be removed after one call. This way you aren't required to call it on each update.
	*/
	void SetCollisionCallback(VObject* a, VObject* b, const std::function<bool(VPhysicsObject*, VPhysicsObject*)>& callback, VPhysicsCallbackType type, bool persist = false);

	/**
	* Collision callbacks act slightly differently from VCollision.
	* Here you can set the collision between specific objects, their callback, when the callback should be applied and whether you want the collision to be active forever or deactivated after being called once.
	* @param a The first object to test collisions with for the callback.
	* @param b The second object to test collisions with for the callback. Setting this to nullptr will apply the callback between object a and anything else.
	* @param callback The function to call if the collision takes place, the two parameters are VPhysicsObject so you can adjust the collision properties and access the VObject at the same time.
	* @param The type of collision callback to set (see VPhysicsCallbackType).
	* @param persist If true, this callback will remain active for the duration of the space, otherwise this callback will be removed after one call. This way you aren't required to call it on each update.
	*/
	void SetCollisionCallback(VPhysicsObject* a, VPhysicsObject* b, const std::function<bool(VPhysicsObject*, VPhysicsObject*)>& callback, VPhysicsCallbackType type, bool persist = false);

	/**
	* This function is called to process all the callbacks, this should only be overridden if you want to manipulate the collision response directly.
	* @param arb The collision arbiter, which essentially holds all the collision response infomation between two shapes.
	* @param space The Physics world the collision took place in (always the one created in VPhysicsGroup).
	* @param type The type of callback to process.
	* @return A boolean value for the begin and presolve callbacks.
	*/
	bool ProcessCallback(cpArbiter *arb, cpSpace *space, VPhysicsCallbackType type);

#ifdef _DEBUG
	/**
	* Draw the debug visual aides for the physics objects.
	@param
	*/
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
#endif

};
#endif
