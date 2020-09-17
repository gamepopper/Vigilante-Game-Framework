#include "VRenderLayer.h"
#include "VGlobal.h"

VRenderLayer::VRenderLayer(unsigned int maxSize) : VGroup(maxSize)
{
	
}

void VRenderLayer::Destroy()
{
	VSUPERCLASS::Destroy();
	PostEffect = nullptr;
}

void VRenderLayer::SetTint(const sf::Color& tint)
{
	sprite.setColor(tint);
}

const sf::Color& VRenderLayer::GetTint()
{
	return sprite.getColor();
}

void VRenderLayer::Update(float dt)
{
	VSUPERCLASS::Update(dt);

	if (PostEffect)
	{
		PostEffect->Update(dt);
	}
}

const sf::Texture& VRenderLayer::GetTexture()
{
	renderTex.clear(sf::Color::Transparent);
	VSUPERCLASS::Draw(renderTex);
	renderTex.display();

	return renderTex.getTexture();
}

void VRenderLayer::Draw(sf::RenderTarget& RenderTarget)
{
	sf::View MainView = RenderTarget.getView();

	if (renderTex.getSize() != sf::Vector2u(MainView.getSize()))
		renderTex.create((unsigned int)MainView.getSize().x, (unsigned int)MainView.getSize().y);

	renderTex.setView(MainView);
	renderTex.clear(sf::Color::Transparent);
	VSUPERCLASS::Draw(renderTex);
	renderTex.display();

	RenderTarget.setView(RenderTarget.getDefaultView());

	if (PostEffect == nullptr || !VPostEffectBase::isSupported())
	{
		sprite.setTexture(renderTex.getTexture());
	}
	else
	{
		postProcessTex.create(renderTex.getSize().x, renderTex.getSize().y);

		postProcessTex.clear(sf::Color::Transparent);
		PostEffect->Apply(renderTex.getTexture(), postProcessTex);
		postProcessTex.display();

		sprite.setTexture(postProcessTex.getTexture());
	}
	
	RenderTarget.draw(sprite, RenderState);
	RenderTarget.setView(MainView);
}
