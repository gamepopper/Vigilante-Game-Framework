#ifndef VFRAME_NO_3D
#include "V3DScene.h"
#include "V3DObject.h"
#include "V3DShader.h"
#include "V3DCamera.h"
#include <cstring>

#pragma comment(lib, "glu32.lib")

V3DScene::V3DScene(float x, float y, unsigned int width, unsigned int height, const sf::ContextSettings& settings, unsigned int maxSize) : VRenderGroup(x, y, width, height, maxSize), contextSettings(settings)
{
	postProcessTex.create(width, height);
	renderTex.create(width, height, settings);
	Sprite->Size = sf::Vector2f(sf::Vector2u(width, height));
}

V3DScene::V3DScene(sf::Vector2f position, sf::Vector2u size, const sf::ContextSettings& settings, unsigned int maxSize) : VRenderGroup(position, size, maxSize), contextSettings(settings)
{
	postProcessTex.create(size.x, size.y);
	renderTex.create(size.x, size.y, settings);
	Sprite->Size = sf::Vector2f(size);
}

#include "V3DModel.h"
void V3DScene::Destroy()
{
	VSUPERCLASS::Destroy();

	renderTex.resetGLStates();
	renderTex.setActive(false);

	if (V3DModel::DefaultTexture)
		glDeleteTextures(1, &V3DModel::DefaultTexture);
}

void V3DScene::Resize(int width, int height)
{
	postProcessTex.create(width, height);
	renderTex.create(width, height, contextSettings);
	Sprite->Size = sf::Vector2f(sf::Vector2u(width, height));
}

void V3DScene::Update(float dt)
{
	VSUPERCLASS::Update(dt);
}

const sf::Texture& V3DScene::GetTexture()
{
	sf::Texture::getMaximumSize();

	renderTex.setActive(true);
	renderTex.clear(BackgroundTint);
	glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	glCheck(glViewport(0, 0, renderTex.getSize().x, renderTex.getSize().y));

	glCheck(glEnable(GL_DEPTH_TEST));
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glCullFace(GL_BACK));

	Shader->Bind();
	Shader->Update();

	for (unsigned int i = 0; i < members.size(); i++)
	{
		V3DObject* base = dynamic_cast<V3DObject*>(members[i]);

		if (base != nullptr && base->exists && base->visible)
		{
			base->UpdateShader(Shader.get(), Camera.get());
			base->Draw(renderTex);
		}
	}

	renderTex.display();
	renderTex.setActive(false);

	return renderTex.getTexture();
}

void V3DScene::Draw(sf::RenderTarget& RenderTarget)
{
	if (!visible)
		return;

	sf::Texture texture = GetTexture();

	if (PostEffect != nullptr && VPostEffectBase::isSupported())
	{
		postProcessTex.clear(sf::Color::Transparent);
		PostEffect->Apply(renderTex.getTexture(), postProcessTex);
		postProcessTex.display();

		updateTexture(postProcessTex.getTexture());
	}
	else
	{
		updateTexture(texture);
	}

	RenderTarget.resetGLStates();
	Sprite->Draw(RenderTarget);
}
#endif