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
	quads.resize(4);
	quads[NORTHWEST] = new VQuadTree();
	quads[NORTHWEST]->bounds = sf::FloatRect(
		0.0f, 0.0f, 
		VGlobal::p()->WorldBounds.width / 2, 
		VGlobal::p()->WorldBounds.height / 2);
	quads[NORTHEAST] = new VQuadTree();
	quads[NORTHEAST]->bounds = sf::FloatRect(
		VGlobal::p()->WorldBounds.width / 2, 0.0f, 
		VGlobal::p()->WorldBounds.width / 2, 
		VGlobal::p()->WorldBounds.height / 2);
	quads[SOUTHWEST] = new VQuadTree();
	quads[SOUTHWEST]->bounds = sf::FloatRect(
		0.0f, 
		VGlobal::p()->WorldBounds.height / 2, 
		VGlobal::p()->WorldBounds.width / 2, 
		VGlobal::p()->WorldBounds.height / 2);
	quads[SOUTHEAST] = new VQuadTree();
	quads[SOUTHEAST]->bounds = sf::FloatRect(
		VGlobal::p()->WorldBounds.width / 2, 
		VGlobal::p()->WorldBounds.height / 2, 
		VGlobal::p()->WorldBounds.width / 2, 
		VGlobal::p()->WorldBounds.height / 2);
}

VCollision::~VCollision()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		quads[i]->clear();
		delete quads[i];
		quads[i] = nullptr;
	}

	quads.clear();
	quads.shrink_to_fit();
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
			/*std::vector<VTile*> tiles = tilemap->Tiles;

			for (VTile* t : tiles)
			{
				AddToListA(t);
			}*/

			quads[NORTHEAST]->checkBounds(tilemap, list);
			quads[NORTHWEST]->checkBounds(tilemap, list);
			quads[SOUTHEAST]->checkBounds(tilemap, list);
			quads[SOUTHWEST]->checkBounds(tilemap, list);
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
			quads[NORTHEAST]->checkBounds(object, list);
			quads[NORTHWEST]->checkBounds(object, list);
			quads[SOUTHEAST]->checkBounds(object, list);
			quads[SOUTHWEST]->checkBounds(object, list);
		}
	}
}

bool VCollision::Run(std::function<void(VObject*, VObject*)>const& response, std::function<bool(VObject*, VObject*)>const& process)
{
	bool overlapFound = false;

	for (VQuadTree* tree : quads)
	{
		if (tree->listB.size() > 0)
		{
			for (unsigned int a = 0; a < tree->listA.size(); a++)
			{
				for (unsigned int b = 0; b < tree->listB.size(); b++)
				{
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
		else if (tree->listA.size() > 1)
		{
			for (unsigned int a = 0; a < tree->listA.size(); a++)
			{
				for (unsigned int b = 0; b < tree->listA.size(); b++)
				{
					if (a == b)
						continue;
					else if (tree->listA[a] == tree->listA[b])
						continue;

					if (testOverlap(tree->listA[a], tree->listA[b]))
					{
						if (process != nullptr)
						{
							process(tree->listA[a], tree->listA[b]);
						}

						if (response != nullptr)
						{
							response(tree->listA[a], tree->listA[b]);
						}

						overlapFound = true;
					}
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
