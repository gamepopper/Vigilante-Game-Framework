#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <stdint.h>

#if _DEBUG
#include <iostream>
#endif

enum VType : unsigned char
{
	NONE,
	OBJECT,
	TILE,
	TILEMAP,
	GROUP,
	RENDERGROUP,
	NUMTYPES
};

class VBase
{
private:
	bool destroyed = false;

public:
	bool active = true;
	bool visible = true;
	bool alive = true;
	bool exists = true;
	VType type = NONE;
	uint32_t RefCount = 0;

#ifdef _DEBUG
	static int DebugObjectCount;
#endif

	VBase()
	{
#ifdef _DEBUG
		DebugObjectCount++;
#endif
	}

	virtual ~VBase()
	{
		if (!destroyed)
			Destroy();
	}

	//Destroys object and any data that must be destroyed.
	virtual void Destroy()
	{
		destroyed = true;
		exists = false;

#ifdef _DEBUG
		DebugObjectCount--;
#endif
	}

	//Kills the object, so it's still in-scene but will not render or update.
	virtual void Kill()
	{
		alive = false;
		exists = false;
	}

	//Revives the object after it's been killed.
	virtual void Revive()
	{
		alive = true;
		exists = true;
	}

	//Updates object and any data it contains.
	virtual void Update(float dt) {	}

	//Renders object onto scene.
	virtual void Draw(sf::RenderTarget &RenderTarget)	{}

	//Outputs text for logging purposes (to Console, Output Window ect...).
	static void VLog(const char* fmt, ...);

	//Outputs text with VLog, then throws an error message to let the user know this is serious.
	static void VLogError(const char* fmt, ...);

	//Clears output log
	static void VClearLog();
};