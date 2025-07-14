#include "VRenderLayer.h"
#include "VGlobal.h"

VRenderLayer::VRenderLayer(unsigned int maxSize) : VGroup(maxSize)
{
	vertexArray.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	vertexArray.append({});
	vertexArray.append({});
	vertexArray.append({});
	vertexArray.append({});
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
		vertexArray[0] = { sf::Vector2f(0.0f, 0.0f),	vertexArray[0].color, sf::Vector2f() };
		vertexArray[1] = { sf::Vector2f(Width, 0.0f),	vertexArray[0].color, sf::Vector2f(Width, 0.0f) };
		vertexArray[2] = { sf::Vector2f(0.0f, Height),	vertexArray[0].color, sf::Vector2f(0.0f, Height) };
		vertexArray[3] = { sf::Vector2f(Width, Height),	vertexArray[0].color, sf::Vector2f(Width, Height) };
		if (!renderTex.resize({ (unsigned int)Width, (unsigned int)Height }))
		{
			VLogError("Invalid size for a RenderLayer");
		}
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
		{
			if (!postProcessTex.resize(renderTex.getSize()))
			{
				VLogError("Invalid Texture Size to resize post processing texture in RenderGroup");
			}
		}

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
