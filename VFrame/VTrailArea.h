/**
* @file    VTrailArea.h
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
* Similar to VRenderGroup, except it displays a trail effect within a specific area.
*/

#pragma once
#include "VRenderGroup.h"
#include <SFML/System/Clock.hpp>

///Special VRenderGroup that applies a trail to objects it contains within the area.
class VTrailArea : public VRenderGroup
{
private:
	sf::Sprite trailSprite;
	sf::Texture trailTexture;
	sf::Clock fadeTimer;
	sf::Vector2f lastPos;

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VRenderGroup VSUPERCLASS;
	///Delay between applying a swap to produce a trail.
	float Delay = 0.05f;
	///The amount of red to remove in the next swap (done as 255 - (255 * Multiplier * 0.0039f).
	float RedMultiplier		= 1.0f;
	///The amount of blue to remove in the next swap (done as 255 - (255 * Multiplier * 0.0039f).
	float BlueMultiplier	= 1.0f;
	///The amount of green to remove in the next swap (done as 255 - (255 * Multiplier * 0.0039f).
	float GreenMultiplier	= 1.0f;
	///The amount of alpha to remove in the next swap (done as 255 - (255 * Multiplier * 0.0039f).
	float AlphaMultiplier	= 0.8f;

	/**
	* @param x X position of the sprite.
	* @param y Y position of the sprite.
	* @param width Width of the sprite (also sets the width of the render area).
	* @param height Height of the sprite (also sets the height of the render area).
	* @param maxSize The fixed length of the group. If 0, then the VGroup has no limit in size.
	*/
	VTrailArea(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize = 0);
	
	/**
	* @param position Position of the sprite.
	* @param size Size of the sprite (also sets the size of the render area).
	* @param maxSize The fixed length of the group. If 0, then the VGroup has no limit in size.
	*/
	VTrailArea(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize = 0);

	/**
	* Updates all alive and active objects in the group and renders all objects to the render texture to apply to the trail area.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) override;

	/**
	* Applies the renderTex to the sprite and renders the sprite to the scene.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

