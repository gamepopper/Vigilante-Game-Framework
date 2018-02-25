/**
* @file    VGroup.h
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
* Group class that processes multiple base classes, which can include VGroups.
*/

#pragma once
#include "VBase.h"

#if _DEBUG
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#endif

#include <vector>
#include <functional>

///An object management class that stores a set of objects to be updated and rendered in a batch.
class VGroup : public VBase
{
protected:
	///VBase List
	std::vector<VBase*> members;
	///Current amount of members in the list.
	int length = 0;
	///Maximum Size of the list, used if VGroup is set to have a fixed size.
	unsigned int MaxSize = 0;

#if _DEBUG
	///Render vertices for drawing collision bounderies of all the objects. Used when in Debug mode with DebugDraw = true.
	sf::VertexArray debuggingVertices;
#endif

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VBase VSUPERCLASS;

	///@param maxSize The fixed length of the group. If 0, then the VGroup has no limit in size.
	VGroup(unsigned int maxSize = 0) : VBase()
	{
		MaxSize = maxSize;

		if (MaxSize > 0)
		{
			members.reserve(MaxSize);

#if _DEBUG
			debuggingVertices.resize(length * 8);
#endif
		}

		type = VType::GROUP;

#if _DEBUG
		debuggingVertices.setPrimitiveType(sf::Lines);
#endif
	}

	///@return The current size of the group based on amount of active elements.
	int Length()
	{
		return length;
	}

	///@param object The VBase object to add to the group.
	VBase* Add(VBase* object);
	
	/**
	* @param object The VBase object to remove from the group.
	* @param splice If true, all elements above the object will be moved down, else the element will become null.
	*/
	VBase* Remove(VBase* object, bool splice = false);

	///@return First object regardless of state.
	VBase* FirstAvailable();
	///@return First index that is set to nullptr.
	int FirstNULL();
	///@return First object that exists in scene.
	VBase* FirstExisting();
	///@return First object that is alive.
	VBase* FirstAlive();
	///@return First object that is dead.
	VBase* FirstDead();

	///@return Amount of alive objects.
	int CountAlive();
	///@return Amount of dead objects.
	int CountDead();
	///@return Random object from a set range. Min and Max will be clamped to 0 and maxSize.
	VBase* GetRandom(int min = 0, int max = 0);

	/**
	* Applies function for each object in group.
	*/
	void ForEach(std::function<void(VBase*)> function, bool recursive = false);

	/**
	* Applies function for each object in group that's alive.
	*/
	void ForEachAlive(std::function<void(VBase*)> function, bool recursive = false);

	/**
	* Applies function for each object in group that's dead.
	*/
	void ForEachDead(std::function<void(VBase*)> function, bool recursive = false);

	/**
	* Applies function for each object in group that exists.
	*/
	void ForEachExists(std::function<void(VBase*)> function, bool recursive = false);

	/**
	* Gets an object from a specified index in group.
	*/
	VBase* GetGroupItem(int index);

	/**
	* Gets the index of a specific object.
	*/
	int GetIndexOfItem(VBase* object);

	///Sort nullptr members to the bottom of the group.
	void OrganiseNULLS();

	/**
	* Swaps order of two groups.
	*/
	void Swap(int a, int b);

	/**
	* Sorts objects based on function.
	*/
	void Sort(std::function<bool(VBase*, VBase*)> func);

	///Reverse order of group.
	void Reverse();
	///Clears the whole group (does not destroy any of the members, that must be done using the Destroy method).
	void Clear();
	///Removes and/or destroys all objects inside the group before destroying extra data. (If an object is referenced in another VGroup, the object won't be destroyed)
	virtual void Destroy() override;
	///Kills all living objects before killing the group itself.
	virtual void Kill() override;
	///Revives all dead objects before reviving the group itself.
	virtual void Revive() override;

	/**
	* Updates all alive and active objects in the group.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) override;

	/**
	* Renders all alive and visible objects in the group.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};
