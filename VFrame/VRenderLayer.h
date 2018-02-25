/**
* @file    VRenderLayer.h
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
* Similar to VGroup, except all objects are rendered onto an sf::Sprite that renders to the entire screen.
*/

#pragma once
#include "VGroup.h"
#include "VPostEffect.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

///Processes group of objects to a sprite. Unlike the VRenderGroup, the sprite cannot be moved and will always render to the size of the screen.
class VRenderLayer : public VGroup
{
protected:
	///The render and transform infomation of the layer.
	sf::Sprite sprite;
	///The sf::RenderTexture to render all the objects to.
	sf::RenderTexture renderTex;
	///Another sf::RenderTexture for outputting the post process effect onto.
	sf::RenderTexture postProcessTex;

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VGroup VSUPERCLASS;

	///RenderState for the layer.
	sf::RenderStates RenderState;
	///Post Effect to apply to the layer.
	VPostEffectBase* PostEffect = nullptr;

	///@param maxSize The fixed size of the layer. 0 assumes the size is not fixed.
	VRenderLayer(unsigned int maxSize = 0);

	///@return The sprite's texture after the scene is rendered to it.
	virtual const sf::Texture& GetTexture();

	///Removes and/or destroys all objects inside the group and sprite before destroying extra data. (If an object is referenced in another VGroup, the object won't be destroyed)
	virtual void Destroy() override;

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

