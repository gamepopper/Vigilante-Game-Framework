#pragma once
#include "VRenderGroup.h"
#include <SFML/System/Clock.hpp>

//Special VRenderGroup that applies a trail to objects it contains within the area.
class VTrailArea : public VRenderGroup
{
private:
	sf::Sprite trailSprite;
	sf::RenderTexture trailTex;
	sf::RenderTexture trailTex2;
	sf::Clock swapTimer;
	sf::Clock fadeTimer;
	float alphaTrack = 0;

public:
	typedef VRenderGroup VSUPERCLASS;
	float Delay = 0.05f;
	float RedMultiplier		= 1.0f;
	float BlueMultiplier	= 1.0f;
	float GreenMultiplier	= 1.0f;
	float AlphaMultiplier	= 0.8f;

	VTrailArea(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize = 0);
	VTrailArea(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize = 0);

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

