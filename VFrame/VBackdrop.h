/**
* @file    VBackdrop.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2018 Tim Stoddard
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
* Object that renders a full texture that can be repeated and scrolled with the camera. Good for backgrounds.
*/

#pragma once
#include "VObject.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/String.hpp>

///Renders and endless scrolling scene that takes up the whole screen. Good for scrolling backgrounds.
class VBackdrop : public VObject
{
protected:
	///The texture to render.
	sf::Texture* texture = nullptr;
	///The vertex data for the object.
	sf::VertexArray vertices;
	///Transform matrix for the object.
	sf::Transform transform;

	///Updates the transform.
	virtual void updateTransform() override;

	///If true, the texture will appear repeated in the X Axis, else the texture will lock in the X axis.
	bool RepeatX;
	///If true, the texture will appear repeated in the Y Axis, else the texture will lock in the Y axis.
	bool RepeatY;

	bool disposable = false;

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VObject VSUPERCLASS;

	///Scroll Amount. Used when CameraScroll is false.
	sf::Vector2f Scroll;
	///Scroll Velocity Amount. Used when CameraScroll is false.
	sf::Vector2f ScrollVelocity;
	///Scroll Acceleration Amount. Used when CameraScroll is false.
	sf::Vector2f ScrollAcceleration;
	///Scroll Drag Amount. Used when CameraScroll is false.
	sf::Vector2f ScrollDrag;
	///Maximum Scroll Velocity. Used when CameraScroll is false.
	sf::Vector2f ScrollMaxVelocity;
	///Scale of the whole background.
	sf::Vector2f Scale;
	///RenderState of the backdrop.
	sf::RenderStates RenderState;

	///Overrides the scrolling coordinates to use the game's camera.
	bool CameraScroll = true;

	/**
	* @param scrollX The parallax scroll amount in the X-axis, set to ScrollFactor. (1.0f = Scrolls as fast as a Camera, 0.5f = Scrolls at half the speed of the camera).
	* @param scrollY The parallax scroll amount in the Y-axis, set to ScrollFactor. (1.0f = Scrolls as fast as a Camera, 0.5f = Scrolls at half the speed of the camera).
	* @param repeatX Repeats the texture in horizontally.
	* @param repeatY Repeats the texture in vertically.
	* @param filename Filepath and name of texture to use. Loads with VContent in VGlobal.
	*/
	VBackdrop(float scrollX, float scrollY, bool repeatX, bool repeatY, const sf::String& filename = "") : VObject()
	{
		if (filename != "")
			LoadGraphic(filename);

		vertices.setPrimitiveType(sf::Quads);
		vertices.resize(4);

		Size.x = 0;
		Size.y = 0;

		Scale.x = 1;
		Scale.y = 1;

		RepeatX = repeatX;
		RepeatY = repeatY;

		ScrollFactor.x = scrollX;
		ScrollFactor.y = scrollY;

		ScrollMaxVelocity.x = 10000.0f;
		ScrollMaxVelocity.y = 10000.0f;
	}

	///Destroy the VBackdrop.
	virtual void Destroy();
	
	///@param filename Filepath and name of texture to use. Loads with VContent in VGlobal.
	VBackdrop* LoadGraphic(sf::String filename, const sf::IntRect& area = sf::IntRect());
	///@param texture sf::Texture object to use.
	VBackdrop* LoadGraphicFromTexture(sf::Texture& texture);

	/**
	* Sets the colour tint of the backdrop.
	* @param tint The colour tint you want to set the backdrop to.
	*/
	void SetTint(const sf::Color& tint);
	///@return The current colour tint as an sf::Color object.
	sf::Color& GetTint();

	/**
	* Updates backdrop.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) override;

	/**
	* Renders backdrop.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget &RenderTarget) override;
};

