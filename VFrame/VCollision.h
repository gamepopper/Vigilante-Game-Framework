#pragma once
#include "VObject.h"

#include <vector>

#include <functional>

enum VCollideList : bool
{
	A,
	B
};

struct VQuadTree
{
	std::vector<VObject*> listA;
	std::vector<VObject*> listB;
	sf::FloatRect bounds;

	bool checkBounds(VObject* object, VCollideList list);

	void clear();
};

class VCollision
{
	enum QuadTreeDir : unsigned char
	{
		NORTHWEST,
		NORTHEAST,
		SOUTHWEST,
		SOUTHEAST,
	};
	std::vector<VQuadTree*> quads;

	virtual bool testOverlap(VObject* a, VObject* b);

public:
	VCollision();
	virtual ~VCollision();

	void AddToList(VBase* item, VCollideList List);

	bool Run(std::function<void(VObject*, VObject*)>const& response, std::function<bool(VObject*, VObject*)>const& process);
};

class VCircleCollision : public VCollision
{
	virtual bool testOverlap(VObject* a, VObject* b) override;

public:
	VCircleCollision() {}
	virtual ~VCircleCollision() {};
};

