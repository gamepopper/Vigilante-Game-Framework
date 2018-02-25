#ifndef VFRAME_NO_3D
#include "V3DObject.h"
#include "VObject.h"

void V3DObject::Reset(sf::Vector3f newPos)
{
	Position = newPos;
	Rotation = sf::Vector3f();
	Last = Position;
	Velocity = sf::Vector3f();
	Revive();
}

void V3DObject::Reset(float x, float y, float z)
{
	Reset(sf::Vector3f(x, y, z));
}

void V3DObject::Hurt(float damage)
{
	Health -= damage;

	if (Health <= 0)
	{
		Health = 0;
		Kill();
	}
}

void V3DObject::updateMotion(float dt)
{
	float deltaV;
	float delta;

	deltaV = 0.5f * (VObject::computeVelocity(AngleVelocity.x, AngleAcceleration.x, AngleDrag.x, AngleMax.x, dt) - AngleVelocity.x);
	AngleVelocity.x += deltaV;
	Rotation.x += AngleVelocity.x * dt;
	AngleVelocity.x += deltaV;
	Rotation.x = fmodf(Rotation.x + 360.0f, 360.0f);

	deltaV = 0.5f * (VObject::computeVelocity(AngleVelocity.y, AngleAcceleration.y, AngleDrag.y, AngleMax.y, dt) - AngleVelocity.y);
	AngleVelocity.y += deltaV;
	Rotation.y += AngleVelocity.y * dt;
	AngleVelocity.y += deltaV;
	Rotation.y = fmodf(Rotation.y + 360.0f, 360.0f);

	deltaV = 0.5f * (VObject::computeVelocity(AngleVelocity.z, AngleAcceleration.z, AngleDrag.z, AngleMax.z, dt) - AngleVelocity.z);
	AngleVelocity.z += deltaV;
	Rotation.z += AngleVelocity.z * dt;
	AngleVelocity.z += deltaV;
	Rotation.z = fmodf(Rotation.z + 360.0f, 360.0f);

	deltaV = 0.5f * (VObject::computeVelocity(Velocity.x, Acceleration.x, Drag.x, MaxVelocity.x, dt) - Velocity.x);
	Velocity.x += deltaV;
	delta = Velocity.x * dt;
	Velocity.x += deltaV;
	Position.x += delta;

	deltaV = 0.5f * (VObject::computeVelocity(Velocity.y, Acceleration.y, Drag.y, MaxVelocity.y, dt) - Velocity.y);
	Velocity.y += deltaV;
	delta = Velocity.y * dt;
	Velocity.y += deltaV;
	Position.y += delta;

	deltaV = 0.5f * (VObject::computeVelocity(Velocity.z, Acceleration.z, Drag.z, MaxVelocity.z, dt) - Velocity.z);
	Velocity.z += deltaV;
	delta = Velocity.z * dt;
	Velocity.z += deltaV;
	Position.z += delta;
}

void V3DObject::UpdateShader(V3DShader* shader, V3DCamera* camera)
{

}

void V3DObject::Update(float dt)
{
	VSUPERCLASS::Update(dt);

	Last.x = Position.x;
	Last.y = Position.y;
	Last.z = Position.z;

	if (Moves)
	{
		if (Velocity.x > MaxVelocity.x)
			Velocity.x = MaxVelocity.x;
		if (Velocity.x < -MaxVelocity.x)
			Velocity.x = -MaxVelocity.x;
		if (Velocity.y > MaxVelocity.y)
			Velocity.y = MaxVelocity.y;
		if (Velocity.y < -MaxVelocity.y)
			Velocity.y = -MaxVelocity.y;
		if (Velocity.z > MaxVelocity.z)
			Velocity.z = MaxVelocity.z;
		if (Velocity.z < -MaxVelocity.z)
			Velocity.z = -MaxVelocity.z;

		updateMotion(dt);
	}

	updateTransform();
}

void V3DObject::Draw(sf::RenderTarget &RenderTarget)
{
	VSUPERCLASS::Draw(RenderTarget);
}
#endif