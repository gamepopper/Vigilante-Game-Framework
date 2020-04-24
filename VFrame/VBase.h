/**
* @file    VBase.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2020 Tim Stoddard
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* @section DESCRIPTION
*
* Base class for objects, every object that is used in a VState or VSubstate is here.
*/

#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <stdint.h>

#if _DEBUG
#include <iostream>
#endif

///Types of objects the base class could have. This is mainly used for collision tests.
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

///Base class that any object that updates or renders (including VState) inherints from this class.
class VBase
{
private:
	///True when the base has been destroyed using the Destroy function.
	bool destroyed = false;

public:
	///Base can be updated using Update if true.
	bool active = true;
	///Base can be rendered using Draw if true.
	bool visible = true; 
	///Helper member that is mainly used filter out behaviours to objects which aren't alive in the scene.
	bool alive = true;
	///Base can be updated and rendered if true, else it would no longer be treated as being part of the scene.
	bool exists = true;
	///Base object type, typically set in the constructor of an inherited object to specify what type it is.
	VType type = NONE;
	///Counts the amount of times the object is referenced in groups, ensures an object is only destroyed when no longer part of a single group.
	uint32_t RefCount = 0;

#ifdef _DEBUG
	///Counts the overall amount of objects in a state.
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

	///Destroys object and any data that must be destroyed.
	virtual void Destroy()
	{
		destroyed = true;
		exists = false;

#ifdef _DEBUG
		DebugObjectCount--;
#endif
	}

	/**
	* Kills the object by alive to false, alive is also set to false so it will not render or update.
	* To allow it to be rendered or updated, the function must be overridden so only alive is set to false.
	*/
	virtual void Kill()
	{
		alive = false;
		exists = false;
	}

	///Revives the object by setting both alive and exists back to true.
	virtual void Revive()
	{
		alive = true;
		exists = true;
		destroyed = false;
	}

	/**
	* Updates object and any data it contains. (Default does nothing)
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) {	}

	/**
	* Draws object.  (Default does nothing)
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget &RenderTarget)	{}
	
	/**
	* Helper to create strings with format parameters.
	* @param fmt Formatted string that will be printed (for example: %d will print an integer value).
	* @param ... You must pass in all values of each specified type that are required for the formatted string.
	*/
	static const wchar_t* VString(const char* fmt, ...);

	/**
	* Outputs text for logging purposes (to Console, Output Window ect...).
	* @param fmt Formatted string that will be printed (for example: %d will print an integer value).
	* @param ... You must pass in all values of each specified type that are required for the formatted string.
	*/
	static void VLog(const char* fmt, ...);

	/**
	* Outputs text for logging purposes (to Console, Output Window ect...) but will also throw a runtime error or show a popup window of the specific error.
	* @param fmt Formatted string that will be printed (for example: %d will print an integer value).
	* @param ... You must pass in all values of each specified type that are required for the formatted string.
	*/
	static void VLogError(const char* fmt, ...);

	/**
	* Helper function that clears out the command line/terminal.
	*/
	static void VClearLog();

#ifndef VFRAME_NO_3D
#ifdef _DEBUG
#define glCheck(expr) do { expr; VBase::VCheckGLError(__FILE__, __LINE__, #expr); } while (false)
	static void VCheckGLError(const char* file, unsigned int line, const char* expression);
#else
	///Function to provide debugging help for OpenGL functions.
#define glCheck(expr) (expr)
#endif

#endif
};