#include "V3DCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "depend/glm/glm.hpp" 
#include "depend/glm/gtx/transform.hpp"

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

V3DCamera::V3DCamera(const sf::Vector3f& pos, float zNear, float zFar)
	: Position(pos), fnear(zNear), ffar(zFar) {}

glm::mat4 V3DCamera::Orientation() const
{
	glm::mat4 orientation = glm::rotate(glm::radians(Rotation.x), glm::vec3(1, 0, 0));
	orientation *= glm::rotate(glm::radians(Rotation.y), glm::vec3(0, 1, 0));
	return orientation;
}

void V3DCamera::LookAt(const sf::Vector3f& pos)
{
	if (pos != Position)
	{
		sf::Vector3f dir = pos - Position;
		glm::vec3 dirN = glm::normalize(glm::vec3(dir.x, dir.y, dir.z));
		Rotation.x = glm::degrees(asinf(-dirN.y));
		Rotation.y = -glm::degrees(atan2f(-dirN.x, -dirN.z));
		normalizeAngles();
	}
}

glm::mat4 V3DCamera::ViewMatrix() const
{	
	return Orientation() * glm::translate(-glm::vec3(Position.x, Position.y, Position.z));
}

glm::mat4 V3DCamera::PVMatrix() const
{
	return ProjectionMatrix() * ViewMatrix();
}

sf::Vector3f V3DCamera::Forward() const
{
	glm::vec4 forward = glm::inverse(Orientation()) * glm::vec4(0, 0, -1, 1);
	return sf::Vector3f(forward.x, forward.y, forward.z);
}

sf::Vector3f V3DCamera::Right() const
{
	glm::vec4 right = glm::inverse(Orientation()) * glm::vec4(1, 0, 0, 1);
	return sf::Vector3f(right.x, right.y, right.z);
}

sf::Vector3f V3DCamera::Up() const
{
	glm::vec4 up = glm::inverse(Orientation()) * glm::vec4(0, 1, 0, 1);
	return sf::Vector3f(up.x, up.y, up.z);
}

void V3DCamera::normalizeAngles()
{
	Rotation.y = fmodf(Rotation.y + 360.0f, 360.0f);

	Rotation.x = fminf(MaxVerticalAngle, Rotation.x);
	Rotation.x = fmaxf(-MaxVerticalAngle, Rotation.x);
}

V3DPerspectiveCamera::V3DPerspectiveCamera(const sf::Vector3f& pos, float fieldOfView, float aspectRatio, float zNear, float zFar)
	: V3DCamera(pos, zNear, zFar), fov(fieldOfView), aspect(aspectRatio) {}

glm::mat4 V3DPerspectiveCamera::ProjectionMatrix() const
{
	return glm::perspective(glm::radians(fov), aspect, fnear, ffar);
}

float V3DPerspectiveCamera::GetAspectRatio()
{
	return aspect;
}

void V3DPerspectiveCamera::SetAspectRatio(float value)
{
	aspect = value;
}

float V3DPerspectiveCamera::GetFOV()
{
	return fov;
}

void V3DPerspectiveCamera::SetFOV(float value)
{
	if (value < 0 || value >= 180.0f)
		return;

	fov = value;
}

V3DOrthographicCamera::V3DOrthographicCamera(const sf::Vector3f& pos, float width, float height, float zNear, float zFar)
	: V3DCamera(pos, zNear, zFar), viewDimensions(width, height) {}

glm::mat4 V3DOrthographicCamera::ProjectionMatrix() const
{
	return glm::ortho(0.0f, viewDimensions.x, viewDimensions.y, 0.0f, fnear, ffar);
}

sf::Vector2f V3DOrthographicCamera::GetViewDimensions()
{
	return viewDimensions;
}

void V3DOrthographicCamera::SetViewDimensions(const sf::Vector2f& value)
{
	viewDimensions = value;
}