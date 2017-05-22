#pragma once
#include "VRenderGroup.h"
#include "depend/glew.h"
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

	enum CameraType
	{
		CAMERA_FRUSTUM,
		CAMERA_ORTHO,
		NUM_CAMERA_TYPES
	};

	V3DScene(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize = 0);
	V3DScene(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize = 0);

	void SetLight(GLenum id, sf::Color Ambient, sf::Color Diffuse, sf::Color Specular, sf::Vector3f Position);
	V3DLight* GetLight(GLenum id);
	void DeactivateLight(GLenum id);

	void SetGlobalLight(sf::Color Ambient);
	GLfloat* GetGlobalLight();

	void SetViewCamera(CameraType type, float znear, float zfar);

	virtual const sf::Texture& GetTexture();

	virtual void Destroy();
	virtual void Resize(int width, int height) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

