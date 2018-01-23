#include "BloomPostEffect.h"

BloomPostEffect::BloomPostEffect()
{
	brightness.loadFromFile("Example/Assets/Brightness.frag", sf::Shader::Fragment);
	downSample.loadFromFile("Example/Assets/DownSample.frag", sf::Shader::Fragment);
	gaussianBlur.loadFromFile("Example/Assets/GaussianBlur.frag", sf::Shader::Fragment);
	addition.loadFromFile("Example/Assets/Add.frag", sf::Shader::Fragment);

	SetBloomFactor(1.2f);
}

void BloomPostEffect::Apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
	prepareTextures(input.getSize());

	filterBright(input, mBrightnessTexture);

	downsample(mBrightnessTexture, mFirstPassTextures[0]);
	blurMultipass(mFirstPassTextures);

	downsample(mFirstPassTextures[0], mSecondPassTextures[0]);
	blurMultipass(mSecondPassTextures);

	add(mFirstPassTextures[0], mSecondPassTextures[0], mFirstPassTextures[1]);
	mFirstPassTextures[1].display();
	add(input, mFirstPassTextures[1], output);
}

void BloomPostEffect::SetBloomFactor(float factor)
{
	brightness.setUniform("Factor", factor);
}

void BloomPostEffect::prepareTextures(sf::Vector2u size)
{
	if (mBrightnessTexture.getSize() != size)
	{
		mBrightnessTexture.create(size.x, size.y);
		mBrightnessTexture.setSmooth(true);

		mFirstPassTextures[0].create(size.x / 2, size.y / 2);
		mFirstPassTextures[0].setSmooth(true);
		mFirstPassTextures[1].create(size.x / 2, size.y / 2);
		mFirstPassTextures[1].setSmooth(true);

		mSecondPassTextures[0].create(size.x / 4, size.y / 4);
		mSecondPassTextures[0].setSmooth(true);
		mSecondPassTextures[1].create(size.x / 4, size.y / 4);
		mSecondPassTextures[1].setSmooth(true);
	}
}

void BloomPostEffect::filterBright(const sf::RenderTexture& input, sf::RenderTexture& output)
{
	brightness.setUniform("texture", input.getTexture());
	applyShader(brightness, output);
	output.display();
}

void BloomPostEffect::blurMultipass(RenderArray& renderTextures)
{
	sf::Vector2u textureSize = renderTextures[0].getSize();

	for (std::size_t count = 0; count < 2; ++count)
	{
		blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
		blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
	}
}

void BloomPostEffect::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor)
{
	gaussianBlur.setUniform("texture", input.getTexture());
	gaussianBlur.setUniform("offsetFactor", offsetFactor);
	applyShader(gaussianBlur, output);
	output.display();
}

void BloomPostEffect::downsample(const sf::RenderTexture& input, sf::RenderTexture& output)
{
	downSample.setUniform("texture", input.getTexture());
	downSample.setUniform("textureSize", sf::Vector2f(input.getSize()));
	applyShader(downSample, output);
	output.display();
}

void BloomPostEffect::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output)
{
	addition.setUniform("texture", source.getTexture());
	addition.setUniform("other", bloom.getTexture());
	applyShader(addition, output);
}
