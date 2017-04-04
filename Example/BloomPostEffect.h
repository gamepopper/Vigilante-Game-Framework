#pragma once
#include "..\VFrame\VPostEffect.h"
#include <array>

class BloomPostEffect : public VPostEffectBase
{
public:
	BloomPostEffect();
	~BloomPostEffect() = default;

	virtual void Apply(const sf::RenderTexture& input, sf::RenderTarget& output);
	void SetBloomFactor(float factor);

private:
	typedef std::array<sf::RenderTexture, 2> RenderArray;

	void	prepareTextures(sf::Vector2u size);

	void	filterBright(const sf::RenderTexture& input, sf::RenderTexture& output);
	void	blurMultipass(RenderArray& renderTextures);
	void	blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor);
	void	downsample(const sf::RenderTexture& input, sf::RenderTexture& output);
	void	add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& target);

	sf::Shader brightness;
	sf::Shader downSample;
	sf::Shader gaussianBlur;
	sf::Shader addition;

	sf::RenderTexture mBrightnessTexture;
	RenderArray	mFirstPassTextures;
	RenderArray	mSecondPassTextures;
};

