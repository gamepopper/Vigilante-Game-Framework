#pragma once
#include "VBase.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

enum SidesTouching
{
	TOUCHNONE	= 0,
	TOUCHLEFT = 1 << 0,
	TOUCHRIGHT = 1 << 1,
	TOUCHTOP = 1 << 2,
	TOUCHBOTTOM = 1 << 3,
	TOUCHALL = TOUCHLEFT | TOUCHRIGHT | TOUCHTOP | TOUCHBOTTOM
};

class VObject : public VBase
{
protected:
	void updateMotion(float dt);
	virtual void updateTransform() {};

	static float overlapX(VObject* a, VObject *b, bool maxOverlap = true);
	static float overlapY(VObject* a, VObject *b, bool maxOverlap = true);

	static sf::Vector2f overlapCircle(VObject* a, VObject *b, bool maxOverlap = true);

	static bool separateX(VObject* a, VObject *b);
	static bool separateY(VObject* a, VObject *b);

	static bool touchFlagX(VObject* a, VObject *b);
	static bool touchFlagY(VObject* a, VObject *b);

public:
	typedef VBase VSUPERCLASS;
	sf::Vector2f Position;
	sf::Vector2f Size;
	sf::Vector2f Last;
	float Radius;
	float Angle = 0;
	float AngleVelocity = 0;
	float AngleAcceleration = 0;
	float AngleDrag = 0;
	float AngleMax = 10000;
	bool Moves = true;
	bool Immovable = false;
	bool Solid = true;
	sf::Vector2f Velocity;
	sf::Vector2f Acceleration;
	sf::Vector2f Drag;
	sf::Vector2f MaxVelocity;
	sf::Vector2f ScrollFactor;
	float RotateFactor = 1;
	float ZoomFactor = 1;
	float Elasticity = 0;
	float Mass = 1;
	float Health = 100;
	int Touching = SidesTouching::TOUCHNONE;
	int WasTouching = SidesTouching::TOUCHNONE;
	bool CollisionXDrag = true;

#if _DEBUG
	sf::Color DebugColor;
#endif

	static float SeparateBias;

	VObject(sf::Vector2f position, sf::Vector2f size = sf::Vector2f()) : VBase()
	{
		Position = position;
		Size = size;
		MaxVelocity = sf::Vector2f(10000, 10000);
		ScrollFactor = sf::Vector2f(1, 1);
		RotateFactor = 1;
		ZoomFactor = 1;
		type = VType::OBJECT;

		Radius = Size.x < Size.y ? Size.x / 2 : Size.y / 2;
	}

	VObject(float x = 0, float y = 0, float width = 0, float height = 0) : VBase()
	{
		Position = sf::Vector2f(x, y);
		Size = sf::Vector2f(width, height);
		MaxVelocity = sf::Vector2f(10000, 10000);
		ScrollFactor = sf::Vector2f(1, 1);
		RotateFactor = 1;
		ZoomFactor = 1;
		type = VType::OBJECT;

		Radius = Size.x < Size.y ? Size.x / 2 : Size.y / 2;
	}

	virtual ~VObject()
	{
		Destroy();
	}

	static bool separate(VObject* a, VObject *b);
	static bool touchFlag(VObject* a, VObject *b);

	static bool separateCircle(VObject* a, VObject *b);

	static float computeVelocity(float v, float a, float d, float max, float dt);

	void SetPositionAtCentre(float x, float y);
	void SetPositionAtCentre(sf::Vector2f newPos);

	virtual void Reset(sf::Vector2f newPos);
	virtual void Reset(float x, float y);
	virtual void Hurt(float damage);

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};