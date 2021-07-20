﻿#include "States.h"

enum DemoPages
{
	PAGE_SPRITE,
	PAGE_TILED,
	PAGE_9SLICE,
	PAGE_TEXT,
	PAGE_TEXTPATH,
	PAGE_CONTROLLER,
	PAGE_TILEMAP,
	PAGE_COLLISION,
	PAGE_INTERPOLATE,
	PAGE_PARTICLES,
	PAGE_TRAILAREA,
	PAGE_PATH,
#ifndef VFRAME_NO_PHYSICS
	PAGE_PHYSICS,
#endif
#ifndef VFRAME_NO_3D
	PAGE_GRAPHICS3D,
#endif
#ifndef VFRAME_NO_CANDLE
	PAGE_CANDLE,
#endif
	PAGE_ASCII,
	PAGE_LIGHTING2D,
	PAGE_BACKDROP,
	PAGE_ASYNC,
	NUM_DEMOPAGES,
};

void DemoStatesManager::SetNewPage()
{
	VGlobal::p()->PostProcess.reset();

	VSubState* subState;
	switch (CurrentPage)
	{
		case PAGE_9SLICE:
			subState = new NineSliceState();
			break;
		case PAGE_ASCII:
			subState = new ASCIITestState();
			break;
		case PAGE_ASYNC:
			subState = new AsyncTestState();
			break;
		case PAGE_BACKDROP:
			subState = new BackdropState();
			break;
#ifndef VFRAME_NO_CANDLE
		case PAGE_CANDLE:
			subState = new CandleState();
			break;
#endif
		case PAGE_COLLISION:
			subState = new CollisionState();
			break;
		case PAGE_CONTROLLER:
			subState = new ControllerState();
			break;
#ifndef VFRAME_NO_3D
		case PAGE_GRAPHICS3D:
			subState = new Graphics3DState();
			break;
#endif
		case PAGE_INTERPOLATE:
			subState = new InterpolateState();
			break;
		case PAGE_LIGHTING2D:
			subState = new Lighting2DState();
			break;
		case PAGE_PATH:
			subState = new PathState();
			break;
		case PAGE_PARTICLES:
			subState = new ParticlesState();
			break;
#ifndef VFRAME_NO_PHYSICS
		case PAGE_PHYSICS:
			subState = new PhysicsState();
			break;
#endif
		case PAGE_SPRITE:
			subState = new SpriteState();
			break;
		case PAGE_TEXT:
			subState = new TextState();
			break;
		case PAGE_TEXTPATH:
			subState = new WordShapeState();
			break;
		case PAGE_TILED:
			subState = new TiledState();
			break;
		case PAGE_TILEMAP:
			subState = new TilemapState();
			break;
		case PAGE_TRAILAREA:
			subState = new TrailAreaState();
			break;
		default:
			subState = NULL;
			break;
	}

	if (SubState() != nullptr)
	{
		if (SubState()->GetIndexOfItem(stateText) >= 0)
		{
			SubState()->Remove(stateText);
		}
	}

	OpenSubState(subState);

	Cameras[0]->Reset();
	VGlobal::p()->BackgroundColor = sf::Color::Black;

	ResetSubState();
	subState->Add(stateText);
}

