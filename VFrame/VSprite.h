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
* Sprite rendering class.
*/

#pragma once
#include "VObject.h"
#include "VAnimationManager.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <memory>

///A sprite rendering object class.
class VSprite :	public VObject
{
protected:
	///If true, texture will be destroyed within VSprite. Important if texture isn't loaded from VContent.
	bool disposible = false;
	///Sprite render infomation.
	sf::VertexArray vertexArray;

	/**
	* Sets up the Sprite's frame size and animation.
	* @param texWidth The area width of the texture for the sprite.
	* @param texHeight The area height of the texture for the sprite.
	* @param animated If false, sprite frame will be the entire texture area.
	* @param width Frame width (ignored if animated is false).
	* @param height Frame height (ignored if animated is false).
	* @param offsetX Texture X offset if the entire texture won't be used for the sprite.
	* @param offsetY Texture Y offset if the entire texture won't be used for the sprite.
	*/
	void setSize(unsigned int texWidth, unsigned int texHeight, bool animated = false, int width = 0, int height = 0, int offsetX = 0, int offsetY = 0);

	///Update the tranform.
	virtual void updateTransform() override;
	///Update the current frame for animations.
	virtual void updateFrame();

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VObject VSUPERCLASS;

	///RenderState for the Sprite.
	sf::RenderStates RenderState = sf::RenderStates::Default;
	///Colour Tint of the Sprite.
	sf::Color Tint = sf::Color::White;
	///Origin of the Sprite in pixels (for rotation).
	sf::Vector2f Origin;
	///Offset of the graphic from the Position.
	sf::Vector2f Offset;
	///Scale transform.
	sf::Vector2f Scale;
	///The size of a frame, can be different from Size.
	sf::Vector2u FrameSize;
	///If true, the sprite is flipped horzontally.
	bool FlipX = false;
	///If true, the sprite is flipped vertically.
	bool FlipY = false;
	///The animation manager for this sprite. Use this to set up and control sprite animations.
	VAnimationManager Animation;

	/**
	* @param Position Position of the Sprite.
	* @param Filename File path and name of the texture for this sprite.
	*/
	VSprite(sf::Vector2f Position, const sf::String& Filename = "");

	/**
	* @param x X Position of the Sprite.
	* @param y Y Position of the Sprite.
	* @param filename File path and name of the texture for this sprite.
	*/
	VSprite(float x = 0, float y = 0, const sf::String& filename = "");

	/**
	* Load Graphic from filepath.
	* @param filename Path of texture file.
	* @param animated Sets if the graphic is part of an animation or not.
	* @param width Width of sprite, ignored if animated is false.
	* @param height Height of sprite, ignored if animated is false.
	* @param rect Only use a certain area of the texture, whether you are animated or not. Default value assumes whole texture is used. (Ignored if VTiledSprite or VBackdrop)
	*/
	virtual VSprite* LoadGraphic(sf::String filename, bool animated = false, int width = 0, int height = 0, const sf::IntRect& rect = sf::IntRect());
	
	/**
	* Load Graphic from an sf::Texture object.
	* @param texture sf::Texture object.
	* @param animated Sets if the graphic is part of an animation or not.
	* @param width Width of sprite, ignored if animated is false.
	* @param height Height of sprite, ignored if animated is false.
	* @param rect Only use a certain area of the texture, whether you are animated or not. Default value assumes whole texture is used. (Ignored if VTiledSprite or VBackdrop)
	*/
	virtual VSprite* LoadGraphicFromTexture(sf::Texture& texture, bool animated = false, int width = 0, int height = 0, const sf::IntRect& rect = sf::IntRect());
	
	/**
	* Make sprite texture as a primitive rectangle. This should be used mainly for testing purposes with VSprites. For more permanent use of primitive shapes, look at the VShape class.
	* @param width Width of sprite.
	* @param height Height of sprite.
	* @param color Fill Colour of sprite.
	* @param outline Outline thickness.
	* @param outlineColor colour of sprite outline.
	*/
	virtual VSprite* MakeGraphic(int width, int height, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent);
	
	/**
	* Make sprite texture a circle. This should be used mainly for testing purposes with VSprites. For more permanent use of primitive shapes, look at the VShape class.
	* @param radius Radius of the circle.
	* @param color Fill Colour of sprite.
	* @param outline Outline thickness.
	* @param outlineColor colour of sprite outline.
	*/
	virtual VSprite* MakeGraphicCircle(int radius, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent);

	///Destroys the sprite rendering data (and texture if disposable is set for true).
	virtual void Destroy() override;

	/**
	* Updates animation data.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) override;

	/**
	* Renders the sprite. Updates sprite transform and rendering info.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

