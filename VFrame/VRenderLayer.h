#pragma once
#include "VGroup.h"
#include "VPostEffect.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

class VRenderLayer : public VGroup
{
protected:
	sf::Sprite sprite;
	sf::RenderTexture renderTex;
	sf::RenderTexture postProcessTex;

public:
	typedef VGroup VSUPERCLASS;

	sf::RenderStates RenderState;
	VPostEffectBase* PostEffect = NULL;

	VRenderLayer(unsigned int maxSize = 0);
	virtual ~VRenderLayer()
	{
		Destroy();

		if (PostEffect != NULL)
		{
			delete PostEffect;
			PostEffect = NULL;
		}
	}

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

