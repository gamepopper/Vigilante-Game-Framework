#pragma once
#include <SFML\Graphics\RenderTexture.hpp>
#include <SFML\Graphics\Shader.hpp>
#include <SFML\System\String.hpp>
#include <SFML\Graphics\Glsl.hpp>
#include <vector>

class VPostEffectBase
{
protected:
	static void applyShader(const sf::Shader& shader, sf::RenderTarget& output);
	static void passThrough(const sf::RenderTexture& input, sf::RenderTarget& output);

public:
	VPostEffectBase() = default;
	virtual ~VPostEffectBase() = default;

	static bool isSupported();
	virtual void Apply(const sf::RenderTexture& input, sf::RenderTarget& output) = 0;
	virtual void Update(float dt) {};
};

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

	void AddPostEffect(VPostEffectBase* post);
	VPostEffectBase* GetPostEffect(unsigned int index);
	void EnableEffect(unsigned int index, bool enable);

	virtual void Apply(const sf::RenderTexture& input, sf::RenderTarget& output);
	virtual void Update(float dt);
};

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

	void SetInputTextureName(sf::String name)
	{
		sourceTextureName = name;
	}

	void SetParameter(sf::String paramName, sf::Shader::CurrentTextureType currentTextureType)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, currentTextureType);
	}

	void SetParameter(sf::String paramName, const sf::Texture& texture)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, texture);
	}

	void SetParameter(sf::String paramName, const sf::Transform& transform)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, (sf::Glsl::Mat4)transform);
	}

	void SetParameter(sf::String paramName, const sf::Color& colour)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, (sf::Glsl::Vec4)colour);
	}

	void SetParameter(sf::String paramName, sf::Vector3f& vec3)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, vec3);
	}

	void SetParameter(sf::String paramName, sf::Vector2f& vec2)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, vec2);
	}

	void SetParameter(sf::String paramName, float x, float y, float z, float w)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, sf::Glsl::Vec4(x, y, z, w));
	}

	void SetParameter(sf::String paramName, float x, float y, float z)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, sf::Glsl::Vec3(x, y, z));
	}

	void SetParameter(sf::String paramName, float x, float y)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, sf::Glsl::Vec2(x, y));
	}

	void SetParameter(sf::String paramName, float x)
	{
		if (paramName != sourceTextureName)
			shader.setUniform(paramName, x);
	}

	static bool isSupported();
	virtual void Apply(const sf::RenderTexture& input, sf::RenderTarget& output);
};

