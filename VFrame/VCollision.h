#pragma once
#include "VObject.h"

#include <vector>
#include <functional>
#include <memory>

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
static const int VQuadTreeListSize = 24;

class VCollision
{
	std::vector<std::unique_ptr<VQuadTree>> quads;
	unsigned int quadCount = 0;
	sf::FloatRect mainRect;

	void setupQuad(const sf::FloatRect& subsection, int remaining, bool create = true);

public:
	VCollision();
	virtual ~VCollision();

	//Initialises Quad Tree Collision System, required call for first use.
	void Initialise(const sf::FloatRect& initialRect);

	//Adds objects from first parameter to list specified in second parameter.
	void AddToList(VBase* item, VCollideList List);

	//Applies response and process functions to any object from list A that overlaps an object from List B.
	bool Run(std::function<bool(VObject*, VObject*)> testOverlap, std::function<void(VObject*, VObject*)>const& response, std::function<bool(VObject*, VObject*)>const& process);

	//Cleans up Quad Tree List
	void Cleanup();
};

