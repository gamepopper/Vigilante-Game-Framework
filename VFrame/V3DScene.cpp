#ifndef VFRAME_NO_3D
#include "V3DScene.h"
#include "V3DObject.h"
#include "V3DBatchModelGroup.h"
#include "V3DShader.h"
#include "V3DCamera.h"
#include <cstring>

V3DScene::V3DScene(float x, float y, unsigned int width, unsigned int height, const sf::ContextSettings& settings, unsigned int maxSize) : VRenderGroup(maxSize), contextSettings(settings)
{
	renderTex.create(width, height, settings);
	Sprite->Position = sf::Vector2f(x, y);
	Sprite->Size = sf::Vector2f(sf::Vector2u(width, height));
}

V3DScene::V3DScene(sf::Vector2f position, sf::Vector2u size, const sf::ContextSettings& settings, unsigned int maxSize) : VRenderGroup(maxSize), contextSettings(settings)
{
	renderTex.create(size.x, size.y, settings);
	Sprite->Position = position;
	Sprite->Size = sf::Vector2f(size);
}

void V3DScene::SetActive(bool value)
{
	renderTex.setActive(value);
}

void V3DScene::PushGLStates()
{
	renderTex.pushGLStates();
}

void V3DScene::PopGLStates()
{
	renderTex.popGLStates();
}

void V3DScene::ResetGLStates()
{
	renderTex.resetGLStates();
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

void V3DScene::RenderGroup(VGroup* group)
{
	for (int i = 0; i < group->Length(); i++)
	{
		V3DObject* base = dynamic_cast<V3DObject*>(group->GetGroupItem(i));

		if (base != nullptr && base->exists && base->visible)
		{
			if (Camera[CurrentCamera]->BoxInView(base->Position, base->GetMinimum(), base->GetMaximum()))
			{
				base->UpdateShader(Shader.get(), Camera[CurrentCamera].get());
				base->Draw(renderTex); //If Renderable 3D Object
			}
		}
		else
		{
			V3DBatchModelGroup* batchGroup = dynamic_cast<V3DBatchModelGroup*>(group->GetGroupItem(i));

			if (batchGroup != nullptr)
			{
				batchGroup->UpdateShader(Camera[CurrentCamera].get(), Shader.get()); //If RenderBatch Group
				batchGroup->Draw(renderTex);
			}
			else
			{
				VGroup* childGroup = dynamic_cast<VGroup*>(group->GetGroupItem(i));
				if (childGroup != nullptr)
				{
					RenderGroup(childGroup); //If regular group.
				}
				else
				{
					group->GetGroupItem(i)->Draw(renderTex); //If nothing else, just draw it.
				}
			}
		}
	}
}

const sf::Texture& V3DScene::GetTexture()
{
	sf::Texture::getMaximumSize();

	renderTex.setActive(true);
	renderTex.clear(BackgroundTint);

	glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	glCheck(glEnable(GL_DEPTH_TEST));
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glCullFace(GL_BACK));
	for (CurrentCamera = 0; CurrentCamera < Camera.size(); CurrentCamera++)
	{
		sf::IntRect Viewport(
			(int)(Camera[CurrentCamera]->Viewport.left		* renderTex.getSize().x),
			(int)(Camera[CurrentCamera]->Viewport.top		* renderTex.getSize().y),
			(int)(Camera[CurrentCamera]->Viewport.width		* renderTex.getSize().x),
			(int)(Camera[CurrentCamera]->Viewport.height	* renderTex.getSize().y)
		);

		glCheck(glViewport(Viewport.left, Viewport.top, Viewport.width, Viewport.height));
		Shader->SetCamera(Camera[CurrentCamera].get());
		Shader->Update();
		Shader->Bind();

		RenderGroup(this);
	}

	renderTex.display();
	renderTex.setActive(false);

	return renderTex.getTexture();
}

const sf::Texture V3DScene::GetTexture(V3DShader* shader, V3DCamera* camera)
{
	std::unique_ptr<V3DShader> uShader(shader);
	std::unique_ptr<V3DCamera> uCamera(camera);

	if (Shader.get() != shader)
		Shader.swap(uShader);

	if (Camera[CurrentCamera].get() != camera)
		Camera[CurrentCamera].swap(uCamera);

	GetTexture();

	if (Shader.get() != shader)
		Shader.swap(uShader);

	if (Camera[CurrentCamera].get() != camera)
		Camera[CurrentCamera].swap(uCamera);

	uShader.release();
	uCamera.release();

	return renderTex.getTexture();
}

void V3DScene::Draw(sf::RenderTarget& RenderTarget)
{
	if (!visible)
		return;

	GetTexture();

	RenderTarget.resetGLStates();
	if (PostEffect != nullptr && VPostEffectBase::isSupported())
	{
		if (postProcessTex.getSize().x == 0 || postProcessTex.getSize().y == 0)
		{
			postProcessTex.create(renderTex.getSize().x, renderTex.getSize().y);
		}

		PostEffect->Apply(renderTex.getTexture(), postProcessTex);
		postProcessTex.display();

		updateTexture(postProcessTex.getTexture());
	}
	else
	{
		updateTexture(renderTex.getTexture());
	}

	Sprite->Draw(RenderTarget);
}
#endif