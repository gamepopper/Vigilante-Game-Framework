#ifndef VFRAME_NO_3D
#include "V3DObject.h"
#include "VObject.h"

#include <cmath>

V3DObject::V3DObject(sf::Vector3f position, sf::Vector3f rotation, sf::Vector3f scale) : VBase()
{
	Position = position;
	Rotation = rotation;
	Scale = scale;
	MaxVelocity = sf::Vector3f(10000, 10000, 10000);
	AngleMax = sf::Vector3f(10000, 10000, 10000);
	type = VType::OBJECT;
}

V3DObject::V3DObject(float posX, float posY, float posZ,
	float rotX, float rotY, float rotZ,
	float scaleX, float scaleY, float scaleZ) : VBase()
{
	Position = sf::Vector3f(posX, posY, posZ);
	Rotation = sf::Vector3f(rotX, rotY, rotZ);
	Scale = sf::Vector3f(scaleX, scaleY, scaleZ);
	MaxVelocity = sf::Vector3f(10000, 10000, 10000);
	AngleMax = sf::Vector3f(10000, 10000, 10000);
	type = VType::OBJECT;
}

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

#define GLM_ENABLE_EXPERIMENTAL
#include "depend/glm/glm.hpp" 
#include "depend/glm/gtx/transform.hpp"
sf::Vector3f V3DObject::GetMinimum()
{
	if (Rotation != sf::Vector3f())
	{
		glm::mat4 matrix_scale = glm::scale(glm::vec3(Scale.x, Scale.y, Scale.z));
		// Represent each stored rotation as a different matrix, because 
		// we store angles. 
		//          x  y  z 
		glm::mat4 matrix_rotX = glm::rotate(-Rotation.x * (VFRAME_PI / 180.0f), glm::vec3(1, 0, 0));
		glm::mat4 matrix_rotY = glm::rotate(-Rotation.y * (VFRAME_PI / 180.0f), glm::vec3(0, 1, 0));
		glm::mat4 matrix_rotZ = glm::rotate(-Rotation.z * (VFRAME_PI / 180.0f), glm::vec3(0, 0, 1));
		// Create a rotation matrix. 
		// Multiply in reverse order it needs to be applied. 
		glm::mat4 matrix_rotation = matrix_rotZ * matrix_rotY * matrix_rotX;
		// Apply transforms in reverse order they need to be applied in. 
		glm::mat4 transform = matrix_rotation * matrix_scale;

		glm::vec3 min = glm::vec3(glm::vec4(minimum.x, minimum.y, minimum.z, 0.0f) * transform);
		glm::vec3 max = glm::vec3(glm::vec4(maximum.x, maximum.y, maximum.z, 0.0f) * transform);

		sf::Vector3f result = sf::Vector3f(
			min.x < max.x ? min.x : max.x,
			min.y < max.y ? min.y : max.y,
			min.z < max.z ? min.z : max.z);

		return result;
	}

	return minimum;
}

sf::Vector3f V3DObject::GetMaximum()
{
	if (Rotation != sf::Vector3f())
	{
		glm::mat4 matrix_scale = glm::scale(glm::vec3(Scale.x, Scale.y, Scale.z));
		// Represent each stored rotation as a different matrix, because 
		// we store angles. 
		//          x  y  z 
		glm::mat4 matrix_rotX = glm::rotate(Rotation.x * (VFRAME_PI / 180.0f), glm::vec3(1, 0, 0));
		glm::mat4 matrix_rotY = glm::rotate(Rotation.y * (VFRAME_PI / 180.0f), glm::vec3(0, 1, 0));
		glm::mat4 matrix_rotZ = glm::rotate(Rotation.z * (VFRAME_PI / 180.0f), glm::vec3(0, 0, 1));
		// Create a rotation matrix. 
		// Multiply in reverse order it needs to be applied. 
		glm::mat4 matrix_rotation = matrix_rotZ * matrix_rotY * matrix_rotX;
		// Apply transforms in reverse order they need to be applied in. 
		glm::mat4 transform = matrix_rotation * matrix_scale;

		glm::vec3 min = glm::vec3(glm::vec4(minimum.x, minimum.y, minimum.z, 0.0f) * transform);
		glm::vec3 max = glm::vec3(glm::vec4(maximum.x, maximum.y, maximum.z, 0.0f) * transform);

		sf::Vector3f result = sf::Vector3f(
			min.x > max.x ? min.x : max.x,
			min.y > max.y ? min.y : max.y,
			min.z > max.z ? min.z : max.z);

		return result;
	}

	return maximum;
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
		Velocity.x = std::fminf(Velocity.x, MaxVelocity.x);
		Velocity.x = std::fmaxf(Velocity.x, -MaxVelocity.x);
		Velocity.y = std::fminf(Velocity.y, MaxVelocity.y);
		Velocity.y = std::fmaxf(Velocity.y, -MaxVelocity.y);
		Velocity.z = std::fminf(Velocity.z, MaxVelocity.z);
		Velocity.z = std::fmaxf(Velocity.z, -MaxVelocity.z);

		updateMotion(dt);
	}
}

void V3DObject::Draw(sf::RenderTarget &RenderTarget)
{
	updateTransform();
	VSUPERCLASS::Draw(RenderTarget);
}
#endif