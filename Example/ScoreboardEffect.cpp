#include "ScoreboardEffect.h"
#include "../VFrame/VGlobal.h"

ScoreboardEffect::ScoreboardEffect()
{
	downSample.loadFromFile("Example/Assets/FullPass.vert", "Example/Assets/DownSample.frag");
	maskSample.loadFromFile("Example/Assets/FullPass.vert", "Example/Assets/CircleMask.frag");
	gaussianBlur.loadFromFile("Example/Assets/FullPass.vert", "Example/Assets/GaussianBlur.frag");

	SetPointSize(1.0f);
	SetPointAmount(1);
	SetBlur(false, 1.0f);
}

void ScoreboardEffect::Apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
	prepareTextures(input.getSize());

	downsample(input, mDownSampleTexture);

	if (blurEffect)
	{
		mask(mDownSampleTexture, mBlurPassTextures[0]);
		blurMultipass(mBlurPassTextures);
		passThrough(mBlurPassTextures[0], output);
		mBlurPassTextures[0].clear(sf::Color::Transparent);
		mBlurPassTextures[1].clear(sf::Color::Transparent);
	}
	else
	{
		mask(mDownSampleTexture, output);
	}
}

void ScoreboardEffect::SetPointSize(float size)
{
	maskSample.setUniform("circleRadius", size / 2.0f);
}

void ScoreboardEffect::SetPointAmount(unsigned int amount)
{
	pointAmount = amount;
	maskSample.setUniform("multiplier", (float)amount);
}

void ScoreboardEffect::SetBlur(bool value, float amount)
{
	blurEffect = value;
	blurAmount = amount;
}

void ScoreboardEffect::prepareTextures(sf::Vector2u size)
{
	float divider = std::fminf((float)size.x, (float)size.y) / pointAmount;
	sf::Vector2u texSize = sf::Vector2u(sf::Vector2f(size) / divider);
	sf::Vector2u blurSize = sf::Vector2u(sf::Vector2f(size) / blurAmount);

	if (mDownSampleTexture.getSize() != texSize || mBlurPassTextures[0].getSize() != blurSize)
	{
		mDownSampleTexture.create(texSize.x, texSize.y);
		
		mBlurPassTextures[0].create(blurSize.x, blurSize.y);
		mBlurPassTextures[0].setSmooth(true);
		mBlurPassTextures[1].create(blurSize.x, blurSize.y);
		mBlurPassTextures[1].setSmooth(true);
	}
}

void ScoreboardEffect::downsample(const sf::RenderTexture& input, sf::RenderTexture& output)
{
	downSample.setUniform("texture", input.getTexture());
	downSample.setUniform("textureSize", sf::Vector2f(input.getSize()));
	applyShader(downSample, output);
	output.display();
}

void ScoreboardEffect::mask(const sf::RenderTexture& source, sf::RenderTarget& target)
{
	maskSample.setUniform("texture", source.getTexture());
	maskSample.setUniform("screenDim", sf::Vector2f((float)VGlobal::p()->Width, (float)VGlobal::p()->Height));
	applyShader(maskSample, target);
}

void ScoreboardEffect::blurMultipass(RenderArray& renderTextures)
{
	sf::Vector2u textureSize = renderTextures[0].getSize();

	for (std::size_t count = 0; count < 2; ++count)
	{
		blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
		blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
	}
}

void ScoreboardEffect::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor)
{
	gaussianBlur.setUniform("texture", input.getTexture());
	gaussianBlur.setUniform("offsetFactor", offsetFactor);
	applyShader(gaussianBlur, output);
	output.display();
}