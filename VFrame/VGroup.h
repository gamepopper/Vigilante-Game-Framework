#pragma once
#include "VBase.h"

#if _DEBUG
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#endif

#include <vector>
#include <functional>

class VGroup : public VBase
{
protected:
	std::vector<VBase*> members;
	int length = 0;
	unsigned int MaxSize = 0;

#if _DEBUG
	sf::VertexArray debuggingVertices;
#endif

public:
	typedef VBase VSUPERCLASS;	
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

	int Length()
	{
		return length;
	}

	//Add object to group.
	VBase* Add(VBase* object);
	//Remove object from group.
	VBase* Remove(VBase* object, bool splice = false);

	//Gets first object regardless of state.
	VBase* FirstAvailable();
	//Gets first index that is set to nullptr.
	int FirstNULL();
	//Gets first object that exists in scene.
	VBase* FirstExisting();
	//Gets first object that is alive.
	VBase* FirstAlive();
	//Gets first object that is dead.
	VBase* FirstDead();

	//Count alive objects.
	int CountAlive();
	//Count dead objects.
	int CountDead();
	//Gets random object from a set range. Min and Max will be clamped to 0 and maxSize.
	VBase* GetRandom(int min = 0, int max = 0);

	//Applies function for each object in group.
	void ForEach(std::function<void(VBase*)> function, bool recursive = false);
	//Applies function for each object in group that's alive.
	void ForEachAlive(std::function<void(VBase*)> function, bool recursive = false);
	//Applies function for each object in group that's dead.
	void ForEachDead(std::function<void(VBase*)> function, bool recursive = false);
	//Applies function for each object in group that exists.
	void ForEachExists(std::function<void(VBase*)> function, bool recursive = false);

	//Gets an object from a specified index in group.
	VBase* GetGroupItem(int index);
	//Gets the index of a specific object.
	int GetIndexOfItem(VBase* object);

	//Sort nullptr members to the bottom of the group.
	void OrganiseNULLS();
	//Swaps order of two groups.
	void Swap(int a, int b);
	//Sorts objects based on function.
	void Sort(std::function<bool(VBase*, VBase*)> func);

	//Reverse order of group.
	void Reverse();
	//Clear group.
	void Clear();

	virtual void Destroy() override;
	virtual void Kill() override;
	virtual void Revive() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};
