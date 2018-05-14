#ifndef VFRAME_NO_3D
#include <GL/glew.h>
#endif

#include "VGame.h"
#include "VCamera.h"
#include "VGlobal.h"
#include "VTimer.h"
#include "VPostEffect.h"
#include "VState.h"

#include <SFML/System/Clock.hpp>

VGame::~VGame()
{
	if (!cleaned)
	{
		Cleanup();
	}
}

int VGame::Init()
{
	VBase::VLog("Calling Init()");

	try
	{
		#ifndef VFRAME_NO_3D
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			return 2;
		#endif

		if (!VGlobal::p()->App->isOpen())
			return 3;

		renderTarget = std::unique_ptr<sf::RenderTexture>(new sf::RenderTexture());
		if (!renderTarget->create(VGlobal::p()->Width, VGlobal::p()->Height))
			return 4;

		VGlobal::p()->WorldBounds = sf::FloatRect(0, 0, static_cast<float>(VGlobal::p()->Width), static_cast<float>(VGlobal::p()->Height));
		VGlobal::p()->App->requestFocus();
		VCameraList::Default = renderTarget->getDefaultView();

		vertexArray.resize(4);
		vertexArray.setPrimitiveType(sf::Quads);
		vertexArray[0] = sf::Vertex(sf::Vector2f(), sf::Color::White, sf::Vector2f());
		vertexArray[1] = sf::Vertex(sf::Vector2f(VGlobal::p()->WorldBounds.width, 0.0f), 
			sf::Color::White, 
			sf::Vector2f(VGlobal::p()->WorldBounds.width, 0.0f));
		vertexArray[2] = sf::Vertex(sf::Vector2f(VGlobal::p()->WorldBounds.width, VGlobal::p()->WorldBounds.height), 
			sf::Color::White, 
			sf::Vector2f(VGlobal::p()->WorldBounds.width, VGlobal::p()->WorldBounds.height));
		vertexArray[3] = sf::Vertex(sf::Vector2f(0.0f, VGlobal::p()->WorldBounds.height), 
			sf::Color::White, 
			sf::Vector2f(0.0f, VGlobal::p()->WorldBounds.height));

		ResizeCheck();
	}
	catch (int e)
	{
		return e;
	}

	return EXIT_SUCCESS;
}

int VGame::Cleanup()
{
	VBase::VLog("Calling Cleanup()");

	try
	{
		VGlobal::Cleanup();
		cleaned = true;
	}
	catch (int e)
	{
		VBase::VLog("Cleanup error: %d", e);
		return e;
	}

	return EXIT_SUCCESS;
}

int VGame::Run(const sf::String& title, VState* initialState, int windowwidth, int windowheight, float fps, int flags, const sf::ContextSettings& settings)
{
	return Run(title, initialState, windowwidth, windowheight, windowwidth, windowheight, fps, flags, settings);
}

int VGame::Run(const sf::String& title, VState* initialState, int windowwidth, int windowheight, int screenwidth, int screenheight, float fps, int flags, const sf::ContextSettings& settings)
{
	VGlobal::p()->Title = title;
	VGlobal::p()->Width = screenwidth;
	VGlobal::p()->Height = screenheight;
	VGlobal::p()->FPS = fps;
	VGlobal::p()->WindowStyle = flags;
	VGlobal::p()->ContextSettings = settings;
	VGlobal::p()->RenderState = sf::RenderStates::Default;

	VGlobal::p()->App->create(sf::VideoMode(windowwidth, windowheight), title, flags, settings);

	VBase::VLog("Welcome to the ViglanteFramework - Version: %s ", VFRAME_VERSION);

	int error = 0;
	if ((error = Init()))
	{
		VBase::VLogError("Error in Init(): %d", error);
		return error;
	}

	#ifndef VFRAME_NO_3D
	VBase::VLog("OpenGL Version: %s", glGetString(GL_VERSION));
	#endif
	VBase::VLog("\nStarting Game: %s", title.toUtf8().c_str());

	initialState->DefaultCamera->Reset();
	VGlobal::p()->ChangeState(initialState);
	VGlobal::p()->ChangeState(nullptr);

	sf::Clock clock;
	VBase::VLog("Initialisation finished");

	try
	{
		double end = 0.0;

		while (VGlobal::p()->App->isOpen())
		{
			VGlobal::p()->Async->ProcessSyncRequests();

			float dt = 1.0f / VGlobal::p()->FPS;
			double start = clock.getElapsedTime().asSeconds();
			double passed = start - end;
			float frameRate = (float)(1.0 / passed);
			frameRate = frameRate < 12 ? 12 : frameRate;
			float deltaScale = (VGlobal::p()->FPS / frameRate);
			end = start;

			if (VGlobal::p()->IfChangedState)
			{
				VGlobal::p()->ChangeState(nullptr);
			}

			if (VGlobal::p()->IfPushedState)
			{
				VGlobal::p()->PushState(nullptr);
			}

			HandleEvents();
			if (focused)
			{
				Update(dt * deltaScale * VGlobal::p()->TimeScale);
			}

			if (focused && !VGlobal::p()->IfChangedState && !VGlobal::p()->IfPushedState)
			{
				PreRender();
				for (unsigned int c = 0; c < VGlobal::p()->CurrentState()->Cameras.size(); c++)
				{
					Render(VGlobal::p()->CurrentState()->Cameras[c]);
				}
				PostRender();
			}
		}
	}
	catch (int e)
	{
		return e;
	}

	if ((error = Cleanup()))
	{
		VBase::VLogError("Error in Cleanup(): %d", error);
		return error;
	}

	VBase::VLog("Cleanup Successful");

	return EXIT_SUCCESS;
}

