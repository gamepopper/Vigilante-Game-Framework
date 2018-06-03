#pragma once
#include "../VFrame/VPostEffect.h"
#include <array>

class BloomPostEffect : public VPostEffectBase
{
public:
	BloomPostEffect();
	~BloomPostEffect() = default;

	virtual void Apply(const sf::Texture& input, sf::RenderTarget& output);
	void SetBloomFactor(float factor);

private:
	typedef std::array<sf::RenderTexture, 2> RenderArray;

	void	prepareTextures(sf::Vector2u size);

	void	filterBright(const sf::Texture& input, sf::RenderTarget& output);
	void	blurMultipass(RenderArray& renderTextures);
	void	blur(const sf::Texture& input, sf::RenderTarget& output, sf::Vector2f offsetFactor);
	void	downsample(const sf::Texture& input, sf::RenderTarget& output);
	void	add(const sf::Texture& source, const sf::Texture& bloom, sf::RenderTarget& target);

	sf::Shader brightness;
	sf::Shader downSample;
	sf::Shader gaussianBlur;
	sf::Shader addition;

	sf::RenderTexture mBrightnessTexture;
	RenderArray	mFirstPassTextures;
	RenderArray	mSecondPassTextures;
};

