#include "VTrailArea.h"
#include "VGlobal.h"
#include "VTimer.h"

VTrailArea::VTrailArea(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize) : VRenderGroup(x, y, width, height, maxSize), trailSprite(trailTexture)
{
	renderTex.clear(sf::Color::Transparent);
	timer = new VTimer();
}

VTrailArea::VTrailArea(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize) : VRenderGroup(position, size, maxSize), trailSprite(trailTexture)
{
	renderTex.clear(sf::Color::Transparent);
	timer = new VTimer();
}

void VTrailArea::Destroy()
{
	VSUPERCLASS::Destroy();
	delete timer;
}

void VTrailArea::Update(float dt)
{
	VSUPERCLASS::Update(dt);
	timer->Update(dt);
}

void VTrailArea::Draw(sf::RenderTarget& RenderTarget)
{
	updateTransform();

#ifdef VFRAME_DEBUGDRAW
	bool drawDebug = VGlobal::p()->DrawDebug;
	VGlobal::p()->DrawDebug = false;
#endif

	float time = timer->Seconds();
	if (time > Delay)
	{
		sf::View renderTexView = renderTex.getView();
		renderTex.setView(renderTex.getDefaultView());
		trailTexture = sf::Texture(renderTex.getTexture());
		trailSprite.setTexture(trailTexture, true);
		trailSprite.setPosition({ lastPos.x - Sprite->Position.x, lastPos.y - Sprite->Position.y });
		trailSprite.setColor(
			sf::Color(
				255 - uint8_t(255 * RedMultiplier	* AlphaMultiplier   * time),
				255 - uint8_t(255 * GreenMultiplier	* AlphaMultiplier	* time),
				255 - uint8_t(255 * BlueMultiplier	* AlphaMultiplier	* time),
				255 - uint8_t(255 * AlphaMultiplier	* time)
			)
		);
		renderTex.clear(sf::Color::Transparent);
		renderTex.draw(trailSprite);
		renderTex.setView(renderTexView);
		VGroup::Draw(renderTex);
		renderTex.display();
		timer->Restart();

		lastPos = Sprite->Position;
	}
	else
	{
		sf::View renderTexView = renderTex.getView();
		renderTex.setView(renderTex.getDefaultView());
		trailTexture = sf::Texture(renderTex.getTexture());
		trailSprite.setTexture(trailTexture, true);
		trailSprite.setPosition({ lastPos.x - Sprite->Position.x, lastPos.y - Sprite->Position.y });
		trailSprite.setColor(sf::Color::White);
		renderTex.clear(sf::Color::Transparent);
		renderTex.draw(trailSprite);
		renderTex.setView(renderTexView);
		VGroup::Draw(renderTex);
		renderTex.display();

		lastPos = Sprite->Position;
	}

	if (PostEffect != nullptr && VPostEffectBase::isSupported())
	{
		if (postProcessTex.getSize().x == 0 || postProcessTex.getSize().y == 0)
		{
			if (!postProcessTex.resize(renderTex.getSize()))
			{
				VLogError("Invalid Texture Size to resize post processing texture in TrailArea");
			}
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

#ifdef VFRAME_DEBUGDRAW
	VGlobal::p()->DrawDebug = drawDebug;
#endif

	Sprite->Draw(RenderTarget);
	Camera->Render(RenderTarget);

	if (RenderOutside != VRENDERGROUP_NO)
		VGroup::Draw(RenderTarget);
}