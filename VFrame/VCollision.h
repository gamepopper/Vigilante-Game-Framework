/**
* @file    VCollision.h
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
* Collision Handler for Games using a Quad Tree Collision System.
*/

#pragma once
#include "VObject.h"

#include <vector>
#include <functional>
#include <memory>

///Determines whether the object testing is the left or right list.
enum VCollideList : bool
{
	A,
	B
};

///Separates the entire world into four sections, and only test collisions of objects within each section. This reduces the amount of collision tests, but requires WorldBounds from VGlobal to be set to the correct world size.
struct VQuadTree
{
	///The size reserved for each collision list in the subsections.
	std::vector<VObject*> listA;
	///The List of the Right Side in Collision Testing.
	std::vector<VObject*> listB;
	///The boundary of the collision area.
	sf::FloatRect bounds;

	/**
	* Tests if an object is within a the boundary area to add to either of its two lists.
	* @param object The VObject to test.
	* @param list Which list to add to if the object is within the bounds.
	* @return If the object is within the bounds, then the function returns true and adds to the specified list.
	*/
	bool checkBounds(VObject* object, VCollideList list);

	///Clears out both lists.
	void clear();
};

///Specifies the amount of recursions for subsections (e.g 2 means 4 ^ 2 (16) subsections will be created in total.
static const int VQuadTreeSubsectionCount = 2;
///The size reserved for each collision list in the subsections.
static const int VQuadTreeListSize = 24;

///Collision management system using Quad Tree Collision.
class VCollision
{
	///List of quads to test for.
	std::vector<std::unique_ptr<VQuadTree>> quads;
	///The amount of quads to generate.
	unsigned int quadCount = 0;
	///The rectangle for the entire scene.
	sf::FloatRect mainRect; 

	/**
	* Recursively sets up quads to add to the list. Will continue calling itself until all quads have been created.
	* @param subsection Rectangle area of the area the quad will take.
	* @param remaining The amount of levels to call remaining.
	* @param create If true, a new VQuadTree will be created. Else an existing one will be modified.
	*/
	void setupQuad(const sf::FloatRect& subsection, int remaining, bool create = true);

public:
	VCollision();
	virtual ~VCollision();

	/**
	* Initialises the Collision Handling System.
	* @param initialRect The initial large area to perform a Quad Tree collision on (typically the value in WorldBounds).
	*/
	void Initialise(const sf::FloatRect& initialRect);

	/**
	* @param item The object to add to the list. It could be anything as long as it eventually inherits from VBase.
	* @param list The list the object should specifically add to.
	*/
	void AddToList(VBase* item, VCollideList list);

	/**
	* Runs the collision testing and handling process. This will iterate through all quads to test all collisions.
	* @param testOverlap The method to testing a collision, such as Rectangle or Circle based collision testing.
	* @param response An optional response callback if an overlap is found. Only gets called if an overlap is true.
	* @param process An optional process callback if an overlap is found and there is a need to handle further tests and changes to the objects. Only gets called if an overlap is true, and should return true further collision tests are true.
	* @return Function returns true if at least one overlap has been found to be true.
	*/
	bool Run(std::function<bool(VObject*, VObject*)> testOverlap, std::function<void(VObject*, VObject*)>const& response = nullptr, std::function<bool(VObject*, VObject*)>const& process = nullptr);

	///Cleans up the entire Quad Tree List and all Quad Tree Subsections
	void Cleanup();
};

