#pragma once
#include "VRenderGroup.h"
#include <SFML/OpenGL.hpp>
#include <array>

struct V3DLight
{
	GLfloat Ambient[4];
	GLfloat Diffuse[4];
	GLfloat Specular[4];
	GLfloat Position[4];
};

class V3DCamera
{
public:
	float ZNear = 1.0f;
	float ZFar = 500.0f;

	sf::Vector3f Position;
	sf::Vector3f Rotate;

	V3DCamera(float Near = 1.0f, float Far = 500.0f);
	virtual void SetupView(const sf::Vector2f& ViewSize) {};
	void UpdateTransform();
};

class V3DOrthographicCamera : public V3DCamera
{
public:
	V3DOrthographicCamera(float Near = 1.0f, float Far = 500.0f);
	virtual void SetupView(const sf::Vector2f& ViewSize) override;
};

class V3DPerspectiveCamera : public V3DCamera
{
public:
	float POV = 45.0f;

	V3DPerspectiveCamera(float POVAngle, float Near = 1.0f, float Far = 500.0f);
	virtual void SetupView(const sf::Vector2f& ViewSize) override;
};

class V3DScene : public VRenderGroup
{
protected:
	std::array<V3DLight*, GL_LIGHT7 - GL_LIGHT0 + 1> lights;
	GLfloat globalLight[4];
	void SetupScene();

public:
	typedef VRenderGroup VSUPERCLASS;

	V3DScene(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize = 0);
	V3DScene(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize = 0);

	std::unique_ptr<V3DCamera> Camera;

	/*
	Sets a light to the scene.
	id: specifies which one of 8 lights to set.
	Ambient: Set ambient colour of light.
	Diffuse: Set diffuse colour of light.
	Specular: Set diffuse colour of light.
	Position: Sets position (or direction) of light.
	*/
	void SetLight(GLenum id, sf::Color Ambient, sf::Color Diffuse, sf::Color Specular, sf::Vector3f Position, bool Directional = true);
	V3DLight* GetLight(GLenum id);

	/*
	Destroys the light data for a specific light object.
	*/
	void DeactivateLight(GLenum id);

	//Sets the colour of the global light.
	void SetGlobalLight(sf::Color Ambient);
	GLfloat* GetGlobalLight();

	virtual const sf::Texture& GetTexture();

	virtual void Destroy();
	virtual void Resize(int width, int height) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

