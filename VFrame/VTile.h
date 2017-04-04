#pragma once
#include "VObject.h"

class VTile : public VObject
{
public:
	VTile(sf::Vector2f position, sf::Vector2f size) : VObject(position, size)
	{
		Immovable = true;
		Moves = false;
		type = OBJECT;
	}

	VTile(float x, float y, float width, float height) : VObject(x,y,width,height)
	{
		Immovable = true;
		Moves = false;
		type = OBJECT;
	}
	virtual ~VTile()
	{
		Destroy();
	}
};