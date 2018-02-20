#pragma once
#include "VGroup.h"
#include "VPostEffect.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

//Processes group of objects to a sprite.
//Unlike the VRenderGroup, the sprite cannot be moved and will always render to the size of the screen.
class VRenderLayer : public VGroup
{
protected:
	sf::Sprite sprite;
	sf::RenderTexture renderTex;
	sf::RenderTexture postProcessTex;

public:
	typedef VGroup VSUPERCLASS;

	sf::RenderStates RenderState;
	VPostEffectBase* PostEffect = nullptr;

	VRenderLayer(unsigned int maxSize = 0);

	const sf::Texture& GetTexture();

	virtual void Destroy() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

