#include "VObject.h"
#include "VTilemap.h"
#include <cmath>

float VObject::SeparateBias = 4;

bool VObject::separate(VObject* a, VObject *b)
{
	bool separatedX = separateX(a, b) != 0;
	bool separatedY = separateY(a, b) != 0;
	return separatedX || separatedY;
}

bool VObject::touchFlag(VObject* a, VObject *b)
{
	bool touchX = touchFlagX(a, b);
	bool touchY = touchFlagY(a, b);
	return touchX || touchY;
}

bool VObject::separateX(VObject* a, VObject *b)
{
	bool aImmovable = a->Immovable;
	bool bImmovable = b->Immovable;

	if (aImmovable && bImmovable)
	{
		return false;
	}

	//Tilemap
	if (a->type == VType::TILEMAP)
	{
		VTilemap* t = dynamic_cast<VTilemap*>(a);
		return t->OverlapWithCallback(b, std::bind(&VObject::separateX, std::placeholders::_1, std::placeholders::_2));
	}

	if (b->type == VType::TILEMAP)
	{
		VTilemap* t = dynamic_cast<VTilemap*>(b);
		return t->OverlapWithCallback(a, std::bind(&VObject::separateX, std::placeholders::_1, std::placeholders::_2), true);
	}

	float overlap = overlapX(a, b);
	if (overlap != 0)
	{
		float aV = a->Velocity.x;
		float bV = b->Velocity.x;

		if (!aImmovable && !bImmovable)
		{
			overlap *= 0.5f;
			a->Position.x -= overlap;
			b->Position.x += overlap;

			float aVel = sqrtf((bV * bV * b->Mass) / a->Mass) * (bV > 0 ? 1 : -1);
			float bVel = sqrtf((aV * aV * a->Mass) / b->Mass) * (aV > 0 ? 1 : -1);
			float avg = (aVel + bVel) / 2;
			aVel -= avg;
			bVel -= avg;
			a->Velocity.x = (avg + aVel) * a->Elasticity;
			b->Velocity.x = (avg + bVel) * b->Elasticity;
		}
		else if (!aImmovable)
		{
			a->Position.x -= overlap;
			a->Velocity.x = (bV - aV) * a->Elasticity;
		}
		else if (!bImmovable)
		{
			b->Position.x -= overlap;
			b->Velocity.x = (aV - bV) * b->Elasticity;
		}

		return true;
	}

	return false;
}

bool VObject::separateY(VObject* a, VObject *b)
{
	bool aImmovable = a->Immovable;
	bool bImmovable = b->Immovable;

	if (aImmovable && bImmovable)
	{
		return false;
	}

	//Tilemap
	if (a->type == VType::TILEMAP)
	{
		VTilemap* t = dynamic_cast<VTilemap*>(a);
		return t->OverlapWithCallback(b, std::bind(&VObject::separateY, std::placeholders::_1, std::placeholders::_2));
	}

	if (b->type == VType::TILEMAP)
	{
		VTilemap* t = dynamic_cast<VTilemap*>(b);
		return t->OverlapWithCallback(a, std::bind(&VObject::separateY, std::placeholders::_1, std::placeholders::_2), true);
	}

	float overlap = overlapY(a, b);
	if (overlap != 0)
	{
		float aDelta = a->Position.y - a->Last.y;
		float bDelta = b->Position.y - b->Last.y;
		float aV = a->Velocity.y;
		float bV = b->Velocity.y;

		if (!aImmovable && !bImmovable)
		{
			overlap *= 0.5f;
			a->Position.y -= overlap;
			b->Position.y += overlap;

			float aVel = a->Mass > 0 ? sqrtf((bV * bV * b->Mass)/a->Mass) * (bV > 0 ? 1 : -1) : 0;
			float bVel = b->Mass > 0 ? sqrtf((aV * aV * a->Mass)/b->Mass) * (aV > 0 ? 1 : -1) : 0;
			float avg = (aVel + bVel) / 2;
			aVel -= avg;
			bVel -= avg;
			a->Velocity.y = (avg + aVel) * a->Elasticity;
			b->Velocity.y = (avg + bVel) * b->Elasticity;
		}
		else if (!aImmovable)
		{
			a->Position.y -= overlap;
			a->Velocity.y = (bV - aV) * a->Elasticity;

			//For moving platforms
			if (a->CollisionXDrag && b->active && b->Moves && aDelta > bDelta)
			{
				a->Position.x += b->Position.x - b->Last.x;
			}
		}
		else if (!bImmovable)
		{
			b->Position.y -= overlap;
			b->Velocity.y = (aV - bV) * b->Elasticity;

			//For moving platforms
			if (b->CollisionXDrag && a->active && a->Moves && bDelta > aDelta)
			{
				b->Position.x += a->Position.x - a->Last.x;
			}
		}

		return true;
	}

	return false;
}

