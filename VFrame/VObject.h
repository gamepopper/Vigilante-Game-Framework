#pragma once
#include "VBase.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

enum SidesTouching : unsigned char
{
	TOUCHNONE	= 0,
	TOUCHLEFT	= 1 << 0,
	TOUCHRIGHT	= 1 << 1,
	TOUCHTOP	= 1 << 2,
	TOUCHBOTTOM = 1 << 3,
	TOUCHWALL	= TOUCHLEFT | TOUCHRIGHT,
	TOUCHALL	= TOUCHLEFT | TOUCHRIGHT | TOUCHTOP | TOUCHBOTTOM
};

class VObject : public VBase
{
protected:
	virtual void updateMotion(float dt);
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

	float Radius = 0;
	float Angle = 0;
	float AngleVelocity = 0;
	float AngleAcceleration = 0;
	float AngleDrag = 0;
	float AngleMax = 10000;

	//Should object update it's position and angle.
	bool Moves = true;

	sf::Vector2f Velocity;
	sf::Vector2f Acceleration;
	sf::Vector2f Drag;
	sf::Vector2f MaxVelocity;

	//Set for parallax movement
	sf::Vector2f ScrollFactor;
	//Set for parallax rotation
	float RotateFactor = 1;
	//Set for parallax zoom
	float ZoomFactor = 1;

	//Bounciness of the object (0 assumes no bounce)
	float Elasticity = 0;
	//Mass of object in comparison to other objects in world.
	float Mass = 1;

	//Standard health value (object dies if health is <= 0)
	float Health = 100;

	//Effects how to respond to collisions with other objects.
	bool Immovable = false;

	//Which sides are currently touching (is reset on each Update call!)
	unsigned char Touching = SidesTouching::TOUCHNONE;
	//Which sides on previous frame were touching (is reset on each Update call!)
	unsigned char WasTouching = SidesTouching::TOUCHNONE;
	
	//Which sides are allowed to check collisions for.
	unsigned char AllowCollisions = SidesTouching::TOUCHALL;
	
	//Useful for moving platforms.
	bool CollisionXDrag = true;

#if _DEBUG
	sf::Color DebugColor;
#endif

	//Adds to the maximum overlap distance between objects.
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

	//Separate objects that overlap based on rectangle collision.
	static bool separate(VObject* a, VObject *b);
	//Separate objects that overlap based on circle collision.
	static bool separateCircle(VObject* a, VObject *b);
	//Process touch test.
	static bool touchFlag(VObject* a, VObject *b);

	//Base function for calculating velocity.
	static float computeVelocity(float v, float a, float d, float max, float dt);

	//Sets object position based on the centre of it's circular area.
	void SetPositionAtCentre(float x, float y);
	//Sets object position based on the centre of it's circular area.
	void SetPositionAtCentre(sf::Vector2f newPos);

	//Returns true if collision is allowed on object on any side.
	bool IsSolid();

	//Resets object with new position.
	virtual void Reset(sf::Vector2f newPos);
	//Resets object with new position.
	virtual void Reset(float x, float y);
	//Reduces health and kills object if health is below 0.
	virtual void Hurt(float damage);

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};