void DemoStatesManager::Initialise()
{
	VSUPERCLASS::Initialise();

	auto title = new VText(sf::Vector2f(0.0f, 2.0f), (float)VGlobal::p()->Width, "SFML VFrame", 24);
	title->SetAlignment(VText::ALIGNCENTRE);
	title->ScrollFactor = sf::Vector2f();
	title->ZoomFactor = 0;

	std::stringstream ss;
	ss << "Version: " << VFRAME_VERSION;
	auto version = new VText(5.0f, 10.0f, (float)VGlobal::p()->Width - 10.0f, ss.str(), 12);
	version->SetAlignment(VText::ALIGNRIGHT);
	version->ScrollFactor = sf::Vector2f();
	version->ZoomFactor = 0;

	auto header = new VShape();
	header->SetRectangle((float)VGlobal::p()->Width, (float)(title->Position.y + 32));
	header->SetFillTint(sf::Color::Black);
	header->ScrollFactor = sf::Vector2f();
	header->ZoomFactor = 0;

	stateText = new VGroup(3);
	stateText->Add(header);
	stateText->Add(title);
	stateText->Add(version);

	SetNewPage();
	SubState()->Add(stateText);

	if (VGlobal::p()->Music->Status() != sf::Music::Playing)
	{
		VGlobal::p()->Music->OpenMusicFile("Example/Assets/game.ogg");
		VGlobal::p()->Music->SetLoop(true);
		VGlobal::p()->Music->SetVolume(50.0f);
		VGlobal::p()->Music->SetPitch(0.5f);
		VGlobal::p()->Music->Play();
	}

	VGlobal::p()->Sound->Load("Example/Assets/menuSelect.ogg", "select");
	VGlobal::p()->Sound->Load("Example/Assets/menuSwitch.ogg", "switch");

#ifdef USE_GAMEPAD_API
	VGlobal::p()->Input->SetAxisInput("leftX", sf::Keyboard::D, sf::Keyboard::A, VInputHandler::XAxis::PovX);
	VGlobal::p()->Input->SetAxisInput("leftY", sf::Keyboard::S, sf::Keyboard::W, VInputHandler::XAxis::PovY);
	VGlobal::p()->Input->SetAxisInput("rightX", sf::Keyboard::L, sf::Keyboard::J, VInputHandler::XAxis::Z);
	VGlobal::p()->Input->SetAxisInput("rightY", sf::Keyboard::K, sf::Keyboard::I, VInputHandler::XAxis::V);
	VGlobal::p()->Input->SetAxisInput("LT", sf::Keyboard::E, sf::Keyboard::Q, VInputHandler::XAxis::L);
	VGlobal::p()->Input->SetAxisInput("RT", sf::Keyboard::O, sf::Keyboard::U, VInputHandler::XAxis::R);
	VGlobal::p()->Input->SetButtonInput("A", sf::Keyboard::N, BUTTON_A);
	VGlobal::p()->Input->SetButtonInput("B", sf::Keyboard::M, BUTTON_B);
	VGlobal::p()->Input->SetButtonInput("X", sf::Keyboard::H, BUTTON_X);
	VGlobal::p()->Input->SetButtonInput("Y", sf::Keyboard::J, BUTTON_Y);
	VGlobal::p()->Input->SetButtonInput("leftShoulder", sf::Keyboard::T, BUTTON_LEFT_SHOULDER);
	VGlobal::p()->Input->SetButtonInput("rightShoulder", sf::Keyboard::Y, BUTTON_RIGHT_SHOULDER);
	VGlobal::p()->Input->SetButtonInput("leftStick", sf::Keyboard::B, BUTTON_LEFT_THUMB);
	VGlobal::p()->Input->SetButtonInput("rightStick", sf::Keyboard::N, BUTTON_RIGHT_THUMB);
	VGlobal::p()->Input->SetButtonInput("Back", sf::Keyboard::BackSpace, BUTTON_BACK);
	VGlobal::p()->Input->SetButtonInput("Start", sf::Keyboard::Return, BUTTON_START);
#elif defined(USE_SFML_JOYSTICK)
	VGlobal::p()->Input->SetAxisInput("leftX", sf::Keyboard::D, sf::Keyboard::A, sf::Joystick::Axis::X);
	VGlobal::p()->Input->SetAxisInput("leftY", sf::Keyboard::S, sf::Keyboard::W, sf::Joystick::Axis::Y);
	VGlobal::p()->Input->SetAxisInput("rightX", sf::Keyboard::L, sf::Keyboard::J, sf::Joystick::Axis::U);
	VGlobal::p()->Input->SetAxisInput("rightY", sf::Keyboard::K, sf::Keyboard::I, sf::Joystick::Axis::R);
	VGlobal::p()->Input->SetAxisInput("LT", sf::Keyboard::E, sf::Keyboard::Q, sf::Joystick::Axis::Z);
	VGlobal::p()->Input->SetAxisInput("RT", sf::Keyboard::O, sf::Keyboard::U, sf::Joystick::Axis::Z);
	VGlobal::p()->Input->SetButtonInput("A", sf::Keyboard::N, VInputHandler::BUTTON_A);
	VGlobal::p()->Input->SetButtonInput("B", sf::Keyboard::M, VInputHandler::BUTTON_B);
	VGlobal::p()->Input->SetButtonInput("X", sf::Keyboard::H, VInputHandler::BUTTON_X);
	VGlobal::p()->Input->SetButtonInput("Y", sf::Keyboard::J, VInputHandler::BUTTON_Y);
	VGlobal::p()->Input->SetButtonInput("leftShoulder", sf::Keyboard::T, VInputHandler::BUTTON_LEFT_SHOULDER);
	VGlobal::p()->Input->SetButtonInput("rightShoulder", sf::Keyboard::Y, VInputHandler::BUTTON_RIGHT_SHOULDER);
	VGlobal::p()->Input->SetButtonInput("leftStick", sf::Keyboard::B, VInputHandler::BUTTON_LEFT_THUMB);
	VGlobal::p()->Input->SetButtonInput("rightStick", sf::Keyboard::N, VInputHandler::BUTTON_RIGHT_THUMB);
	VGlobal::p()->Input->SetButtonInput("Back", sf::Keyboard::BackSpace, VInputHandler::BUTTON_BACK);
	VGlobal::p()->Input->SetButtonInput("Start", sf::Keyboard::Return, VInputHandler::BUTTON_START);
	VGlobal::p()->Input->SetButtonInput("Home", sf::Keyboard::Home, VInputHandler::BUTTON_HOME);
	VGlobal::p()->Input->SetDeadzone(20.0f);
#else
	VGlobal::p()->Input->SetAxisInput("leftX", sf::Keyboard::D, sf::Keyboard::A, sf::XInputDevice::XAxis::PovX);
	VGlobal::p()->Input->SetAxisInput("leftY", sf::Keyboard::S, sf::Keyboard::W, sf::XInputDevice::XAxis::PovY);
	VGlobal::p()->Input->SetAxisInput("rightX", sf::Keyboard::L, sf::Keyboard::J, sf::XInputDevice::XAxis::Z);
	VGlobal::p()->Input->SetAxisInput("rightY", sf::Keyboard::K, sf::Keyboard::I, sf::XInputDevice::XAxis::V);
	VGlobal::p()->Input->SetAxisInput("LT", sf::Keyboard::E, sf::Keyboard::Q, sf::XInputDevice::XAxis::L);
	VGlobal::p()->Input->SetAxisInput("RT", sf::Keyboard::O, sf::Keyboard::U, sf::XInputDevice::XAxis::R);
	VGlobal::p()->Input->SetButtonInput("A", sf::Keyboard::N, sf::XInputDevice::A);
	VGlobal::p()->Input->SetButtonInput("B", sf::Keyboard::M, sf::XInputDevice::B);
	VGlobal::p()->Input->SetButtonInput("X", sf::Keyboard::H, sf::XInputDevice::X);
	VGlobal::p()->Input->SetButtonInput("Y", sf::Keyboard::J, sf::XInputDevice::Y);
	VGlobal::p()->Input->SetButtonInput("leftShoulder", sf::Keyboard::T, sf::XInputDevice::LB);
	VGlobal::p()->Input->SetButtonInput("rightShoulder", sf::Keyboard::Y, sf::XInputDevice::RB);
	VGlobal::p()->Input->SetButtonInput("leftStick", sf::Keyboard::B, sf::XInputDevice::LEFT_THUMB);
	VGlobal::p()->Input->SetButtonInput("rightStick", sf::Keyboard::N, sf::XInputDevice::RIGHT_THUMB);
	VGlobal::p()->Input->SetButtonInput("Back", sf::Keyboard::BackSpace, sf::XInputDevice::BACK);
	VGlobal::p()->Input->SetButtonInput("Start", sf::Keyboard::Return, sf::XInputDevice::START);
#endif
}