bool VObject::separateCircle(VObject* a, VObject *b)
{
	bool aImmovable = a->Immovable;
	bool bImmovable = b->Immovable;

	if (aImmovable && bImmovable)
	{
		return false;
	}

	//Tilemap
	if (a->type == VType::TILEMAP)
	{
		VTilemap* t = dynamic_cast<VTilemap*>(a);
		return t->OverlapWithCallback(b, std::bind(&VObject::separateCircle, std::placeholders::_1, std::placeholders::_2));
	}

	if (b->type == VType::TILEMAP)
	{
		VTilemap* t = dynamic_cast<VTilemap*>(b);
		return t->OverlapWithCallback(a, std::bind(&VObject::separateCircle, std::placeholders::_1, std::placeholders::_2), true);
	}

	sf::Vector2f overlap = overlapCircle(a, b);
	if (overlap.x != 0 || overlap.y != 0)
	{
		sf::Vector2f aDelta = a->Position - a->Last;
		sf::Vector2f bDelta = b->Position - b->Last;
		sf::Vector2f aV = a->Velocity;
		sf::Vector2f bV = b->Velocity;

		if (!aImmovable && !bImmovable)
		{
			overlap *= 0.5f;
			a->Position -= overlap;
			b->Position += overlap;

			sf::Vector2f aVel;
			aVel.x = sqrtf((bV.x * bV.x * b->Mass) / a->Mass) * (bV.x > 0 ? 1 : -1);
			aVel.y = sqrtf((bV.y * bV.y * b->Mass) / a->Mass) * (bV.y > 0 ? 1 : -1);

			sf::Vector2f bVel;
			bVel.x = sqrtf((aV.x * aV.x * a->Mass) / b->Mass) * (aV.x > 0 ? 1 : -1);
			bVel.y = sqrtf((aV.y * aV.y * a->Mass) / b->Mass) * (aV.y > 0 ? 1 : -1);

			sf::Vector2f avg = (aVel + bVel) / 2.0f;
			aVel -= avg;
			bVel -= avg;
			a->Velocity = (avg + aVel) * a->Elasticity;
			b->Velocity = (avg + bVel) * b->Elasticity;
		}
		else if (!aImmovable)
		{
			a->Position -= overlap;
			a->Velocity = (bV - aV) * a->Elasticity;

			if (a->CollisionXDrag && b->active && b->Moves && aDelta.x > bDelta.x)
			{
				a->Position.x += b->Position.x - b->Last.x;
			}

			if (a->CollisionXDrag && b->active && b->Moves && aDelta.y > bDelta.y)
			{
				a->Position.y += b->Position.y - b->Last.y;
			}
		}
		else if (!bImmovable)
		{
			b->Position -= overlap;
			b->Velocity = (aV - bV) * b->Elasticity;

			if (b->CollisionXDrag && a->active && a->Moves && bDelta.x > aDelta.x)
			{
				b->Position.x += a->Position.x - a->Last.x;
			}

			if (b->CollisionXDrag && a->active && a->Moves && bDelta.y > aDelta.y)
			{
				b->Position.y += a->Position.y - a->Last.y;
			}
		}

		return true;
	}

	return false;
}

bool VObject::touchFlagX(VObject* a, VObject *b)
{
	//Tilemap
	if (a->type == VType::TILEMAP)
	{
		VTilemap* t = dynamic_cast<VTilemap*>(a);
		return t->OverlapWithCallback(b, std::bind(&VObject::touchFlagX, std::placeholders::_1, std::placeholders::_2));
	}

	if (b->type == VType::TILEMAP)
	{
		VTilemap* t = dynamic_cast<VTilemap*>(b);
		return t->OverlapWithCallback(a, std::bind(&VObject::touchFlagX, std::placeholders::_1, std::placeholders::_2), true);
	}

	return overlapX(a, b, false) != 0;
}

bool VObject::touchFlagY(VObject* a, VObject *b)
{
	//Tilemap
	if (a->type == VType::TILEMAP)
	{
		VTilemap* t = dynamic_cast<VTilemap*>(a);
		return t->OverlapWithCallback(b, std::bind(&VObject::touchFlagY, std::placeholders::_1, std::placeholders::_2));
	}

	if (b->type == VType::TILEMAP)
	{
		VTilemap* t = dynamic_cast<VTilemap*>(b);
		return t->OverlapWithCallback(a, std::bind(&VObject::touchFlagY, std::placeholders::_1, std::placeholders::_2), true);
	}

	return overlapY(a, b, false) != 0;
}

