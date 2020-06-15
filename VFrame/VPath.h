/**
* @file    VPath.h
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
* Moving an object around a path defined by points.
*/

#pragma once
#include <vector>
#include <functional>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "VObject.h"
#include "VInterpolate.h"

///The path setting class.
class VPath
{
private:
	std::vector<sf::Vector2f> points;
	std::vector<sf::Vector2f> tmp;
	unsigned int pathType = 0;
	VObject* object = NULL;
	sf::Vector2f lastPos;
	sf::VertexArray vertexArray;
	VInterpolate::VInterpolateType interpolationType = VInterpolate::Linear;

	bool active = false;
	bool angle = false;
	bool dirty = false;
	float t = 0.0f;
	float speed = 1.0f;
	std::function<void()> onComplete;

	sf::Vector2f getBezierPoint(float t);

public:
	/// The types of paths to use.
	enum VPathType
	{
		///Follows a straight line between each point.
		LINE,
		///Follows a curved path based on all defined points.
		CURVE,
		NUMPATHTYPES
	};

	VPath() = default;
	~VPath() = default;

	/**
	* Sets a path based on a list of points.
	* @param points The list of points to set the path. Path follows the points in order of the list.
	*/
	void SetPath(std::vector<sf::Vector2f> points);

	/**
	* Add a point to the end of the path.
	* @param point The point to add to the end of the path.
	*/
	void AddPoint(sf::Vector2f point);

	/**
	* Remove a point from the path.
	* @param index The element in the list of points to remove from the path, has to be within range 0 to (size-1).
	*/
	void RemovePoint(unsigned int index);
	
	///Ckears the entire list of points, effectively removing the path.
	void ClearPath();

	///@return The number of points currently in the path.
	int GetNumPoints();

	///@return True if the current path is being actively used by an object.
	bool IsActive();

	/**
	* Sets the type of path. See VPathType for reference.
	* @param pathType The type of path to set.
	*/
	void SetPathType(VPathType pathType);

	///@return The current path type.
	VPathType GetPathType();

	/**
	* Sets the speed of the current path, defined as 1 / time. Default value is 1, which assumes the object will complete the path in one second. 0.5 = 2 seconds. 2 = 0.5 seconds, etc.
	* @param speed The new speed of the path following.
	*/
	void SetSpeed(float speed);

	///@return The current speed of following the path.
	float GetSpeed();

	/**
	* Sets whether the object following the path should update its angle. Assumes that the object is facing right when angle is 0.
	* @param active Whether updating the angle is active or not.
	*/
	void SetUpdateAngle(bool active);

	/**
	* Sets the kind of interpolation to use for the object following the path, uses the VInterpolate class. The default is Linear.
	* @param type The type of interpolation to use.
	*/
	void SetInterpolationType(VInterpolate::VInterpolateType type);

	///@return The current type of interpolation function being used for the object following the path.
	VInterpolate::VInterpolateType GetInterpolationType();

	/**
	* Activates the path to be followed. It also resets the path to the beginning of each call.
	* @param object The game object that should follow the path. This can be NULL.
	* @param onComplete Function that calls once the object has completed travelling along the path.
	*/
	void StartFollowing(VObject* object, std::function<void()> onComplete);

	///Stops the object from following the path. Good for any sudden events.
	void StopFollowing();

	/**
	* Gets an exact point in the entire path from a single point in time.
	* @param t The position in the point as a point in time. T has to be between 0 and 1.
	*/
	sf::Vector2f GetPoint(float t);

	/**
	* Updates the Path following.
	* @param dt Delta Time between the previous and current frame.
	*/
	void Update(float dt);

	/**
	* Render the path (useful for debugging purposes).
	* @param RenderTarget The Render Target to render the path towards.
	*/
	void Draw(sf::RenderTarget& RenderTarget);
};

