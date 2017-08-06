#pragma once
#include "VObject.h"

#include <vector>

#include <functional>

enum VCollideList : bool
{
	A,
	B
};

class VCollision
{
	std::vector<VObject*> listA;
	std::vector<VObject*> listB;

	virtual bool testOverlap(VObject* a, VObject* b);

public:
	VCollision() {}
	virtual ~VCollision()
	{
		listA.clear();
		listB.clear();

		listA.shrink_to_fit();
		listB.shrink_to_fit();
	}

	void AddToListA(VBase* item);
	void AddToListB(VBase* item);
	bool Run(std::function<void(VObject*, VObject*)>const& response, std::function<bool(VObject*, VObject*)>const& process);
};

class VCircleCollision : public VCollision
{
	virtual bool testOverlap(VObject* a, VObject* b) override;

public:
	VCircleCollision() {}
	virtual ~VCircleCollision() {};
};