void DemoStatesManager::HandleEvents(const sf::Event& event)
{
	VSUPERCLASS::HandleEvents(event);

	if (event.type == sf::Event::KeyPressed)
	{
		if (!VGlobal::p()->Async->ActiveAsyncFunctions())
		{
			if (event.key.code == sf::Keyboard::Left)
			{
				CurrentPage--;

				if (CurrentPage < 0)
					CurrentPage = NUM_DEMOPAGES - 1;

				VGlobal::p()->Sound->Play("switch");

				SetNewPage();
			}

			if (event.key.code == sf::Keyboard::Right)
			{
				CurrentPage++;
				CurrentPage %= NUM_DEMOPAGES;

				VGlobal::p()->Sound->Play("switch");

				SetNewPage();
			}
		}

#ifdef _DEBUG
		if (event.key.code == sf::Keyboard::Q)
		{
			VGlobal::p()->DrawDebug = !VGlobal::p()->DrawDebug;
			VGlobal::p()->Sound->Play("select");
		}
#endif

		if (event.key.code == sf::Keyboard::E)
		{
			VGlobal::p()->Antialiasing = !VGlobal::p()->Antialiasing;
			VGlobal::p()->Sound->Play("select");
		}

		if (event.key.code == sf::Keyboard::F)
		{
			VGlobal::p()->ToggleFullscreen();
			VGlobal::p()->Sound->Play("select");
		}
	}
}
