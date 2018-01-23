#pragma once
#include "VRenderGroup.h"
#include <SFML/OpenGL.hpp>
#include "V3DCamera.h"
#include <array>

class V3DShader;
class V3DScene : public VRenderGroup
{
public:
	typedef VRenderGroup VSUPERCLASS;

	V3DScene(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize = 0);
	V3DScene(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize = 0);
	std::unique_ptr<V3DShader> Shader;
	std::unique_ptr<V3DCamera> Camera;

	virtual const sf::Texture& GetTexture();

	virtual void Destroy();
	virtual void Resize(int width, int height) override;
	virtual void Update(float dt);
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

