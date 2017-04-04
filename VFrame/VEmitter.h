#pragma once
#include "VGroup.h"
#include "VParticle.h"

#include <SFML\Graphics\VertexArray.hpp>

class VEmitter : public VGroup
{
protected:
	sf::Texture texture;
	sf::VertexArray vertices;

	bool running = false;
	bool willKill = false;
	float timer = 0;
	int counter = 0;
	int amount = 0;

public:
	typedef VGroup VSUPERCLASS;

	float Frequency = 0.5f;
	unsigned int AmountPerEmit = 1;
	bool Explosion = false;
	bool Circular = false;
	bool AngleVelocity = false;
	bool Constant = false;

	sf::Vector2f Position;
	sf::Vector2f Size;
	bool Moves = true;
	bool Immovable = false;
	bool Solid = false;
	sf::Vector2f ScrollFactor;
	float RotateFactor = 1;
	float ZoomFactor = 1;

	VRange<float> EmittingAngle;
	VRange<float> Lifespan;
	VRange<sf::Vector2f> VelocityRange;
	VRange<float> SpeedRange;
	VRange<float> AngleVelocityRange;
	VRange<float> AngleAccelerationRange;
	VRange<float> AngleDragRange;
	VRange<float> AngleRange;
	VRange<sf::Vector2f> DragRange;
	VRange<sf::Vector2f> AccelerationRange;
	VRange<float> ElasticityRange;
	VRangeBounds<sf::Vector2f> ScaleRange;
	VRangeBounds<sf::Color> ColourRange;
	VRangeBounds<float> AlphaRange;

	VParticle* ParticleInstance = new VParticle();

	sf::RenderStates RenderState = sf::RenderStates::Default;

	VEmitter(float x, float y, unsigned int MaxSize = 0) : VGroup(MaxSize)
	{
		this->Position = sf::Vector2f(x,y);

		EmittingAngle = VRange<float>(-180, 180);
		Lifespan = VRange<float>(3);
		VelocityRange = VRange<sf::Vector2f>(sf::Vector2f(-100, -100), sf::Vector2f(100, 100));
		SpeedRange = VRange<float>(0, 100);
		AngleRange = VRange<float>(0, 0);
		AngleVelocityRange = VRange<float>(0, 0);
		AngleAccelerationRange = VRange<float>(0, 0);
		AngleDragRange = VRange<float>(0, 0);
		DragRange = VRange<sf::Vector2f>(sf::Vector2f(), sf::Vector2f());
		AccelerationRange = VRange<sf::Vector2f>(sf::Vector2f(), sf::Vector2f());
		ElasticityRange = VRange<float>(0, 0);
		ScaleRange = VRangeBounds<sf::Vector2f>(sf::Vector2f(1, 1), sf::Vector2f(1, 1));
		ColourRange = VRangeBounds<sf::Color>(sf::Color::White, sf::Color::White);
		AlphaRange = VRangeBounds<float>(1, 1);

		vertices.setPrimitiveType(sf::PrimitiveType::Quads);

		exists = false;
	}
	
	VEmitter(sf::Vector2f Position, unsigned int MaxSize = 0) : 	VGroup(MaxSize),
																	EmittingAngle(-180, 180),
																	Lifespan(3),
																	VelocityRange(sf::Vector2f(-100, -100), sf::Vector2f(100, 100)),
																	SpeedRange(0, 100),
																	AngleRange(0, 0),
																	AngleVelocityRange(0, 0),
																	AngleAccelerationRange(0, 0),
																	AngleDragRange(0, 0),
																	DragRange(sf::Vector2f(), sf::Vector2f()),
																	AccelerationRange(sf::Vector2f(), sf::Vector2f()),
																	ElasticityRange(0, 0),
																	ScaleRange(sf::Vector2f(1, 1), sf::Vector2f(1, 1)),
																	ColourRange(sf::Color::White, sf::Color::White),
																	AlphaRange(1, 1)
	{
		this->Position = Position;
		vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		
		exists = false;
	}
	virtual ~VEmitter()
	{
		Destroy();
	}

	VEmitter* LoadParticlesFromFile(int Amount, sf::String Filename, bool Animated = false, int Width = 1, int Height = 1, int TextureWidth = 0, bool RandomFrames = true);
	VEmitter* LoadParticles(int Amount, sf::Texture Texture, bool Animated = false, int Width = 1, int Height = 1, int TextureWidth = 0, bool RandomFrames = true);
	VEmitter* MakeParticles(int Amount, int Width, int Height, sf::Color Color = sf::Color::White);
	virtual void Kill() override;
	virtual void Destroy() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;

	VEmitter* Start(int Amount = 0);
	virtual void EmitParticle();
};