void VGame::HandleEvents()
{
	sf::Event event;
	while (VGlobal::p()->App->pollEvent(event))
	{
		if (VGlobal::p()->CurrentState()->SubState)
		{
			VGlobal::p()->CurrentState()->SubState->HandleEvents(event);
		}

		VGlobal::p()->CurrentState()->HandleEvents(event);
		if (event.type == sf::Event::Closed)
		{
			VGlobal::p()->Exit();
		}
		if (VGlobal::p()->FocusPause)
		{
			if (event.type == sf::Event::LostFocus)
			{
				focused = false;
			}
			if (event.type == sf::Event::GainedFocus)
			{
				focused = true;
			}
		}
	}
	ResizeCheck();
}

void VGame::ResizeCheck()
{
	sf::Vector2u windowSize = VGlobal::p()->App->getSize();

	if (windowSize.x != VGlobal::p()->WindowWidth || windowSize.y != VGlobal::p()->WindowHeight)
	{
		VGlobal::p()->WindowWidth = windowSize.x;
		VGlobal::p()->WindowHeight = windowSize.y;
		VGlobal::p()->App->setView(sf::View(sf::FloatRect(0, 0, (float)windowSize.x, (float)windowSize.y)));

		float sx = VGlobal::p()->App->getSize().x / (float)VGlobal::p()->Width;
		float sy = VGlobal::p()->App->getSize().y / (float)VGlobal::p()->Height;
		float scale = std::fminf(sx, sy);

		float scaleW = VGlobal::p()->Width * scale;
		float scaleH = VGlobal::p()->Height * scale;
		float scaleX = (VGlobal::p()->App->getSize().x - scaleW) / 2;
		float scaleY = (VGlobal::p()->App->getSize().y - scaleH) / 2;
		vertexArray[0].position = sf::Vector2f(scaleX, scaleY);
		vertexArray[1].position = sf::Vector2f(scaleX + scaleW, scaleY);
		vertexArray[2].position = sf::Vector2f(scaleX + scaleW, scaleY + scaleH);
		vertexArray[3].position = sf::Vector2f(scaleX, scaleY + scaleH);
		VGlobal::p()->ViewportOffset = vertexArray[0].position;
	}
}

void VGame::Update(float dt)
{
	if (VTimeManager::AnyActiveTimers())
	{
		VTimeManager::p()->Update(dt);
	}

	VGlobal::p()->Input->Update(dt);

	if (VGlobal::p()->CurrentState()->active)
	{
		VGlobal::p()->CurrentState()->Update(dt);
	}

	VGlobal::p()->CurrentState()->ResetSubState();

	if (VGlobal::p()->CurrentState()->SubState)
	{
		VGlobal::p()->CurrentState()->SubState->Update(dt);
	}

	for (unsigned int c = 0; c < VGlobal::p()->CurrentState()->Cameras.size(); c++)
	{
		VGlobal::p()->CurrentState()->Cameras[c]->Update(dt);
	}

	VGlobal::p()->Music->Update(dt);

	if (VGlobal::p()->PostProcess != nullptr && VPostEffectBase::isSupported())
	{
		VGlobal::p()->PostProcess->Update(dt);
	}
}

void VGame::PreRender()
{
	renderTarget->clear(VGlobal::p()->BackgroundColor);
	VGlobal::p()->App->clear();
}

void VGame::Render(VCamera* camera)
{
	if (!camera->Active)
		return;

	renderTarget->setView(camera->GetView());
	VState* currentState = VGlobal::p()->CurrentState();

	if (currentState && currentState->visible)
		currentState->Draw(*renderTarget);

	if (currentState->SubState && currentState->SubState->visible)
		currentState->SubState->Draw(*renderTarget);

	camera->Render(*renderTarget);
}

void VGame::PostRender()
{
	renderTarget->display();
	if (renderTarget->isSmooth() != VGlobal::p()->Antialiasing)
	{
		renderTarget->setSmooth(VGlobal::p()->Antialiasing);
	}

	sf::RenderWindow* app = VGlobal::p()->App.get();

	app->setActive(true);
	app->setVerticalSyncEnabled(VGlobal::p()->VSync);

	sf::View view = app->getView();
	if (VGlobal::p()->PostProcess == nullptr || !VPostEffectBase::isSupported())
	{
		view.setViewport(sf::FloatRect(0, 0, 1, 1));
		VGlobal::p()->RenderState.texture = &renderTarget->getTexture();
		app->draw(vertexArray, VGlobal::p()->RenderState);
	}
	else
	{
		sf::Vector2f position = VGlobal::p()->ViewportOffset;
		sf::Vector2f size = view.getSize();

		float left = position.x / size.x;
		float top = position.y / size.y;
		view.setViewport(sf::FloatRect(left, top, 1 - (left * 2), 1 - (top * 2)));

		VGlobal::p()->PostProcess->Apply(*renderTarget, *app);
	}

	app->setView(view);

	VState* currentState = VGlobal::p()->CurrentState();

	if (currentState && currentState->visible)
		currentState->PostDraw(*VGlobal::p()->App);

	if (currentState->SubState && currentState->SubState->visible)
		currentState->SubState->PostDraw(*VGlobal::p()->App);

	app->display();
}
