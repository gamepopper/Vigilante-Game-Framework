/**
* @file    VObject.h
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
* Base class for Objects, extended from VBase to add transformation properties.
*/

#pragma once
#include "VBase.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

///Base object class that handles movement, rotation and collisions.
class VObject : public VBase
{
protected:
	/**
	* Updates the position and rotation of object.
	* By default all velocity values will be set using the computeVelocity function + velocity * delta time.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void updateMotion(float dt);
	///Update the tranform.
	virtual void updateTransform() {};

private:
	/**
	* Checks the amount of overlap between two objects in x axis.
	* @param a Left Object.
	* @param b Right Object.
	* @param maxOverlap If true will only return overlap amount if it doesn't exceed the maximum overlap distance.
	* @return Amount of pixels of overlap between both objects.
	*/
	static float overlapX(VObject* a, VObject *b, bool maxOverlap = true);

	/**
	* Checks the amount of overlap between two objects in y axis.
	* @param a Left Object.
	* @param b Right Object.
	* @param maxOverlap If true will only return overlap amount if it doesn't exceed the maximum overlap distance.
	* @return Amount of pixels of overlap between both objects.
	*/
	static float overlapY(VObject* a, VObject *b, bool maxOverlap = true);

	/**
	* Checks the amount of overlap between two objects.
	* @param a Left Object.
	* @param b Right Object.
	* @param maxOverlap If true will only return overlap amount if it doesn't exceed the maximum overlap distance.
	* @return Amount of pixels of overlap between both objects.
	*/
	static sf::Vector2f overlapCircle(VObject* a, VObject *b, bool maxOverlap = true);

	/**
	* Checks for an overlap and separates both objects by the amount of overlap in the x axis.
	* @param a Left Object.
	* @param b Right Object.
	* @return True if there is an overlap and separation.
	*/
	static bool separateX(VObject* a, VObject *b);

	/**
	* Checks for an overlap and separates both objects by the amount of overlap in the y axis.
	* @param a Left Object.
	* @param b Right Object.
	* @return True if there is an overlap and separation.
	*/
	static bool separateY(VObject* a, VObject *b);

	/**
	* Checks for an overlap and updates the Touching flag in the x axis.
	* @param a Left Object.
	* @param b Right Object.
	* @return True if there is an overlap.
	*/
	static bool touchFlagX(VObject* a, VObject *b);

	/**
	* Checks for an overlap and updates the Touching flag in the y axis.
	* @param a Left Object.
	* @param b Right Object.
	* @return True if there is an overlap.
	*/
	static bool touchFlagY(VObject* a, VObject *b);

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VBase VSUPERCLASS;
	///Position of the object
	sf::Vector2f Position;
	///Size of the object.
	sf::Vector2f Size;
	///Position of the object on the previous frame.
	sf::Vector2f Last;
	///Radius amount for Circle Collisions.
	float Radius = 0;
	///Angle of object.
	float Angle = 0;
	///Velocity of rotation.
	float AngleVelocity = 0;
	///Acceleration of rotation.
	float AngleAcceleration = 0;
	///Drag of rotation.
	float AngleDrag = 0;
	///Maximum Velocity of rotation.
	float AngleMax = 10000;

	///If true, updates the objects position and angle.
	bool Moves = true;
	///Velocity of movement.
	sf::Vector2f Velocity;
	///Acceleration of movement.
	sf::Vector2f Acceleration;
	///Drag of movement.
	sf::Vector2f Drag;
	///Maximum velocity of movement.
	sf::Vector2f MaxVelocity;
	///The amount the object will appear to move when the camera scrolls (1.0f = Scrolls as fast as the camera, 0.5f = Scrolls half as fast as the camera). Good for parallax scrolling.
	sf::Vector2f ScrollFactor;
	///The amount the object will appear to rotate when the camera rotates (1.0f = Rotates as fast as the camera, 0.5f = Rotates half as fast as the camera). Good for parallax scrolling.
	float RotateFactor = 1;
	///The amount the object will appear to scale when the camera zooms (1.0f = Scales as fast as the camera, 0.5f = Scales half as fast as the camera). Good for parallax scrolling.
	float ZoomFactor = 1;
	///Bounciness of the object (0 assumes no bounce)
	float Elasticity = 0;
	///Mass of object in comparison to other objects in world.
	float Mass = 1;
	///Standard health value (object dies if health is <= 0)
	float Health = 100;
	///If true, the object will not change its velocity on collision.
	bool Immovable = false;

	///Flags for which sides are allowed to check if being overlapped for collision purposes.
	enum SidesTouching : unsigned char
	{
		TOUCHNONE = 0,
		TOUCHLEFT = 1 << 0,
		TOUCHRIGHT = 1 << 1,
		TOUCHTOP = 1 << 2,
		TOUCHBOTTOM = 1 << 3,
		TOUCHWALL = TOUCHLEFT | TOUCHRIGHT,
		TOUCHALL = TOUCHLEFT | TOUCHRIGHT | TOUCHTOP | TOUCHBOTTOM
	};

