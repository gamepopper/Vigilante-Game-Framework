#pragma once
#include "VSprite.h"
#include "VRange.h"

class VParticle : public VObject
{
public:
	typedef VObject VSUPERCLASS;
	float Lifespan;
	float Age;
	sf::Color Tint;
	sf::Vector2f Scale;
	VRange<sf::Vector2f> ScaleRange;
	VRange<sf::Color> ColourRange;
	VRange<float> AlphaRange;

	VParticle() : VObject()
	{
		Lifespan = 0;
		Age = 0;
		exists = false;
	}
	virtual ~VParticle()
	{
		Destroy();
	}

	virtual void Update(float dt) override;
	virtual void Reset(float x, float y) override;
	virtual void OnEmit() {}
};

