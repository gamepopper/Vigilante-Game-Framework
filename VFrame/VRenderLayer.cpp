#include "VRenderLayer.h"
#include "VGlobal.h"

VRenderLayer::VRenderLayer(unsigned int maxSize) : VGroup(maxSize)
{
	vertexArray.setPrimitiveType(sf::Quads);
	vertexArray.append(sf::Vertex(sf::Vector2f(0.0f, VGlobal::p()->Height), sf::Color::White, sf::Vector2f(0.0f, VGlobal::p()->Height)));
	vertexArray.append(sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Color::White, sf::Vector2f()));
	vertexArray.append(sf::Vertex(sf::Vector2f(VGlobal::p()->Width, 0.0f), sf::Color::White, sf::Vector2f(VGlobal::p()->Width, 0.0f)));
	vertexArray.append(sf::Vertex(sf::Vector2f(VGlobal::p()->Width, VGlobal::p()->Height), sf::Color::White, sf::Vector2f(VGlobal::p()->Width, VGlobal::p()->Height)));
	renderTex.create(VGlobal::p()->Width, VGlobal::p()->Height);
}

void VRenderLayer::Destroy()
{
	VSUPERCLASS::Destroy();
	PostEffect = nullptr;
}

void VRenderLayer::SetTint(const sf::Color& tint)
{
	vertexArray[0].color = tint;
	vertexArray[1].color = tint;
	vertexArray[2].color = tint;
	vertexArray[3].color = tint;
}

const sf::Color& VRenderLayer::GetTint()
{
	return vertexArray[0].color;
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
	sf::View DefaultView = RenderTarget.getDefaultView();

	renderTex.setView(MainView);
	renderTex.clear(sf::Color::Transparent);
	VSUPERCLASS::Draw(renderTex);
	renderTex.display();

	if (PostEffect == nullptr || !VPostEffectBase::isSupported())
	{
		RenderState.texture = &renderTex.getTexture();
	}
	else
	{
		if (postProcessTex.getSize() != renderTex.getSize())
			postProcessTex.create(renderTex.getSize().x, renderTex.getSize().y);

		postProcessTex.clear(sf::Color::Transparent);
		PostEffect->Apply(renderTex.getTexture(), postProcessTex);
		postProcessTex.display();

		RenderState.texture = &postProcessTex.getTexture();
	}
	
	RenderState.transform = sf::Transform::Identity;

	RenderTarget.setView(DefaultView);
	RenderTarget.draw(vertexArray, RenderState);
	RenderTarget.setView(MainView);
}
