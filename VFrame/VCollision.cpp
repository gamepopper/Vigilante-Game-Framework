#include "VCollision.h"

#include "VGroup.h"
#include "VTilemap.h"
#include "VRenderGroup.h"

#include <cmath>

using std::vector;

void VCollision::AddToListA(VBase* item)
{
	if (item == NULL)
		return;
	
	if (item->type == GROUP)
	{
		//If group, add all members to list.
		VGroup* group = dynamic_cast<VGroup*>(item);

		if (group != NULL)
		{
			for (int i = 0; i < group->Length(); i++)
			{
				AddToListA(group->GetGroupItem(i));
			}
		}
	}
	else if (item->type == TILE)
	{
		//If Tilemap, add all tiles to list.
		VTilemap* tilemap = dynamic_cast<VTilemap*>(item);

		if (tilemap != NULL)
		{
			vector<VTile*> tiles = tilemap->Tiles;

			for (VTile* t : tiles)
			{
				AddToListA(t);
			}
		}
	}
	else if (item->type == RENDERGROUP)
	{
		VRenderGroup* renderGroup = dynamic_cast<VRenderGroup*>(item);

		if (renderGroup != NULL)
		{
			AddToListA(renderGroup->Sprite);
		}
	}
	else
	{
		VObject* object = dynamic_cast<VObject*>(item);

		if (object && object->alive)
		{
			listA.push_back(object);
		}
	}
}

void VCollision::AddToListB(VBase* item)
{
	if (item == NULL)
		return;
	
	if (item->type == GROUP)
	{
		//If group, add all members to list.
		VGroup* group = dynamic_cast<VGroup*>(item);

		if (group != NULL)
		{
			for (int i = 0; i < group->Length(); i++)
			{
				AddToListB(group->GetGroupItem(i));
			}
		}
	}
	else if (item->type == TILE)
	{
		//If Tilemap, add all tiles to list.
		VTilemap* tilemap = dynamic_cast<VTilemap*>(item);

		if (tilemap != NULL)
		{
			vector<VTile*> tiles = tilemap->Tiles;

			for (VTile* t : tiles)
			{
				AddToListB(t);
			}
		}
	}
	else if (item->type == TILE)
	{
		//If Tilemap, add all tiles to list.
		VTilemap* tilemap = dynamic_cast<VTilemap*>(item);

		if (tilemap != NULL)
		{
			vector<VTile*> tiles = tilemap->Tiles;

			for (VTile* t : tiles)
			{
				AddToListB(t);
			}
		}
	}
	else if (item->type == RENDERGROUP)
	{
		VRenderGroup* renderGroup = dynamic_cast<VRenderGroup*>(item);

		if (renderGroup != NULL)
		{
			AddToListB(renderGroup->Sprite);
		}
	}
	else
	{
		VObject* object = dynamic_cast<VObject*>(item);

		if (object && object->alive)
		{
			listB.push_back(object);
		}
	}
}

bool VCollision::Run(std::function<void(VObject*, VObject*)>const& response, std::function<bool(VObject*, VObject*)>const& process)
{
	bool overlapFound = false;

	if (listB.size() != 0)
	{
		for (unsigned int a = 0; a < listA.size(); a++)
		{
			for (unsigned int b = 0; b < listB.size(); b++)
			{
				if (testOverlap(listA[a], listB[b]))
				{
					if (process != NULL)
					{
						process(listA[a], listB[b]);
					}

					if (response != NULL)
					{
						response(listA[a], listB[b]);
					}

					overlapFound = true;
				}
			}
		}
	}
	else
	{
		for (unsigned int a = 0; a < listA.size(); a++)
		{
			for (unsigned int b = 0; b < listA.size(); b++)
			{
				if (a == b)
					continue;
				else if (listA[a] == listA[b])
					continue;

				if (testOverlap(listA[a], listA[b]))
				{
					if (process != NULL)
					{
						process(listA[a], listA[b]);
					}

					if (response != NULL)
					{
						response(listA[a], listA[b]);
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
