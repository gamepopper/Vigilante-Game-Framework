#include "VParticle.h"
#include "VInterpolate.h"

void VParticle::Update(float dt)
{
	if (Age < Lifespan)
	{
		Age += dt;
	}

	if (Age >= Lifespan && Lifespan != 0)
	{
		visible = false;
		Kill();
	}

	float percent = Lifespan > 0 ? Age / Lifespan : 0;
	Scale = VInterpolate::Vector2f(ScaleRange.A, ScaleRange.B, percent);
	Tint = VInterpolate::Color(ColourRange.A, ColourRange.B, percent);
	Tint.a = sf::Uint8(VInterpolate::Float(AlphaRange.A, AlphaRange.B, percent) * 255);

	VSUPERCLASS::Update(dt);
}

void VParticle::Reset(float x, float y)
{
	VSUPERCLASS::Reset(x, y);
	Age = 0;
	visible = true;
}
