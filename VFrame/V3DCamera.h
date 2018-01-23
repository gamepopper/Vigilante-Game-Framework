#pragma once
#include "V3DObject.h"
#include "depend/glm/matrix.hpp"

class V3DCamera
{
public:
	V3DCamera(const sf::Vector3f& pos, float zNear = 0.1f, float zFar = 500.0f);

	sf::Vector3f Position;
	sf::Vector2f Rotation;

	glm::mat4 Orientation() const;
	void LookAt(const sf::Vector3f& pos);

	glm::mat4 ViewMatrix() const;
	virtual glm::mat4 ProjectionMatrix() const = 0;
	glm::mat4 PVMatrix() const;

	sf::Vector3f Forward() const;
	sf::Vector3f Right() const;
	sf::Vector3f Up() const;

protected:
	float fnear;
	float ffar;

	void normalizeAngles();
};

class V3DPerspectiveCamera : public V3DCamera
{
public:
	typedef V3DCamera VSUPERCLASS;

	V3DPerspectiveCamera(const sf::Vector3f& pos, float fov, float aspectRatio, float zNear = 0.1f, float zFar = 500.0f);
	virtual glm::mat4 ProjectionMatrix() const;

	float GetAspectRatio();
	void SetAspectRatio(float value);

	float GetFOV();
	void SetFOV(float value);

private:
	float aspect;
	float fov;
};

class V3DOrthographicCamera : public V3DCamera
{
public:
	typedef V3DCamera VSUPERCLASS;

	V3DOrthographicCamera(const sf::Vector3f& pos, float width, float height, float zNear = 0.1f, float zFar = 500.0f);
	virtual glm::mat4 ProjectionMatrix() const;

	sf::Vector2f GetViewDimensions();
	void SetViewDimensions(const sf::Vector2f& value);

private:
	sf::Vector2f viewDimensions;
};