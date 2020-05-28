/**
* @file    VTile.h
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
* A basic data for tile-based collisions in a VTilemap.
*/

#pragma once
#include "VObject.h"
#include <functional>

class VTIlemap;

///Class for representing a set of tiles for collision purposes.
class VTile : public VObject
{
private:
	sf::Vector2f relativePos;

public:

	typedef VObject VSUPERCLASS;

	///Callback for tile collisions.
	std::function<void(VObject*, VObject*)> Callback = nullptr;
	///For any tile specific collision behaviour, this specifies the tile ID the tile uses.
	char MainTile = (char)0;

	VTilemap* Tilemap;

	/**
	* @param position Position of the tile.
	* @param size Size of the of the tile, can be bigger than a single tile if it represents a single group of tiles.
	*/
	VTile(sf::Vector2f position, sf::Vector2f size);

	/**
	* @param x X Position of the tile.
	* @param y Y Position of the tile.
	* @param width Width of the of the tile, can be bigger than a single tile if it represents a single group of tiles.
	* @param height Height of the of the tile, can be bigger than a single tile if it represents a single group of tiles.
	*/
	VTile(float x, float y, float width, float height);

	virtual void Update(float dt) override;
};