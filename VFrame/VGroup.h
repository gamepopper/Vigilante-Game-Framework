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
	virtual ~VGroup()
	{
		Destroy();
	}

	int Length()
	{
		return length;
	}

	VBase* Add(VBase* object);
	VBase* Remove(VBase* object, bool splice = false);

	VBase* FirstAvailable();
	int FirstNULL();
	VBase* FirstExisting();
	VBase* FirstAlive();
	VBase* FirstDead();

	int CountAlive();
	int CountDead();
	VBase* GetRandom(int min = 0, int max = 0);

	void ForEach(std::function<void(VBase*)> function, bool recursive = false);
	void ForEachAlive(std::function<void(VBase*)> function, bool recursive = false);
	void ForEachDead(std::function<void(VBase*)> function, bool recursive = false);
	void ForEachExists(std::function<void(VBase*)> function, bool recursive = false);
	template <class T>
	void ForEachType(void (*function)(T* object), bool recursive = false);

	VBase* GetGroupItem(int index);
	int GetIndexOfItem(VBase* object);

	void OrganiseNULLS();
	void Swap(int a, int b);
	void Sort(std::function<bool(VBase*, VBase*)> func);

	void Reverse();

	void Clear();

	virtual void Destroy() override;
	virtual void Kill() override;
	virtual void Revive() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};
