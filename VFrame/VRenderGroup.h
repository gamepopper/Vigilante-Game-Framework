/**
* @file    VRenderGroup.h
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
* Similar to VGroup, except all objects are rendered onto a VSprite that can freely be transformed.
*/

#pragma once
#include "VGroup.h"
#include "VSprite.h"
#include "VPostEffect.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

///Processes a group of objects and renders them all to a sprite object.
class VRenderGroup : public VGroup
{
protected:
	///The sf::RenderTexture to render all the objects to.
	sf::RenderTexture renderTex;
	///Another sf::RenderTexture for outputting the post process effect onto.
	sf::RenderTexture postProcessTex;
	///Updates the transform.
	void updateTransform();
	///Update the texture of the sprite.
	void updateTexture(const sf::Texture& texture);

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VGroup VSUPERCLASS;

	///The sprite that the group will render to. Refer to this if you want to transform the render output.
	VSprite* Sprite = nullptr;
	///The background colour the sprite will have.
	sf::Color BackgroundTint = sf::Color::Transparent;
	///Post Process (Shader) Effect to apply to the sprite.
	VPostEffectBase* PostEffect = nullptr;
	///If true, the sprite's render view will be transformed based on the sprite's current transform, else it will use the default view.
	bool RenderViewTransform = true;
	///If true, the content data would also be rendered outside of the sprite area. Used if you want an effect to apply to a specific area.
	bool RenderOutside = false;

	/**
	* @param x X position of the sprite.
	* @param y Y position of the sprite
	* @param width Width of the sprite (also sets the width of the render area).
	* @param height Height of the sprite (also sets the height of the render area).
	* @param maxSize The fixed length of the group. If 0, then the VGroup has no limit in size.
	*/
	VRenderGroup(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize = 0);

	/**
	* @param position Position of the sprite.
	* @param size Size of the sprite (also sets the size of the render area).
	* @param maxSize The fixed length of the group. If 0, then the VGroup has no limit in size.
	*/
	VRenderGroup(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize = 0);

	/**
	* Resizes sprite and render area of the group.
	* @param width Width of the sprite and texture area.
	* @param height Height of the sprite and texture area.
	*/
	virtual void Resize(int width, int height);

	///@return The sprite's texture after the scene is rendered to it.
	virtual const sf::Texture& GetTexture();

	///Removes and/or destroys all objects inside the group and sprite before destroying extra data. (If an object is referenced in another VGroup, the object won't be destroyed)
	virtual void Destroy() override;
	///Kills all living objects before killing the group and the sprite themselves.
	virtual void Kill() override;
	///Revives all dead objects before reviving the group and the sprite themselves.
	virtual void Revive() override;

	/**
	* Updates all alive and active objects in the group.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) override;

	/**
	* Renders all alive and visible objects in the group.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};