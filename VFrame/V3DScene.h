#pragma once
#include "VRenderGroup.h"
#include <SFML/OpenGL.hpp>
#include <array>

struct V3DLight
{
	GLfloat Ambient[4];
	GLfloat Diffuse[4];
	GLfloat Specular[4];
	GLfloat Position[3];
};

class V3DScene : public VRenderGroup
{
protected:
	std::array<V3DLight*, GL_LIGHT7 - GL_LIGHT0 + 1> lights;
	GLfloat globalLight[4];
	GLfloat zNear = 1.f;
	GLfloat zFar = 500.f;

	int cameraType = 0;

	void SetupScene();

public:
	typedef VRenderGroup VSUPERCLASS;

	enum CameraType : unsigned char
	{
		CAMERA_FRUSTUM,
		CAMERA_ORTHO,
		NUM_CAMERA_TYPES
	};

	V3DScene(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize = 0);
	V3DScene(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize = 0);

	/*
	Sets a light to the scene.
	id: specifies which one of 8 lights to set.
	Ambient: Set ambient colour of light.
	Diffuse: Set diffuse colour of light.
	Specular: Set diffuse colour of light.
	Position: Sets position (or direction) of light.
	*/
	void SetLight(GLenum id, sf::Color Ambient, sf::Color Diffuse, sf::Color Specular, sf::Vector3f Position);
	V3DLight* GetLight(GLenum id);
	/*
	Destroys the light data for a specific light object.
	*/
	void DeactivateLight(GLenum id);

	//Sets the colour of the global light.
	void SetGlobalLight(sf::Color Ambient);
	GLfloat* GetGlobalLight();

	/*
	Sets the main view camera of scene.
	type: Set camera to be a Frustum or Orthographic camera.
	znear: Set near bounderies of camera.
	zfar: Set far bounderies of camera.
	*/
	void SetViewCamera(CameraType type, float znear, float zfar);

	virtual const sf::Texture& GetTexture();

	virtual void Destroy();
	virtual void Resize(int width, int height) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

