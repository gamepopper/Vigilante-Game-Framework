#include "V3DScene.h"
#include "V3DObject.h"
#include "V3DShader.h"
#include "V3DCamera.h"
#include <cstring>

#pragma comment(lib, "glu32.lib")

V3DScene::V3DScene(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize) : VRenderGroup(x, y, width, height, maxSize)
{
	postProcessTex.create(width, height);
	renderTex.create(width, height, true);
	Sprite->Size = sf::Vector2f(sf::Vector2u(width, height));
}

V3DScene::V3DScene(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize) : VRenderGroup(position, size, maxSize)
{
	postProcessTex.create(size.x, size.y);
	renderTex.create(size.x, size.y, true);
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
	renderTex.create(width, height, true);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, renderTex.getSize().x, renderTex.getSize().y);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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

	GetTexture();

	if (PostEffect != nullptr && VPostEffectBase::isSupported())
	{
		postProcessTex.clear(sf::Color::Transparent);
		PostEffect->Apply(renderTex, postProcessTex);
		postProcessTex.display();

		updateTexture(postProcessTex.getTexture());
	}
	else
	{
		updateTexture(renderTex.getTexture());
	}

	Sprite->Draw(RenderTarget);
	RenderTarget.resetGLStates();
}