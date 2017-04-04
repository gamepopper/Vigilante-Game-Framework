#pragma once
#include <SFML\Graphics\RenderTarget.hpp>

#if _DEBUG
#include <iostream>
#endif

enum VType
{
	NONE,
	OBJECT,
	TILE,
	GROUP,
	RENDERGROUP,
	NUMTYPES
};

class VBase
{
public:
	bool active = true;
	bool visible = true;
	bool alive = true;
	bool exists = true;
	VType type = NONE;
	int RefCount = 0;

	VBase() = default;
	virtual ~VBase()
	{
		Destroy();
	}

	virtual void Destroy()
	{
		exists = false;
	}

	virtual void Kill()
	{
		alive = false;
		exists = false;
	}

	virtual void Revive()
	{
		alive = true;
		exists = true;
	}

	virtual void Update(float dt) {	}

	virtual void Draw(sf::RenderTarget &RenderTarget)	{}

	static void VLog(const char* fmt, ...);
};