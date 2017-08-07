#pragma once
#include "VObject.h"

#include <vector>

#include <functional>

enum VCollideList : bool
{
	A,
	B
};

//Separate entire world into four sections, and only test collisions of objects within each section. 
//This reduces the amount of collision tests, but requires WorldBounds to be set to the correct world size.
struct VQuadTree
{
	std::vector<VObject*> listA;
	std::vector<VObject*> listB;
	sf::FloatRect bounds;

	//If object is within bounds, add to list. Else, ignore.
	bool checkBounds(VObject* object, VCollideList list);

	//Clear both lists.
	void clear();
};

static const int VQuadTreeSubsectionCount = 2;

class VCollision
{
	std::vector<VQuadTree*> quads;

	virtual bool testOverlap(VObject* a, VObject* b);
	void setupQuad(const sf::FloatRect& subsection, int remaining);

public:
	VCollision();
	virtual ~VCollision();

	//Adds objects from first parameter to list specified in second parameter.
	void AddToList(VBase* item, VCollideList List);

	//Applies response and process functions to any object from list A that overlaps an object from List B.
	bool Run(std::function<void(VObject*, VObject*)>const& response, std::function<bool(VObject*, VObject*)>const& process);
};

//Special case that tests using a circle collision instead of rectangular.
class VCircleCollision : public VCollision
{
	virtual bool testOverlap(VObject* a, VObject* b) override;

public:
	VCircleCollision() {}
	virtual ~VCircleCollision() {};
};