float VObject::overlapX(VObject* a, VObject *b, bool maxOverlap)
{
	float overlap = 0;
	float aDiff = a->Position.x - a->Last.x;
	float bDiff = b->Position.x - b->Last.x;

	if (aDiff != bDiff)
	{
		float aDiffAbs = (aDiff > 0) ? aDiff : -aDiff;
		float bDiffAbs = (bDiff > 0) ? bDiff : -bDiff;

		//Use the Last Y position instead of current to avoid floor collisions due to gravity.
		sf::FloatRect aRect = sf::FloatRect(a->Position.x - ((aDiff > 0) ? aDiff : 0), a->Last.y, a->Size.x + aDiffAbs, a->Size.y);
		sf::FloatRect bRect = sf::FloatRect(b->Position.x - ((bDiff > 0) ? bDiff : 0), b->Last.y, b->Size.x + bDiffAbs, b->Size.y);
		
		if ((aRect.left + aRect.width > bRect.left) && (aRect.left < bRect.left + bRect.width) && 
			(aRect.top + aRect.height > bRect.top) && (aRect.top < bRect.top + bRect.height))
		{
			float maxOverlapDist = maxOverlap ? (aDiffAbs + bDiffAbs + SeparateBias) : 0;

			if (aDiff > bDiff)
			{
				overlap = a->Position.x + a->Size.x - b->Position.x;

				if ((maxOverlap && overlap > maxOverlapDist) || (a->AllowCollisions & SidesTouching::TOUCHRIGHT) == 0 || (b->AllowCollisions & SidesTouching::TOUCHLEFT) == 0)
				{
					overlap = 0;
				}
				else
				{
					a->Touching |= TOUCHRIGHT;
					b->Touching |= TOUCHLEFT;
				}
			}
			else if (aDiff < bDiff)
			{
				overlap = a->Position.x - b->Size.x - b->Position.x;
				
				if ((maxOverlap && -overlap > maxOverlapDist) || (a->AllowCollisions & SidesTouching::TOUCHLEFT) == 0 || (b->AllowCollisions & SidesTouching::TOUCHRIGHT) == 0)
				{
					overlap = 0;
				}
				else
				{
					a->Touching |= TOUCHLEFT;
					b->Touching |= TOUCHRIGHT;
				}
			}
		}
	}

	return overlap;
}

float VObject::overlapY(VObject* a, VObject *b, bool maxOverlap)
{
	float overlap = 0;
	float aDiff = a->Position.y - a->Last.y;
	float bDiff = b->Position.y - b->Last.y;

	if (aDiff != bDiff)
	{
		float aDiffAbs = (aDiff > 0) ? aDiff : -aDiff;
		float bDiffAbs = (bDiff > 0) ? bDiff : -bDiff;

		sf::FloatRect aRect = sf::FloatRect(a->Position.x, a->Position.y - ((aDiff > 0) ? aDiff : 0), a->Size.x, a->Size.y + aDiffAbs);
		sf::FloatRect bRect = sf::FloatRect(b->Position.x, b->Position.y - ((bDiff > 0) ? bDiff : 0), b->Size.x, b->Size.y + bDiffAbs);

		if ((aRect.left + aRect.width > bRect.left) && (aRect.left < bRect.left + bRect.width) &&
			(aRect.top + aRect.height > bRect.top) && (aRect.top < bRect.top + bRect.height))
		{
			float maxOverlapDist = maxOverlap ? (aDiffAbs + bDiffAbs + SeparateBias) : 0;

			if (aDiff > bDiff)
			{
				overlap = a->Position.y + a->Size.y - b->Position.y;

				if ((maxOverlap && overlap > maxOverlapDist) || (a->AllowCollisions & TOUCHBOTTOM) == 0 || (b->AllowCollisions & TOUCHTOP) == 0)
				{
					overlap = 0;
				}
				else
				{
					a->Touching |= TOUCHBOTTOM;
					b->Touching |= TOUCHTOP;
				}
			}
			else if (aDiff < bDiff)
			{
				overlap = a->Position.y - b->Size.y - b->Position.y;

				if ((maxOverlap && -overlap > maxOverlapDist) || (a->AllowCollisions & TOUCHTOP) == 0 || (b->AllowCollisions & TOUCHBOTTOM) == 0)
				{
					overlap = 0;
				}
				else
				{
					a->Touching |= TOUCHTOP;
					b->Touching |= TOUCHBOTTOM;
				}
			}
		}
	}

	return overlap;
}

