/**
* @file    VPostEffect.h
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
* Classes that handle shader effects for either VRenderGroups, VRenderLayers or entire scenes in VGame.
*/

#pragma once
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include <vector>
#include <memory>

///Base class for Post Processing Effects.
class VPostEffectBase
{
protected:
	/**
	* Applies a shader to a render target.
	* @param shader The sf::Shader object containing the vertex, geometry anf/or fragment shader to use.
	* @param output The render target (Whether it's a RenderTexture or RenderWindow) to apply the shader to.
	*/
	static void applyShader(const sf::Shader& shader, sf::RenderTarget& output);

	/**
	* Simply renders an input texture onto a render target directly without any alterations.
	* @param input The texture to pass through.
	* @param output The render target (Whether it's a RenderTexture or RenderWindow) to apply the shader to.
	*/
	static void passThrough(const sf::Texture& input, sf::RenderTarget& output);

public:
	VPostEffectBase() = default;
	virtual ~VPostEffectBase() = default;

	///@return True if Shaders are supported on specific platform, shaders won't apply automatically if not.
	static bool isSupported();

	/**
	* Apply post effect from an input RenderTexture to an output RenderTarget (which may be a RenderTexture or Window). (Must be overwritten)
	* @param input The texture to pass through.
	* @param output The render target (Whether it's a RenderTexture or RenderWindow) to apply the shader to.
	*/
	virtual void Apply(const sf::Texture& input, sf::RenderTarget& output) = 0;

	/**
	* Updates post effect.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) {};
};

///Special class that allows the application of multiple Post Effects, and enable/disable specific ones at runtime.
class VPostEffectMultipass : public VPostEffectBase
{
protected:
	///List of post process effects.
	std::vector<VPostEffectBase*>	postEffects;
	///List to determine which post effects are enabled.
	std::vector<bool>				enabled;
	///List of render textures to apply post effects to.
	std::vector<std::unique_ptr<sf::RenderTexture>>	renderTextures;
	///The fixed size of post process effects.
	unsigned int maxSize = 0;

public:
	///@param MaxSize The fixed amount of shaders the multipass can have.
	VPostEffectMultipass(int MaxSize);
	virtual ~VPostEffectMultipass();

	///@param post The Post Effect to add.
	void AddPostEffect(VPostEffectBase* post);
	///@param index The index of the post effect list to remove.
	void RemovePostEffect(unsigned int index);

	/**
	* @param index The index of the post effect list to remove.
	* @return If index is valid, then returns the post effect.
	*/
	VPostEffectBase* GetPostEffect(unsigned int index);

	/**
	* Enable/disable effect at index.
	* @param index The index of the post effect list to remove.
	* @param enable Enables the post effect if true, else disable it.
	*/
	void EnableEffect(unsigned int index, bool enable);
	
	/**
	* Apply post effect from an input RenderTexture to an output RenderTarget (which may be a RenderTexture or Window).
	* @param input The texture to pass through.
	* @param output The render target (Whether it's a RenderTexture or RenderWindow) to apply the shader to.
	*/
	virtual void Apply(const sf::Texture& input, sf::RenderTarget& output);

	/**
	* Updates all post effects currently enabled.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt);
};

///A basic Post Processing effect that uses a single shader object and allows setting different kinds of Uniform parameters.
class VPostEffect : public VPostEffectBase
{
private:
	sf::Shader shader;
	sf::String sourceTextureName = "texture";

public:
	VPostEffect() = default;

	/**
	* @param filename File path and name of shader file.
	* @param type The Type of shader.
	*/
	VPostEffect(sf::String filename, sf::Shader::Type type)
	{
		shader.loadFromFile(filename, type);
	}

	/**
	* @param vertFile File path and name of the vertex shader file.
	* @param fragFile File path and name of the fragment shader file.
	*/
	VPostEffect(sf::String vertFile, sf::String fragFile)
	{
		shader.loadFromFile(vertFile, fragFile);
	}

	/**
	* @param vertFile File path and name of the vertex shader file.
	* @param geomFile File path and name of the geometry shader file.
	* @param fragFile File path and name of the fragment shader file.
	*/
	VPostEffect(sf::String vertFile, sf::String geomFile, sf::String fragFile)
	{
		shader.loadFromFile(vertFile, geomFile, fragFile);
	}
	~VPostEffect() = default;

	///@param name The name for the Input Texture (the default texture assuming no texture is set to the Uniform variable).
	void SetInputTextureName(sf::String name)
	{
		sourceTextureName = name;
	}

	/**
	* Sets Texture Type Parameter.
	* @param paramName The name of the uniform parameter.
	* @param currentTextureType Special type that represents the texture of the object being drawn.
	*/
	void SetParameter(sf::String paramName, sf::Shader::CurrentTextureType currentTextureType)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, currentTextureType);
	}

	/**
	* Sets Texture Parameter.
	* @param paramName The name of the uniform parameter.
	* @param texture The texture to use for texture.
	*/
	void SetParameter(sf::String paramName, const sf::Texture& texture)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, texture);
	}

	/**
	* Sets Transform Parameter.
	* @param paramName The name of the uniform parameter.
	* @param transform The transform matrix to set.
	*/
	void SetParameter(sf::String paramName, const sf::Transform& transform)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, (sf::Glsl::Mat4)transform);
	}

	/**
	* Sets Colour (Vec4) Parameter
	* @param paramName The name of the uniform parameter.
	* @param colour The colour that will be converted to Vec4.
	*/
	void SetParameter(sf::String paramName, const sf::Color& colour)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, (sf::Glsl::Vec4)colour);
	}

	/**
	* Sets Vec3 Parameter
	* @param paramName The name of the uniform parameter.
	* @param vec3 The Vector3f that will be converted to Vec3.
	*/
	void SetParameter(sf::String paramName, const sf::Vector3f& vec3)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, vec3);
	}

	/**
	* Sets Vec2 Parameter
	* @param paramName The name of the uniform parameter.
	* @param vec2 The Vector3f that will be converted to Vec2.
	*/
	void SetParameter(sf::String paramName, const sf::Vector2f& vec2)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, vec2);
	}

	/**
	* Sets Vec4 Parameter from individual floats.
	* @param paramName The name of the uniform parameter.
	* @param x X coordinate.
	* @param y Y coordinate.
	* @param z Z coordinate.
	* @param w W coordinate.
	*/
	void SetParameter(sf::String paramName, float x, float y, float z, float w)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, sf::Glsl::Vec4(x, y, z, w));
	}

	/**
	* Sets Vec3 Parameter from individual floats.
	* @param paramName The name of the uniform parameter.
	* @param x X coordinate.
	* @param y Y coordinate.
	* @param z Z coordinate.
	*/
	void SetParameter(sf::String paramName, float x, float y, float z)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, sf::Glsl::Vec3(x, y, z));
	}

	/**
	* Sets Vec2 Parameter from individual floats.
	* @param paramName The name of the uniform parameter.
	* @param x X coordinate.
	* @param y Y coordinate.
	*/
	void SetParameter(sf::String paramName, float x, float y)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, sf::Glsl::Vec2(x, y));
	}

	/**
	* Sets float parameter.
	* @param paramName The name of the uniform parameter.
	* @param x Float value.
	*/
	void SetParameter(sf::String paramName, float x)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, x);
	}

	/**
	* Apply post effect from an input RenderTexture to an output RenderTarget (which may be a RenderTexture or Window).
	* @param input The texture to pass through.
	* @param output The render target (Whether it's a RenderTexture or RenderWindow) to apply the shader to.
	*/
	virtual void Apply(const sf::Texture& input, sf::RenderTarget& output);
};

