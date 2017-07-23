#pragma once
#include "VObject.h"
#include <functional>

class VTile : public VObject
{
public:
	std::function<void(VObject*, VObject*)> Callback = nullptr;

	VTile(sf::Vector2f position, sf::Vector2f size) : VObject(position, size)
	{
		Immovable = true;
		Moves = false;
		type = TILE;

#if _DEBUG
		DebugColor = sf::Color::Red;
#endif
	}

	VTile(float x, float y, float width, float height) : VObject(x,y,width,height)
	{
		Immovable = true;
		Moves = false;
		type = TILE;

#if _DEBUG
		DebugColor = sf::Color::Red;
#endif
	}
};