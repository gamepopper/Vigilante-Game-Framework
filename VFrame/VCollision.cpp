#include "VCollision.h"

#include "VGlobal.h"
#include "VGroup.h"
#include "VTilemap.h"
#include "VTile.h"
#include "VRenderGroup.h"

#include <cmath>

bool VQuadTree::checkBounds(VObject* object, VCollideList list)
{
	sf::FloatRect objRect = sf::FloatRect(object->Position, object->Size);

	if (bounds.intersects(objRect))
	{
		if (list == A)
			listA.push_back(object);
		else
			listB.push_back(object);

		return true;
	}

	return false;
}

void VQuadTree::clear()
{
	listA.clear();
	listB.clear();
}

VCollision::VCollision()
{
	
}

VCollision::~VCollision()
{
	quads.clear();
	quads.shrink_to_fit();
}

void VCollision::Initialise(const sf::FloatRect& initialRect)
{
	if (mainRect != initialRect)
	{
		mainRect = initialRect;
		quadCount = 0;

		if (quads.size() == 0)
		{
			quads.reserve((unsigned int)pow(4, VQuadTreeSubsectionCount));
			setupQuad(initialRect, VQuadTreeSubsectionCount - 1);
		}
		else
		{
			setupQuad(initialRect, VQuadTreeSubsectionCount - 1, false);
		}
	}
}

void VCollision::Cleanup()
{
	for (unsigned int i = 0; i < quads.size(); i++)
	{
		quads[i]->clear();
	}
}

void VCollision::setupQuad(const sf::FloatRect& subsection, int remaining, bool create)
{
	sf::FloatRect NW = sf::FloatRect(
		subsection.left, 
		subsection.top,
		subsection.width / 2,
		subsection.height / 2);

	sf::FloatRect NE = sf::FloatRect(
		subsection.left + (subsection.width / 2), 
		subsection.top,
		subsection.width / 2,
		subsection.height / 2);

	sf::FloatRect SW = sf::FloatRect(
		subsection.left,
		subsection.top + (subsection.height / 2),
		subsection.width / 2,
		subsection.height / 2);

	sf::FloatRect SE = sf::FloatRect(
		subsection.left + (subsection.width / 2),
		subsection.top + (subsection.height / 2),
		subsection.width / 2,
		subsection.height / 2);

	if (remaining <= 0)
	{
		if (create)
		{
			quads.push_back(std::make_unique<VQuadTree>());
			quads.push_back(std::make_unique<VQuadTree>());
			quads.push_back(std::make_unique<VQuadTree>());
			quads.push_back(std::make_unique<VQuadTree>());
		}

		quads[quadCount]->listA.reserve(VQuadTreeListSize);
		quads[quadCount]->listB.reserve(VQuadTreeListSize);
		quads[quadCount++]->bounds = NW;
		quads[quadCount]->listA.reserve(VQuadTreeListSize);
		quads[quadCount]->listB.reserve(VQuadTreeListSize);
		quads[quadCount++]->bounds = NE;
		quads[quadCount]->listA.reserve(VQuadTreeListSize);
		quads[quadCount]->listB.reserve(VQuadTreeListSize);
		quads[quadCount++]->bounds = SW;
		quads[quadCount]->listA.reserve(VQuadTreeListSize);
		quads[quadCount]->listB.reserve(VQuadTreeListSize);
		quads[quadCount++]->bounds = SE;

		return;
	}

	setupQuad(NW, remaining - 1);
	setupQuad(NE, remaining - 1);
	setupQuad(SW, remaining - 1);
	setupQuad(SE, remaining - 1);
}

void VCollision::AddToList(VBase* item, VCollideList list)
{
	if (item == nullptr)
		return;
	
	if (item->type == GROUP)
	{
		//If group, add all members to list.
		VGroup* group = dynamic_cast<VGroup*>(item);

		if (group != nullptr)
		{
			for (int i = 0; i < group->Length(); i++)
			{
				AddToList(group->GetGroupItem(i), list);
			}
		}
	}
	else if (item->type == TILEMAP)
	{
		//If Tilemap, add to list. Tilemap object will handle individual tile collisions.
		VTilemap* tilemap = dynamic_cast<VTilemap*>(item);

		if (tilemap != nullptr)
		{
			for (unsigned int i = 0; i < quads.size(); i++)
			{
				quads[i]->checkBounds(tilemap, list);
			}
		}
	}
	else if (item->type == RENDERGROUP)
	{
		//If RenderGroup, add sprite object from renderGroup to list.
		VRenderGroup* renderGroup = dynamic_cast<VRenderGroup*>(item);

		if (renderGroup != nullptr)
		{
			AddToList(renderGroup->Sprite.get(), list);
		}
	}
	else
	{
		VObject* object = dynamic_cast<VObject*>(item);

		if (object && object->exists)
		{
			for (unsigned int i = 0; i < quads.size(); i++)
			{
				quads[i]->checkBounds(object, list);
			}
		}
	}
}

bool VCollision::Run(std::function<bool(VObject*, VObject*)> testOverlap, std::function<void(VObject*, VObject*)>const& response, std::function<bool(VObject*, VObject*)>const& process)
{
	bool overlapFound = false;

	for (unsigned int i = 0; i < quads.size(); i++)
	{
		VQuadTree* tree = quads[i].get();

		unsigned int listASize = tree->listA.size();
		unsigned int listBSize = tree->listB.size();

		if (listASize == 0 || listBSize == 0)
			continue;

		for (unsigned int a = 0; a < listASize; a++)
		{
			for (unsigned int b = 0; b < listBSize; b++)
			{
				if (tree->listA[a] == tree->listB[b]) //Skip if both elements are the same object.
					continue;

				if (testOverlap(tree->listA[a], tree->listB[b]))
				{
					if (process != nullptr)
					{
						process(tree->listA[a], tree->listB[b]);
					}

					if (response != nullptr)
					{
						response(tree->listA[a], tree->listB[b]);
					}

					overlapFound = true;
				}
			}
		}
	}

	Cleanup();

	return overlapFound;
}