sf::Vector2f VObject::overlapCircle(VObject* a, VObject *b, bool maxOverlap)
{
	sf::Vector2f overlap;
	sf::Vector2f aCentre = a->Position + (a->Size / 2.0f);
	sf::Vector2f bCentre = b->Position + (b->Size / 2.0f);
	sf::Vector2f aDiff = a->Position - a->Last;
	sf::Vector2f bDiff = b->Position - b->Last;

	if (aDiff != bDiff)
	{
		sf::Vector2f diff = bCentre - aCentre;
		float length = sqrtf((diff.x * diff.x) + (diff.y * diff.y));

		if (length < a->Radius + b->Radius)
		{
			sf::Vector2f max = sf::Vector2f(maxOverlap ? (abs(aDiff.x) + abs(bDiff.x) + SeparateBias) : 0, maxOverlap ? (abs(aDiff.y) + abs(bDiff.y) + SeparateBias) : 0);
			float maxOverlapDist = sqrtf((max.x * max.x) + (max.y * max.y));
			float o = ceilf((a->Radius + b->Radius) - length);

			if ((maxOverlap && o > maxOverlapDist) || !a->IsSolid() || !b->IsSolid())
			{
				o = 0;
			}
			else
			{
				overlap = o * (diff / length);
			}
		}
	}

	return overlap;
}

bool VObject::IsSolid()
{
	return (AllowCollisions & TOUCHALL) > 0;
}

void VObject::SetPositionAtCentre(float x, float y)
{
	SetPositionAtCentre(sf::Vector2f(x, y));
}

void VObject::SetPositionAtCentre(sf::Vector2f newPos)
{
	Position = newPos - (sf::Vector2f(Size) / 2.0f);
}

void VObject::Reset(sf::Vector2f newPos)
{
	Position = newPos;
	Last = Position;
	Velocity = sf::Vector2f();
	Revive();
}

void VObject::Reset(float x, float y)
{
	Reset(sf::Vector2f(x, y));
}

void VObject::Hurt(float damage)
{
	Health -= damage;

	if (Health <= 0)
	{
		Health = 0;
		Kill();
	}
}

void VObject::updateMotion(float dt)
{
	float deltaV;
	float delta;

	if (Velocity.x > MaxVelocity.x)
		Velocity.x = MaxVelocity.x;
	if (Velocity.x < -MaxVelocity.x)
		Velocity.x = -MaxVelocity.x;
	if (Velocity.y > MaxVelocity.y)
		Velocity.y = MaxVelocity.y;
	if (Velocity.y < -MaxVelocity.y)
		Velocity.y = -MaxVelocity.y;

	deltaV = 0.5f * (computeVelocity(AngleVelocity, AngleAcceleration, AngleDrag, AngleMax, dt) - AngleVelocity);
	AngleVelocity += deltaV;
	Angle += AngleVelocity * dt;
	AngleVelocity += deltaV;

	deltaV = 0.5f * (computeVelocity(Velocity.x, Acceleration.x, Drag.x, MaxVelocity.x, dt) - Velocity.x);
	Velocity.x += deltaV;
	delta = Velocity.x * dt;
	Velocity.x += deltaV;
	Position.x += delta;

	deltaV = 0.5f * (computeVelocity(Velocity.y, Acceleration.y, Drag.y, MaxVelocity.y, dt) - Velocity.y);
	Velocity.y += deltaV;
	delta = Velocity.y * dt;
	Velocity.y += deltaV;
	Position.y += delta;
}

float VObject::computeVelocity(float v, float a, float d, float max, float dt)
{
	if (a != 0)
	{
		v += a * dt;
	}
	else if (d != 0)
	{
		float drag = d * dt;

		if (v - drag > 0)
		{
			v -= drag;
		}
		else if (v + drag < 0)
		{
			v += drag;
		}
		else
		{
			v = 0;
		}
	}

	if (v != 0 && max != 0)
	{
		if (v > max)
		{
			v = max;
		}
		else if (v < -max)
		{
			v = -max;
		}
	}

	return v;
}

void VObject::Update(float dt)
{
	VSUPERCLASS::Update(dt);

	Last.x = Position.x;
	Last.y = Position.y;

	if (Moves)
	{
		updateMotion(dt);
	}

	WasTouching = Touching;
	Touching = TOUCHNONE;
}

void VObject::Draw(sf::RenderTarget &RenderTarget)
{
	updateTransform();
	VSUPERCLASS::Draw(RenderTarget);
}