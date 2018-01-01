#include "V3DScene.h"
#include <cstring>

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

void V3DScene::SetLight(GLenum id, sf::Color Ambient, sf::Color Diffuse, sf::Color Specular, sf::Vector3f Position)
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

void V3DScene::SetViewCamera(CameraType type, float znear, float zfar)
{
	cameraType = type;
	zNear = znear;
	zFar = zfar;
}

void V3DScene::SetupScene()
{
	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	glEnable(GL_NORMALIZE);

	glDisable(GL_CULL_FACE);

	// Disable lighting
	glDisable(GL_LIGHTING);

	glViewport(0, 0, renderTex.getSize().x, renderTex.getSize().y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (cameraType == CAMERA_FRUSTUM)
	{
		GLfloat ratio = static_cast<float>(renderTex.getSize().x) / renderTex.getSize().y;
		glFrustum(-ratio, ratio, -1.f, 1.f, zNear, zFar);
	}
	else if (cameraType == CAMERA_ORTHO)
	{
		glOrtho(0, renderTex.getSize().x, renderTex.getSize().y, 0, zNear, zFar);
	}

	if (globalLight)
	{
		glEnable(GL_LIGHTING);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalLight);
	}

	for (int i = 0; i <= 7; i++)
	{
		if (lights[i])
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0 + i);
		}
		else
		{
			glDisable(GL_LIGHT0 + i);
		}
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	for (int i = 0; i <= 7; i++)
	{
		if (lights[i])
		{
			glPushMatrix();
			glLightfv(GL_LIGHT0 + i, GL_AMBIENT, lights[i]->Ambient);
			glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, lights[i]->Diffuse);
			glLightfv(GL_LIGHT0 + i, GL_SPECULAR, lights[i]->Specular);
			glLightfv(GL_LIGHT0 + i, GL_POSITION, lights[i]->Position);
			glPopMatrix();
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
	VSUPERCLASS::Draw(renderTex);
	renderTex.display();

	renderTex.setActive(false);

	return renderTex.getTexture();
}

void V3DScene::Draw(sf::RenderTarget& RenderTarget)
{
	sf::Texture::getMaximumSize();
	renderTex.setActive(true);

	SetupScene();

	VSUPERCLASS::Draw(RenderTarget);

	renderTex.setActive(false);
	RenderTarget.resetGLStates();
}
