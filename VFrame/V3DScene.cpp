#include "V3DScene.h"
#include <cstring>
#include <gl/GLU.h>

#pragma comment(lib, "glu32.lib")

V3DScene::V3DScene(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize) : VRenderGroup(x, y, width, height, maxSize)
{
	renderTex.create(width, height, true);
	postProcessTex.create(width, height, true);
	Sprite->Size = sf::Vector2f(sf::Vector2u(width, height));

	memset(lights.data(), 0, sizeof(V3DLight*) * lights.size());
}

V3DScene::V3DScene(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize) : VRenderGroup(position, size, maxSize)
{
	renderTex.create(size.x, size.y, true);
	postProcessTex.create(size.x, size.y, true);
	Sprite->Size = sf::Vector2f(size);
}

void V3DScene::SetLight(GLenum id, sf::Color Ambient, sf::Color Diffuse, sf::Color Specular, sf::Vector3f Position, bool Directional)
{
	if (lights[id - GL_LIGHT0])
	{
		DeactivateLight(id);
	}

	V3DLight* l = new V3DLight();
	l->Ambient[0] = Ambient.r / 255.0f;		l->Ambient[1] = Ambient.g / 255.0f;		l->Ambient[2] = Ambient.b / 255.0f;		l->Ambient[3] = Ambient.a / 255.0f;
	l->Diffuse[0] = Diffuse.r / 255.0f;		l->Diffuse[1] = Diffuse.g / 255.0f;		l->Diffuse[2] = Diffuse.b / 255.0f;		l->Diffuse[3] = Diffuse.a / 255.0f;
	l->Specular[0] = Specular.r / 255.0f;	l->Specular[1] = Diffuse.g / 255.0f;	l->Specular[2] = Diffuse.b / 255.0f;	l->Specular[3] = Diffuse.a / 255.0f;

	#if !(defined _WIN32 || defined __MINGW32__)
	l->Position[0] = -Position.x;			l->Position[1] = -Position.y;			l->Position[2] = -Position.z;
	#else
	l->Position[0] = Position.x;			l->Position[1] = Position.y;			l->Position[2] = Position.z;
	#endif

	if (Directional)
		l->Position[3] = 0;
	else
		l->Position[3] = 1;

	lights[id - GL_LIGHT0] = l;
}

V3DLight* V3DScene::GetLight(GLenum id)
{
	return lights[id - GL_LIGHT0];
}

void V3DScene::SetGlobalLight(sf::Color Ambient)
{
	globalLight[0] = Ambient.r / 255.0f;	globalLight[1] = Ambient.g / 255.0f;	globalLight[2] = Ambient.b / 255.0f;	globalLight[3] = Ambient.a / 255.0f;
}

GLfloat* V3DScene::GetGlobalLight()
{
	return globalLight;
}

void V3DScene::DeactivateLight(GLenum id)
{
	if (lights[id - GL_LIGHT0])
	{
		delete lights[id - GL_LIGHT0];
		lights[id - GL_LIGHT0] = nullptr;
	}
}

void V3DScene::SetupScene()
{
	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	glEnable(GL_NORMALIZE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	GL_ORDER;

	// Disable lighting
	glDisable(GL_LIGHTING);

	glViewport(0, 0, renderTex.getSize().x, renderTex.getSize().y);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	Camera->SetupView(sf::Vector2f(renderTex.getSize()));

	if (globalLight)
	{
		glEnable(GL_LIGHTING);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalLight);
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	for (int i = 0; i <= 7; i++)
	{
		if (lights[i])
		{
			glEnable(GL_LIGHTING);
			glLightfv(GL_LIGHT0 + i, GL_AMBIENT, lights[i]->Ambient);
			glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, lights[i]->Diffuse);
			glLightfv(GL_LIGHT0 + i, GL_SPECULAR, lights[i]->Specular);
			glLightfv(GL_LIGHT0 + i, GL_POSITION, lights[i]->Position);
			glEnable(GL_LIGHT0 + i);
		}
		else
		{
			glDisable(GL_LIGHT0 + i);
		}
	}
}

void V3DScene::Destroy()
{
	VSUPERCLASS::Destroy();

	for (int i = 0; i <= 7; i++)
	{
		DeactivateLight(GL_LIGHT0 + i);
	}
}

void V3DScene::Resize(int width, int height)
{
	renderTex.create(width, height, true);
	postProcessTex.create(width, height, true);
	Sprite->Size = sf::Vector2f(sf::Vector2u(width, height));
}

const sf::Texture& V3DScene::GetTexture()
{
	sf::Texture::getMaximumSize();
	renderTex.setActive(true);

	SetupScene();

	renderTex.clear(sf::Color::Transparent);
	Camera->UpdateTransform();
	VSUPERCLASS::Draw(renderTex);
	renderTex.display();

	renderTex.setActive(false);

	return renderTex.getTexture();
}

void V3DScene::Draw(sf::RenderTarget& RenderTarget)
{
	if (Camera == nullptr)
	{
		VLog("No V3DCamera set up!");
		return;
	}

	sf::Texture::getMaximumSize();
	renderTex.setActive(true);

	SetupScene();

	Camera->UpdateTransform();
	VSUPERCLASS::Draw(RenderTarget);

	renderTex.setActive(false);
	RenderTarget.resetGLStates();
}

V3DCamera::V3DCamera(float Near, float Far)
{
	ZNear = Near;
	ZFar = Far;
}

void V3DCamera::UpdateTransform()
{
	glLoadIdentity();
	glRotatef(Rotate.y, 0.0f, 1.0f, 0.0f);
	glRotatef(Rotate.x, 1.0f, 0.0f, 0.0f);
	glRotatef(Rotate.z, 0.0f, 0.0f, 1.0f);
	glTranslatef(-Position.x, -Position.y, -Position.z);
}

V3DOrthographicCamera::V3DOrthographicCamera(float Near, float Far) : V3DCamera(Near, Far)
{
}

void V3DOrthographicCamera::SetupView(const sf::Vector2f& ViewSize)
{
	glOrtho(0.0f, ViewSize.x, ViewSize.y, 0.0f, ZNear, ZFar);
}

V3DPerspectiveCamera::V3DPerspectiveCamera(float POVAngle, float Near, float Far) : V3DCamera(Near, Far)
{
	POV = POVAngle;
}

void V3DPerspectiveCamera::SetupView(const sf::Vector2f& ViewSize)
{
	gluPerspective(POV, ViewSize.x / ViewSize.y, ZNear, ZFar);
}
