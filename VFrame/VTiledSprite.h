/**
* @file    VSprite.h
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
* Repeated Sprite rendering class.
*/

#pragma once
#include "VSprite.h"

///Tiled Sprites display a regular texture in a tiled format, so instead of scaling it repeats. This is good for backgrounds and also works with sprite animation!
class VTiledSprite : public VSprite
{
private:
	std::unique_ptr<sf::Image> image;

protected:
	virtual void updateTransform() override;
	virtual void updateFrame() override;

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VSprite VSUPERCLASS;

	/**
	* @param Position Position of the Sprite.
	* @param Filename File path and name of the texture for this sprite.
	*/
	VTiledSprite(sf::Vector2f Position, const sf::String& Filename = "") : VSprite(Position, "") 
	{
		if (Filename != "")
		{
			LoadGraphic(Filename);
		}

		Origin = sf::Vector2f();
		Animation.GetCurrentFrame();
	}

	/**
	* @param x X Position of the Sprite.
	* @param y Y Position of the Sprite.
	* @param Filename File path and name of the texture for this sprite.
	*/
	VTiledSprite(float x = 0, float y = 0, const sf::String& Filename = "") : VSprite(x, y, "") 
	{ 
		if (Filename != "")
		{
			LoadGraphic(Filename);
		}

		Origin = sf::Vector2f();
		Animation.GetCurrentFrame();
	}

	virtual VSprite* LoadGraphic(sf::String filename, bool animated = false, int width = 0, int height = 0, const sf::IntRect& rect = sf::IntRect()) override;
	virtual VSprite* LoadGraphicFromTexture(sf::Texture& texture, bool animated = false, int width = 0, int height = 0, const sf::IntRect& rect = sf::IntRect()) override;

	/**
	* Make sprite texture. (NOT USED FOR TILED SPRITE)
	* @param width Width of sprite.
	* @param height Height of sprite.
	* @param color Fill Colour of sprite.
	* @param outline Outline thickness.
	* @param outlineColor colour of sprite outline.
	*/
	virtual VSprite* MakeGraphic(int width, int height, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent) override { return this; }

	/**
	* Make sprite texture a circle. (NOT USED FOR TILED SPRITE)
	* @param radius Radius of the circle.
	* @param color Fill Colour of sprite.
	* @param outline Outline thickness.
	* @param outlineColor colour of sprite outline.
	*/
	virtual VSprite* MakeGraphicCircle(int radius, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent) override { return this; }

	/**
	* Make sprite texture a multisided shape. (NOT USED FOR TILED SPRITE)
	* @param radius Radius of the circle.
	* @param sides Number of sides of the shape.
	* @param color Fill Colour of sprite.
	* @param outline Outline thickness.
	* @param outlineColor colour of sprite outline.
	*/
	virtual VSprite* MakeGraphicSided(int radius, int sides, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent) { return this; }

	/**
	* Make sprite texture a convex shape. (NOT USED FOR TILED SPRITE)
	* @param points List of points that make out the shape.
	* @param color Fill Colour of sprite.
	* @param outline Outline thickness.
	* @param outlineColor colour of sprite outline.
	*/
	virtual VSprite* MakeGraphicConvex(const std::vector<sf::Vector2f>& points, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent) { return this; }

	virtual void Destroy() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

