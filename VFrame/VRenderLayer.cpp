#include "VRenderLayer.h"
#include "VGlobal.h"

VRenderLayer::VRenderLayer(unsigned int maxSize) : VGroup(maxSize)
{
	vertexArray.setPrimitiveType(sf::Quads);
	vertexArray.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::White, sf::Vector2f(0.f, 0.f)));
	vertexArray.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::White, sf::Vector2f(0.f, 0.f)));
	vertexArray.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::White, sf::Vector2f(0.f, 0.f)));
	vertexArray.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color::White, sf::Vector2f(0.f, 0.f)));
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

	if (renderTex.getSize() != sf::Vector2u(MainView.getSize()))
	{
		float Width = MainView.getSize().x;
		float Height = MainView.getSize().y;
		vertexArray[0] = sf::Vertex(sf::Vertex(sf::Vector2f(0.0f, Height), sf::Color::White, sf::Vector2f(0.0f, Height)));
		vertexArray[1] = sf::Vertex(sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Color::White, sf::Vector2f()));
		vertexArray[2] = sf::Vertex(sf::Vertex(sf::Vector2f(Width, 0.0f), sf::Color::White, sf::Vector2f(Width, 0.0f)));
		vertexArray[3] = sf::Vertex(sf::Vertex(sf::Vector2f(Width, Height), sf::Color::White, sf::Vector2f(Width, Height)));
		renderTex.create((unsigned int)Width, (unsigned int)Height);
	}

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
