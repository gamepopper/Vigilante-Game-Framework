#ifndef VFRAME_NO_3D
#include "V3DCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "depend/glm/glm.hpp" 
#include "depend/glm/gtx/transform.hpp"

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

V3DCamera::V3DCamera(const sf::Vector3f& pos, float zNear, float zFar)
	: Position(pos), Viewport(0.0f, 0.0f, 1.0f, 1.0f), fnear(zNear), ffar(zFar) {}

glm::mat4 V3DCamera::Orientation() const
{
	glm::mat4 orientation = glm::rotate(glm::radians(Rotation.x), glm::vec3(1, 0, 0));
	orientation *= glm::rotate(glm::radians(Rotation.y), glm::vec3(0, 1, 0));
	orientation *= glm::rotate(glm::radians(Rotation.z), glm::vec3(0, 0, 1));
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
	Rotation.z = fmodf(Rotation.z + 360.0f, 360.0f);

	Rotation.x = fminf(MaxVerticalAngle, Rotation.x);
	Rotation.x = fmaxf(-MaxVerticalAngle, Rotation.x);
}

bool V3DCamera::PointInView(sf::Vector3f p)
{
	updatePlanes();

	for (int i = 0; i < 6; ++i)
	{
		if (pointPlaneDistance(planes[i], glm::vec3(p.x, p.y, p.z)) < 0)
			return false;
	}

	return true;
}

bool V3DCamera::SphereInView(sf::Vector3f p, float radius)
{
	updatePlanes();

	bool view = true;
	float distance = 0.0f;
	for (int i = 0; i < 6; ++i)
	{
		distance = pointPlaneDistance(planes[i], glm::vec3(p.x, p.y, p.z));
		view = view && (distance < -radius);
	}

	return view;
}

bool V3DCamera::BoxInView(sf::Vector3f p, sf::Vector3f min, sf::Vector3f max)
{
	updatePlanes();

	for (int i = 0; i < 6; ++i)
	{
		sf::Vector3f positive = min;
		sf::Vector3f negative = max;
		
		const float pos = planes[i].w;
		const glm::vec3 normal = glm::vec3(planes[i]);

		positive.x = (normal.x >= 0.0f) ? max.x : positive.x;
		negative.x = (normal.x >= 0.0f) ? min.x : negative.x;
									    
		positive.y = (normal.y >= 0.0f) ? max.y : positive.y;
		negative.y = (normal.y >= 0.0f) ? min.y : negative.y;
									    
		positive.z = (normal.z >= 0.0f) ? max.z : positive.z;
		negative.z = (normal.z >= 0.0f) ? min.z : negative.z;

		positive += p;
		negative += p;

		float dotPositive = glm::dot(normal, glm::vec3(positive.x, positive.y, positive.z));
		if (dotPositive + pos < 0.f)
			return false;

		//float dotNegative = glm::dot(normal, glm::vec3(negative.x, negative.y, negative.z));
		//if (dotNegative + pos < -0.f)
		//	return true;
	}

	return true;
}

