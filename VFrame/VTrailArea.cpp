#include "VTrailArea.h"
#include "VGlobal.h"

VTrailArea::VTrailArea(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize) : VRenderGroup(x, y, width, height, maxSize)
{
	trailTex.create(width, height);
	trailTex2.create(width, height);
}

VTrailArea::VTrailArea(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize) : VRenderGroup(position, size, maxSize)
{
	trailTex.create(size.x, size.y);
	trailTex2.create(size.x, size.y);
}

void VTrailArea::Update(float dt)
{
	VSUPERCLASS::Update(dt);

	if (swapTimer.getElapsedTime().asSeconds() > Delay)
	{
		//renderTex.clear(sf::Color::Transparent);
		Sprite->RenderState.blendMode = sf::BlendAdd;
		VGroup::Draw(renderTex);
		renderTex.display();

		trailTex.clear(sf::Color::Transparent);
		trailSprite.setTexture(renderTex.getTexture());
		trailSprite.setColor(sf::Color::White);
		trailTex.draw(trailSprite);
		trailTex.display();

		swapTimer.restart();
	}

	//Ensures fading happens if dt is much smaller than the smallest possible value to multiply
	float time = fadeTimer.getElapsedTime().asSeconds();
	if (time > 0.0039f)
	{
		sf::View renderTexView = renderTex.getView();
		renderTex.setView(renderTex.getDefaultView());
		trailSprite.setTexture(trailTex.getTexture());
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
		renderTex.display();
		renderTex.setView(renderTexView);
		fadeTimer.restart();
	}
}

void VTrailArea::Draw(sf::RenderTarget& RenderTarget)
{	
	updateTexture(renderTex.getTexture());
	Sprite->Draw(RenderTarget);

	VGroup::Draw(RenderTarget);
}