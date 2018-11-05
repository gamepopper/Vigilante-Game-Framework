/**
* @file    V3DCamera.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2018 Tim Stoddard
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* @section DESCRIPTION
*
* 3D Camera Classes.
*/

#pragma once

#ifndef VFRAME_NO_3D
#include "V3DObject.h"
#include "depend/glm/matrix.hpp"

///Base 3D Camera
class V3DCamera
{
public:

	/**
	* @param pos Initial Camera Position.
	* @param zNear The near z value for the projection matrix.
	* @param zFar The far z value for the projection matrix.
	*/
	V3DCamera(const sf::Vector3f& pos, float zNear = 0.1f, float zFar = 500.0f);

	///The Camera Position.
	sf::Vector3f Position;
	///The Camera Angle.
	sf::Vector3f Rotation;
	///@return Calculated the rotation matrix.
	glm::mat4 Orientation() const;
	///@param pos The target position for the camera to look at.
	void LookAt(const sf::Vector3f& pos);
	///@return The calculated View Matrix of the Camera.
	glm::mat4 ViewMatrix() const;
	///@return The calculated Projection Matrix of the Camera.
	virtual glm::mat4 ProjectionMatrix() const = 0;
	///@return The combined Projection View Matrix of the Camera. Combine with a model matrix to get the transform of the model in a scene.
	glm::mat4 PVMatrix() const;
	///Camera's Forward Vector.
	sf::Vector3f Forward() const;
	///Camera's Right Vector.
	sf::Vector3f Right() const;
	///Camera's Up Vector.
	sf::Vector3f Up() const;
	///Render Viewport Area to apply a Renderscene.
	sf::FloatRect Viewport;

	/**
	* @param p Position of 3D Point.
	* @return True if 
	*/
	virtual bool PointInView(sf::Vector3f p);

	/**
	* @param p Centre Position of Sphere.
	* @param radius Radius of Sphere.
	* @return True if sphere is in view.
	*/
	virtual bool SphereInView(sf::Vector3f p, float radius);

	/**
	* @param p Centre Position of Box.
	* @param size Dimensions of Box.
	* @return True if box is in view.
	*/
	virtual bool BoxInView(sf::Vector3f p, sf::Vector3f min, sf::Vector3f max);

protected:
	enum FrustumPlane : unsigned char
	{
		PRIGHT,
		PLEFT,
		PTOP,
		PBOTTOM,
		PNEAR,
		PFAR,
	};

	///Field Near value.
	float fnear;
	///Field Far value.
	float ffar;
	///Normalize the rotation angles.
	void normalizeAngles();

	void updatePlanes();
	float pointPlaneDistance(glm::vec4& plane, glm::vec3& point);

	glm::vec4 planes[6];
};

///3D Camera with a Perspective View
class V3DPerspectiveCamera : public V3DCamera
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef V3DCamera VSUPERCLASS;

	/**
	* @param pos Initial Camera Position.
	* @param fov Field of View Angle.
	* @param aspectRatio The Aspect Ratio of the Camera (Width/Height)
	* @param zNear The near z value for the projection matrix.
	* @param zFar The far z value for the projection matrix.
	*/
	V3DPerspectiveCamera(const sf::Vector3f& pos, float fov, float aspectRatio, float zNear = 0.1f, float zFar = 500.0f);
	virtual glm::mat4 ProjectionMatrix() const;

	///@return Current aspect ratio.
	float GetAspectRatio();
	///@param value New aspect ratio.
	void SetAspectRatio(float value);
	///@return Field of View Angle.
	float GetFOV();
	///@param value New Field of View Angle.
	void SetFOV(float value);

private:
	float aspect;
	float fov;
};

///3D Camera with an Orthographic View.
class V3DOrthographicCamera : public V3DCamera
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef V3DCamera VSUPERCLASS;
	
	/**
	* @param pos Initial Camera Position.
	* @param width Camera View Width.
	* @param height Camera View Height.
	* @param zNear The near z value for the projection matrix.
	* @param zFar The far z value for the projection matrix.
	*/
	V3DOrthographicCamera(const sf::Vector3f& pos, float width, float height, float zNear = 0.1f, float zFar = 500.0f);
	virtual glm::mat4 ProjectionMatrix() const;

	///@return The current camera view dimensions.
	sf::Vector2f GetViewDimensions();
	///@param value New camera view dimensions.
	void SetViewDimensions(const sf::Vector2f& value);

private:
	sf::Vector2f viewDimensions;
};
#endif