void V3DCamera::updatePlanes()
{
	const glm::mat4 &v = ViewMatrix();
	const glm::mat4 &p = ProjectionMatrix();

	glm::mat4 clipMatrix;

	clipMatrix[0][0] = v[0][0] * p[0][0] + v[0][1] * p[1][0] + v[0][2] * p[2][0] + v[0][3] * p[3][0];
	clipMatrix[1][0] = v[0][0] * p[0][1] + v[0][1] * p[1][1] + v[0][2] * p[2][1] + v[0][3] * p[3][1];
	clipMatrix[2][0] = v[0][0] * p[0][2] + v[0][1] * p[1][2] + v[0][2] * p[2][2] + v[0][3] * p[3][2];
	clipMatrix[3][0] = v[0][0] * p[0][3] + v[0][1] * p[1][3] + v[0][2] * p[2][3] + v[0][3] * p[3][3];
	clipMatrix[0][1] = v[1][0] * p[0][0] + v[1][1] * p[1][0] + v[1][2] * p[2][0] + v[1][3] * p[3][0];
	clipMatrix[1][1] = v[1][0] * p[0][1] + v[1][1] * p[1][1] + v[1][2] * p[2][1] + v[1][3] * p[3][1];
	clipMatrix[2][1] = v[1][0] * p[0][2] + v[1][1] * p[1][2] + v[1][2] * p[2][2] + v[1][3] * p[3][2];
	clipMatrix[3][1] = v[1][0] * p[0][3] + v[1][1] * p[1][3] + v[1][2] * p[2][3] + v[1][3] * p[3][3];
	clipMatrix[0][2] = v[2][0] * p[0][0] + v[2][1] * p[1][0] + v[2][2] * p[2][0] + v[2][3] * p[3][0];
	clipMatrix[1][2] = v[2][0] * p[0][1] + v[2][1] * p[1][1] + v[2][2] * p[2][1] + v[2][3] * p[3][1];
	clipMatrix[2][2] = v[2][0] * p[0][2] + v[2][1] * p[1][2] + v[2][2] * p[2][2] + v[2][3] * p[3][2];
	clipMatrix[3][2] = v[2][0] * p[0][3] + v[2][1] * p[1][3] + v[2][2] * p[2][3] + v[2][3] * p[3][3];
	clipMatrix[0][3] = v[3][0] * p[0][0] + v[3][1] * p[1][0] + v[3][2] * p[2][0] + v[3][3] * p[3][0];
	clipMatrix[1][3] = v[3][0] * p[0][1] + v[3][1] * p[1][1] + v[3][2] * p[2][1] + v[3][3] * p[3][1];
	clipMatrix[2][3] = v[3][0] * p[0][2] + v[3][1] * p[1][2] + v[3][2] * p[2][2] + v[3][3] * p[3][2];
	clipMatrix[3][3] = v[3][0] * p[0][3] + v[3][1] * p[1][3] + v[3][2] * p[2][3] + v[3][3] * p[3][3];

	planes[PRIGHT].x = clipMatrix[3][0] + clipMatrix[0][0];
	planes[PRIGHT].y = clipMatrix[3][1] + clipMatrix[0][1];
	planes[PRIGHT].z = clipMatrix[3][2] + clipMatrix[0][2];
	planes[PRIGHT].w = clipMatrix[3][3] + clipMatrix[0][3];

	planes[PLEFT].x = clipMatrix[3][0] - clipMatrix[0][0];
	planes[PLEFT].y = clipMatrix[3][1] - clipMatrix[0][1];
	planes[PLEFT].z = clipMatrix[3][2] - clipMatrix[0][2];
	planes[PLEFT].w = clipMatrix[3][3] - clipMatrix[0][3];

	planes[PBOTTOM].x = clipMatrix[3][0] - clipMatrix[1][0];
	planes[PBOTTOM].y = clipMatrix[3][1] - clipMatrix[1][1];
	planes[PBOTTOM].z = clipMatrix[3][2] - clipMatrix[1][2];
	planes[PBOTTOM].w = clipMatrix[3][3] - clipMatrix[1][3];

	planes[PTOP].x = clipMatrix[3][0] + clipMatrix[1][0];
	planes[PTOP].y = clipMatrix[3][1] + clipMatrix[1][1];
	planes[PTOP].z = clipMatrix[3][2] + clipMatrix[1][2];
	planes[PTOP].w = clipMatrix[3][3] + clipMatrix[1][3];

	planes[PNEAR].x = clipMatrix[3][0] + clipMatrix[2][0];
	planes[PNEAR].y = clipMatrix[3][1] + clipMatrix[2][1];
	planes[PNEAR].z = clipMatrix[3][2] + clipMatrix[2][2];
	planes[PNEAR].w = clipMatrix[3][3] + clipMatrix[2][3];

	planes[PFAR].x = clipMatrix[3][0] - clipMatrix[2][0];
	planes[PFAR].y = clipMatrix[3][1] - clipMatrix[2][1];
	planes[PFAR].z = clipMatrix[3][2] - clipMatrix[2][2];
	planes[PFAR].w = clipMatrix[3][3] - clipMatrix[2][3];

	for (int i = 0; i < 6; ++i)
	{
		float mag = glm::length(glm::vec3(planes[i]));
		planes[i] /= mag;
	}
}

float V3DCamera::pointPlaneDistance(const glm::vec4& plane, const glm::vec3& point)
{
	return (plane.x * point.x) + (plane.y * point.y) + (plane.z * point.z) + plane.w;
}

V3DPerspectiveCamera::V3DPerspectiveCamera(const sf::Vector3f& pos, float fieldOfView, float aspectRatio, float zNear, float zFar)
	: V3DCamera(pos, zNear, zFar), aspect(aspectRatio), fov(fieldOfView) {}

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
	fov = fov * !(value > 0 && value < 180.0f) + value * (value > 0 && value < 180.0f);
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
#endif