	///Which sides are currently touching (is reset on each Update call!)
	unsigned char Touching = SidesTouching::TOUCHNONE;
	///Which sides on previous frame were touching (is reset on each Update call!)
	unsigned char WasTouching = SidesTouching::TOUCHNONE;

	///Which sides are allowed to check collisions for.
	unsigned char AllowCollisions = SidesTouching::TOUCHALL;

	///Useful for moving platforms.
	bool CollisionXDrag = true;

#if _DEBUG
	///Colour to display the collision boundary.
	sf::Color DebugColor;
#endif

	///Adds to the maximum overlap distance between objects.
	static float SeparateBias;

	/**
	* @param position Position of the sprite.
	* @param size Size of the sprite (also sets the size of the render area).
	*/
	VObject(sf::Vector2f position, sf::Vector2f size = sf::Vector2f()) : VBase()
	{
		Position = position;
		Size = size;
		MaxVelocity = sf::Vector2f(10000, 10000);
		ScrollFactor = sf::Vector2f(1, 1);
		RotateFactor = 1;
		ZoomFactor = 1;
		type = VType::OBJECT;
		Last = Position;

		Radius = Size.x < Size.y ? Size.x / 2 : Size.y / 2;
	}

	/**
	* @param x X position of the sprite.
	* @param y Y position of the sprite.
	* @param width Width of the sprite (also sets the width of the render area).
	* @param height Height of the sprite (also sets the height of the render area).
	*/
	VObject(float x = 0, float y = 0, float width = 0, float height = 0) : VBase()
	{
		Position = sf::Vector2f(x, y);
		Size = sf::Vector2f(width, height);
		MaxVelocity = sf::Vector2f(10000, 10000);
		ScrollFactor = sf::Vector2f(1, 1);
		RotateFactor = 1;
		ZoomFactor = 1;
		type = VType::OBJECT;
		Last = Position;

		Radius = Size.x < Size.y ? Size.x / 2 : Size.y / 2;
	}

	/**
	* Separate objects that overlap based on rectangle collision.
	* @param a Left Object.
	* @param b Right Object.
	* @return True if there is an overlap and separation.
	*/
	static bool separate(VObject* a, VObject *b);

	/**
	* Separate objects that overlap based on circle collision.
	* @param a Left Object.
	* @param b Right Object.
	* @return True if there is an overlap and separation.
	*/
	static bool separateCircle(VObject* a, VObject *b);

	/**
	* Process touch test.
	* @param a Left Object.
	* @param b Right Object.
	* @return True if there is an overlap.
	*/
	static bool touchFlag(VObject* a, VObject *b);

	/**
	* Base function for calculating velocity.
	* If the acceleration parameter doesn't equal zero, then the function uses V = iV + A*dt.
	* Otherwise it will apply drag to the velocity.
	* The function will also cap the resulting velocity to whatever the max is, assuming max is greater than 0.
	* @param v Velocity value.
	* @param a Acceleration value.
	* @param d Drag value.
	* @param max Max Velocity value.
	* @param dt Delta Time between the previous and current frame.
	* @return The resulting velocity.
	*/
	static float computeVelocity(float v, float a, float d, float max, float dt);

	/**
	* Function used to for testing if VObject is in view of the camera, useful to test if a VObject should be rendered if in view of the a Camera.
	* @param renderTargetView The sf::View to test the VObject with. You can get this from a VCamera or sf::RenderTarget in the Draw function.
	* @param defaultView A modifiable sf::View that has the same size as the renderTargetView, but is transformed to factor in parallax (e.g. ScrollFactor, RotateFactor and ZoomFactor). This property is modified in the function, but the size and position should be default values.
	* @param o The VObject to test with, must not be NULL.
	* @param renderBox The render area of the object. Default is the Position and Size of the VObject, but this parameter can use whatever custom one is provided.
	* @return If the VObject is within the rectangle of the view (factoring in ScrollFactor, RotateFactor and ZoomFactor), then returns true.
	*/
	static bool TestInView(const sf::View& renderTargetView, sf::View defaultView, VObject* o, const sf::FloatRect& renderBox = sf::FloatRect());

	/**
	* Sets object position based on the centre of it's circular area.
	* @param x New X position of the object at it's centre.
	* @param y New Y position of the object at it's centre.
	*/
	void SetPositionAtCentre(float x, float y);

	/**
	* Sets object position based on the centre of it's circular area.
	* @param newPos New position of the object at it's centre.
	*/
	void SetPositionAtCentre(sf::Vector2f newPos);

	///@return True if collision is allowed on object on any side.
	bool IsSolid();

	/**
	* Resets object with new position.
	* @param newPos New position of the object.
	*/
	virtual void Reset(sf::Vector2f newPos);

	/**
	* Resets object with new position.
	* @param x New X position of the object.
	* @param y New Y position of the object.
	*/
	virtual void Reset(float x, float y);

	/**
	* Reduces health and kills object if health is below 0.
	* @param damage The value to remove from health.
	*/
	virtual void Hurt(float damage);

	/**
	* Updates object and any data it contains.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt);

	/**
	* Draws object.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget &RenderTarget);
};
