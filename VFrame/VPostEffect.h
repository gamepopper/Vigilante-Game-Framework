#pragma once
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Glsl.hpp>
#include <vector>

class VPostEffectBase
{
protected:
	static void applyShader(const sf::Shader& shader, sf::RenderTarget& output);
	static void passThrough(const sf::RenderTexture& input, sf::RenderTarget& output);

public:
	VPostEffectBase() = default;
	virtual ~VPostEffectBase() = default;

	//Check if Shaders are supported on specific platform, shaders won't apply automatically if not.
	static bool isSupported();
	//Apply post effect from an input RenderTexture to an output RenderTarget (which may be a RenderTexture or Window).
	virtual void Apply(const sf::RenderTexture& input, sf::RenderTarget& output) = 0;
	virtual void Update(float dt) {};
};

//Special class that allows developers to apply multiple Post Effects, and enable/disable specific ones at runtime.
class VPostEffectMultipass : public VPostEffectBase
{
protected:
	std::vector<VPostEffectBase*>	postEffects;
	std::vector<bool>				enabled;
	std::vector<sf::RenderTexture*>	renderTextures;
	unsigned int maxSize = 0;

public:
	VPostEffectMultipass(int MaxSize);
	virtual ~VPostEffectMultipass();

	//Add a post effect.
	void AddPostEffect(VPostEffectBase* post);
	//Get post effect at index.
	VPostEffectBase* GetPostEffect(unsigned int index);
	//Enable/disable effect at index.
	void EnableEffect(unsigned int index, bool enable);

	virtual void Apply(const sf::RenderTexture& input, sf::RenderTarget& output);
	virtual void Update(float dt);
};

//Basic PostEffect that uses a single shader object.
class VPostEffect : public VPostEffectBase
{
	sf::Shader shader;
	sf::String sourceTextureName = "texture";

public:
	VPostEffect() = default;
	VPostEffect(sf::String filename, sf::Shader::Type type)
	{
		shader.loadFromFile(filename, type);
	}
	VPostEffect(sf::String vertFile, sf::String fragFile)
	{
		shader.loadFromFile(vertFile, fragFile);
	}
	~VPostEffect() = default;

	//Set name for the Input Texture.
	void SetInputTextureName(sf::String name)
	{
		sourceTextureName = name;
	}

	//Sets Texture Type Parameter
	void SetParameter(sf::String paramName, sf::Shader::CurrentTextureType currentTextureType)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, currentTextureType);
	}

	//Sets Texture Parameter
	void SetParameter(sf::String paramName, const sf::Texture& texture)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, texture);
	}

	//Sets Transform Parameter
	void SetParameter(sf::String paramName, const sf::Transform& transform)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, (sf::Glsl::Mat4)transform);
	}

	//Sets Colour (Vec4) Parameter
	void SetParameter(sf::String paramName, const sf::Color& colour)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, (sf::Glsl::Vec4)colour);
	}

	//Sets Vec3 Parameter
	void SetParameter(sf::String paramName, const sf::Vector3f& vec3)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, vec3);
	}

	//Sets Vec2 Parameter
	void SetParameter(sf::String paramName, const sf::Vector2f& vec2)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, vec2);
	}

	//Sets Vec4 Parameter
	void SetParameter(sf::String paramName, float x, float y, float z, float w)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, sf::Glsl::Vec4(x, y, z, w));
	}

	//Sets Vec3 Parameter
	void SetParameter(sf::String paramName, float x, float y, float z)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, sf::Glsl::Vec3(x, y, z));
	}

	//Sets Vec2 Parameter
	void SetParameter(sf::String paramName, float x, float y)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, sf::Glsl::Vec2(x, y));
	}

	//Sets float Parameter
	void SetParameter(sf::String paramName, float x)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, x);
	}

	virtual void Apply(const sf::RenderTexture& input, sf::RenderTarget& output);
};

