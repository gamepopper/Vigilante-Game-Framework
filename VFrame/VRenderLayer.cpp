#include "VRenderLayer.h"
#include "VGlobal.h"

VRenderLayer::VRenderLayer(unsigned int maxSize) : VGroup(maxSize)
{
	renderTex.create(VGlobal::p()->Width, VGlobal::p()->Height);
	postProcessTex.create(VGlobal::p()->Width, VGlobal::p()->Height);
}

void VRenderLayer::Destroy()
{
	VSUPERCLASS::Destroy();

	if (PostEffect != nullptr)
	{
		delete PostEffect;
		PostEffect = nullptr;
	}
}

void VRenderLayer::Update(float dt)
{
	VSUPERCLASS::Update(dt);

	if (PostEffect)
	{
		PostEffect->Update(dt);
	}
}

void VRenderLayer::Draw(sf::RenderTarget& RenderTarget)
{
	sf::View MainView = RenderTarget.getView();

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
		postProcessTex.clear(sf::Color::Transparent);
		PostEffect->Apply(renderTex, postProcessTex);
		postProcessTex.display();

		sprite.setTexture(postProcessTex.getTexture());
	}
	
	RenderTarget.draw(sprite, RenderState);
	RenderTarget.setView(MainView);
}
