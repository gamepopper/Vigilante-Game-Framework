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

	listA.shrink_to_fit();
	listB.shrink_to_fit();
}

VCollision::VCollision()
{
	quads.reserve(pow(4, VQuadTreeSubsectionCount));
	setupQuad(VGlobal::p()->WorldBounds, VQuadTreeSubsectionCount - 1);
}

VCollision::~VCollision()
{
	for (unsigned int i = 0; i < quads.size(); i++)
	{
		quads[i]->clear();
		delete quads[i];
		quads[i] = nullptr;
	}

	quads.clear();
	quads.shrink_to_fit();
}

void VCollision::setupQuad(const sf::FloatRect& subsection, int remaining)
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
		VQuadTree* nwTree = new VQuadTree();
		nwTree->bounds = NW;
		VQuadTree* neTree = new VQuadTree();
		neTree->bounds = NE;
		VQuadTree* swTree = new VQuadTree();
		swTree->bounds = SW;
		VQuadTree* seTree = new VQuadTree();
		seTree->bounds = SE;

		quads.push_back(nwTree);
		quads.push_back(neTree);
		quads.push_back(swTree);
		quads.push_back(seTree);
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
			AddToList(renderGroup->Sprite, list);
		}
	}
	else
	{
		VObject* object = dynamic_cast<VObject*>(item);

		if (object && object->alive)
		{
			for (unsigned int i = 0; i < quads.size(); i++)
			{
				quads[i]->checkBounds(object, list);
			}
		}
	}
}

bool VCollision::Run(std::function<void(VObject*, VObject*)>const& response, std::function<bool(VObject*, VObject*)>const& process)
{
	bool overlapFound = false;

	for (VQuadTree* tree : quads)
	{
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

	return overlapFound;
}

bool VCollision::testOverlap(VObject* a, VObject* b)
{
	if (a->Position.x < b->Position.x + b->Size.x &&
		a->Position.x + a->Size.x > b->Position.x &&
		a->Position.y < b->Position.y + b->Size.y &&
		a->Position.y + a->Size.y > b->Position.y)
	{
		return true;
	}

	return false;
}

bool VCircleCollision::testOverlap(VObject* a, VObject* b)
{
	sf::Vector2f aCentre = a->Position + (a->Size / 2.0f);
	sf::Vector2f bCentre = b->Position + (b->Size / 2.0f);

	sf::Vector2f diff = aCentre - bCentre;
	float length = sqrtf((diff.x * diff.x) + (diff.y * diff.y));

	if (length < a->Radius + b->Radius)
	{
		return true;
	}

	return false;
}
