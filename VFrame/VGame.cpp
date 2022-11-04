#ifndef VFRAME_NO_3D
	#ifndef NO_GLEW
	#include <GL/glew.h>
	#endif
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
			#ifndef NO_GLEW
			glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK)
				return 2;
			#endif
		#endif

		if (!VGlobal::p()->App->isOpen())
			return 3;

		renderTarget = std::make_unique<sf::RenderTexture>();
		if (!renderTarget->create(VGlobal::p()->Width, VGlobal::p()->Height))
			return 4;

		VGlobal::p()->Frame = renderTarget.get();

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
		#ifndef NO_GLEW
		VBase::VLog("OpenGL Version: %s", glGetString(GL_VERSION));
		#endif
	#endif
	VBase::VLog("\nStarting Game: %s", title.toUtf8().c_str());

	VGlobal::p()->ChangeState(initialState);

	sf::Clock clock;
	VBase::VLog("Initialisation finished");

	try
	{
		float dt = 1.0f / VGlobal::p()->FPS;
		float end = 0.0f;
		float frameDelay = dt;

		while (VGlobal::p()->IsRunning())
		{
			VGlobal::p()->Async->ProcessSyncRequests();

			float start = clock.getElapsedTime().asSeconds();
			float passed = start - end;
			float frameRate = 1.0f / passed;
			frameRate = frameRate < 12 ? 12 : frameRate;
			float deltaScale = (VGlobal::p()->FPS / frameRate);
			end = start;

			frameDelay += passed;

			VGlobal::p()->ChangeState(nullptr);
			VGlobal::p()->PushState(nullptr);

			HandleEvents();

			if (focused)
			{
				Update(dt * deltaScale * VGlobal::p()->TimeScale);

				if (!VGlobal::p()->IfChangedState() && !VGlobal::p()->IfPushedState() && frameDelay >= dt)
				{
					PreRender();
					std::vector<VCamera*>& cameras = VGlobal::p()->CurrentState()->Cameras;
					for (unsigned int c = 0; c < cameras.size(); c++)
					{
						Render(cameras[c]);
					}
					PostRender();
					frameDelay = 0.0f;
				}
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
	VState* currentState = VGlobal::p()->CurrentState();
	while (VGlobal::p()->App->pollEvent(event))
	{
		currentState->active ? currentState->HandleEvents(event) : (void)0;
		event.type == sf::Event::Closed ? VGlobal::p()->Exit() : (void)0;

		if (VGlobal::p()->FocusPause)
		{
			if (event.type == sf::Event::LostFocus)
				focused = false;
			if (event.type == sf::Event::GainedFocus)
				focused = true;
		}
	}

	ResizeCheck();
}

void VGame::ResizeCheck()
{
	sf::Vector2u windowSize = VGlobal::p()->App->getSize();

	if (windowSize.x != VGlobal::p()->WindowWidth ||
		windowSize.y != VGlobal::p()->WindowHeight ||
		orientation != VGlobal::p()->Orientation)
	{
		orientation = VGlobal::p()->Orientation;

		VGlobal::p()->WindowWidth = windowSize.x;
		VGlobal::p()->WindowHeight = windowSize.y;
		VGlobal::p()->App->setView(sf::View(sf::FloatRect(0, 0, (float)windowSize.x, (float)windowSize.y)));

		if (orientation == VGlobal::ANGLE_NONE || orientation == VGlobal::ANGLE_180)
		{
			float sx = VGlobal::p()->App->getSize().x / (float)VGlobal::p()->Width;
			float sy = VGlobal::p()->App->getSize().y / (float)VGlobal::p()->Height;
			float scale = std::fminf(sx, sy);

			float scaleW = VGlobal::p()->Width * scale;
			float scaleH = VGlobal::p()->Height * scale;
			float scaleX = (VGlobal::p()->App->getSize().x - scaleW) / 2;
			float scaleY = (VGlobal::p()->App->getSize().y - scaleH) / 2;

			if (orientation == VGlobal::ANGLE_NONE)
			{
				vertexArray[0].position = sf::Vector2f(scaleX, scaleY);
				vertexArray[1].position = sf::Vector2f(scaleX + scaleW, scaleY);
				vertexArray[2].position = sf::Vector2f(scaleX + scaleW, scaleY + scaleH);
				vertexArray[3].position = sf::Vector2f(scaleX, scaleY + scaleH);
				VGlobal::p()->ViewportOffset = vertexArray[0].position;
			}
			else
			{
				vertexArray[2].position = sf::Vector2f(scaleX, scaleY);
				vertexArray[3].position = sf::Vector2f(scaleX + scaleW, scaleY);
				vertexArray[0].position = sf::Vector2f(scaleX + scaleW, scaleY + scaleH);
				vertexArray[1].position = sf::Vector2f(scaleX, scaleY + scaleH);
				VGlobal::p()->ViewportOffset = vertexArray[2].position;
			}
		}
		else
		{
			float sx = VGlobal::p()->App->getSize().x / (float)VGlobal::p()->Height;
			float sy = VGlobal::p()->App->getSize().y / (float)VGlobal::p()->Width;
			float scale = std::fminf(sx, sy);

			float scaleW = VGlobal::p()->Height * scale;
			float scaleH = VGlobal::p()->Width * scale;
			float scaleX = (VGlobal::p()->App->getSize().x - scaleW) / 2;
			float scaleY = (VGlobal::p()->App->getSize().y - scaleH) / 2;

			if (orientation == VGlobal::ANGLE_90)
			{
				vertexArray[3].position = sf::Vector2f(scaleX, scaleY);
				vertexArray[0].position = sf::Vector2f(scaleX + scaleW, scaleY);
				vertexArray[1].position = sf::Vector2f(scaleX + scaleW, scaleY + scaleH);
				vertexArray[2].position = sf::Vector2f(scaleX, scaleY + scaleH);
				VGlobal::p()->ViewportOffset = vertexArray[3].position;
			}
			else
			{
				vertexArray[1].position = sf::Vector2f(scaleX, scaleY);
				vertexArray[2].position = sf::Vector2f(scaleX + scaleW, scaleY);
				vertexArray[3].position = sf::Vector2f(scaleX + scaleW, scaleY + scaleH);
				vertexArray[0].position = sf::Vector2f(scaleX, scaleY + scaleH);
				VGlobal::p()->ViewportOffset = vertexArray[1].position;
			}
		}
	}
}

void VGame::Update(float dt)
{
	VGlobal::p()->Input->Update(dt);

	VState* currentState = VGlobal::p()->CurrentState();
	currentState->active ? currentState->Update(dt) : (void)0;

	std::vector<VCamera*>& cameras = currentState->Cameras;
	for (unsigned int c = 0; c < cameras.size(); c++)
	{
		cameras[c]->Update(dt);
	}

	currentState->ResetSubState();
	VSubState* subState = currentState->SubState();
	subState ? subState->Update(dt) : (void)0;
	VGlobal::p()->Music->Update(dt);
	VGlobal::p()->PostProcess != nullptr && VPostEffectBase::isSupported() ? VGlobal::p()->PostProcess->Update(dt) : (void)0;
}

void VGame::PreRender()
{
	renderTarget->clear(VGlobal::p()->BackgroundColor);
	VGlobal::p()->App->clear();

	VState* currentState = VGlobal::p()->CurrentState();

	if (currentState && currentState->visible)
		currentState->PreDraw(*VGlobal::p()->App);
}

void VGame::Render(VCamera* camera)
{
	if (!camera->Active)
		return;

	renderTarget->setView(camera->GetView());
	VState* currentState = VGlobal::p()->CurrentState();

	currentState->Draw(*renderTarget);
	currentState->SubState() ? currentState->SubState()->Draw(*renderTarget) : (void)0;

	camera->Render(*renderTarget);
}

void VGame::PostRender()
{
	renderTarget->display();
	renderTarget->isSmooth() != VGlobal::p()->Antialiasing ? renderTarget->setSmooth(VGlobal::p()->Antialiasing) : (void)0;

	sf::RenderWindow* app = VGlobal::p()->App.get();

	app->setActive(true);
	app->setVerticalSyncEnabled(VGlobal::p()->VSync);

	VGlobal::p()->RenderState.texture = &renderTarget->getTexture();
	VGlobal::p()->PostProcess && VPostEffectBase::isSupported() ?
		VGlobal::p()->PostProcess->Apply(*VGlobal::p()->RenderState.texture, *renderTarget) : (void)0;

	app->draw(vertexArray, VGlobal::p()->RenderState);

	VState* currentState = VGlobal::p()->CurrentState();
	VSubState* subState = currentState->SubState();

	currentState && currentState->visible ? currentState->PostDraw(*VGlobal::p()->App) : (void)0;
	subState && subState->visible ? subState->PostDraw(*VGlobal::p()->App) : (void)0;

	app->display();
}
