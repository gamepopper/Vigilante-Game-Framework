/**
* @file    V3DScene.h
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
* VRenderGroup that handles 3D rendering.
*/

#pragma once
#ifndef VFRAME_NO_3D
#include "VRenderGroup.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include "V3DCamera.h"
#include <array>

class V3DShader;
///A special VRenderGroup for Rendering 3D scenes.
class V3DScene : public VRenderGroup
{
private:
	sf::ContextSettings contextSettings;
	void RenderGroup(VGroup* group);

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VRenderGroup VSUPERCLASS;

	///Current Camera Index for rendering.
	unsigned int CurrentCamera = 0;

	/**
	* @param x X position of the sprite.
	* @param y Y position of the sprite
	* @param width Width of the sprite (also sets the width of the render area).
	* @param height Height of the sprite (also sets the height of the render area).
	* @param settings The settings of the OpenGL Context that the sf::RenderTexture will have.
	* @param maxSize The fixed length of the group. If 0, then the VGroup has no limit in size.
	*/
	V3DScene(float x, float y, unsigned int width, unsigned int height, const sf::ContextSettings& settings = sf::ContextSettings(), unsigned int maxSize = 0);
	
	/**
	* @param position Position of the sprite.
	* @param size Size of the sprite (also sets the size of the render area).
	* @param settings The settings of the OpenGL Context that the sf::RenderTexture will have.
	* @param maxSize The fixed length of the group. If 0, then the VGroup has no limit in size.
	*/
	V3DScene(sf::Vector2f position, sf::Vector2u size, const sf::ContextSettings& settings = sf::ContextSettings(), unsigned int maxSize = 0);

	///OpenGL Shader for the entire scene.
	std::unique_ptr<V3DShader> Shader;
	///Main 3D Camera for this scene.
	std::vector<std::unique_ptr<V3DCamera>> Camera;

	///@return The rendered scene as a texture.
	virtual const sf::Texture& GetTexture() override;

	/**
	* Renders the scene 
	* @param shader The scene shader.
	* @param camera The scene camera.
	* @return The rendered scene as a texture.
	*/
	const sf::Texture GetTexture(V3DShader* shader, V3DCamera* camera);

	/**
	* Resizes sprite and render area of the group.
	* @param width Width of the sprite and texture area.
	* @param height Height of the sprite and texture area.
	*/
	virtual void Resize(int width, int height) override;

	///@param value If true, sets the scene to be the current active context.
	void SetActive(bool value = true);
	///Push all the GL States up the stack to preserve them from any changes.
	void PushGLStates();
	///Pop all the GL States off the stack to restore them.
	void PopGLStates();
	///Reset all the GL States.
	void ResetGLStates();

	virtual void Destroy() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};
#endif