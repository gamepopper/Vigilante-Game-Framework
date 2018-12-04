#include "VTrailArea.h"
#include "VGlobal.h"

VTrailArea::VTrailArea(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize) : VRenderGroup(x, y, width, height, maxSize)
{
	
}

VTrailArea::VTrailArea(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize) : VRenderGroup(position, size, maxSize)
{
	
}

void VTrailArea::Update(float dt)
{
	VSUPERCLASS::Update(dt);
}

void VTrailArea::Draw(sf::RenderTarget& RenderTarget)
{
	float time = fadeTimer.getElapsedTime().asSeconds();
	if (time > Delay)
	{
		sf::View renderTexView = renderTex.getView();
		renderTex.setView(renderTex.getDefaultView());
		trailTexture.loadFromImage(renderTex.getTexture().copyToImage());
		trailSprite.setTexture(trailTexture);
		trailSprite.setPosition(0.0f, 0.0f);
		trailSprite.setColor(
			sf::Color(
				255 - sf::Uint8(255 * RedMultiplier   * time),
				255 - sf::Uint8(255 * GreenMultiplier * time),
				255 - sf::Uint8(255 * BlueMultiplier  * time),
				255 - sf::Uint8(255 * AlphaMultiplier * time)
			)
		);
		renderTex.clear(sf::Color::Transparent);
		renderTex.draw(trailSprite);
		renderTex.setView(renderTexView);
		VGroup::Draw(renderTex);
		renderTex.display();
		fadeTimer.restart();
	}

	if (PostEffect != nullptr && VPostEffectBase::isSupported())
	{
		if (postProcessTex.getSize().x == 0 || postProcessTex.getSize().y == 0)
		{
			postProcessTex.create(renderTex.getSize().x, renderTex.getSize().y);
		}

		postProcessTex.clear(sf::Color::Transparent);
		PostEffect->Apply(renderTex.getTexture(), postProcessTex);
		postProcessTex.display();

		updateTexture(postProcessTex.getTexture());
	}
	else
	{
		updateTexture(renderTex.getTexture());
	}

	Sprite->RenderState.blendMode = sf::BlendAdd;
	Sprite->Draw(RenderTarget);

	if (RenderOutside)
		VGroup::Draw(RenderTarget);
}