#include "States.h"

enum DemoPages
{
	PAGE_SPRITE,
	PAGE_TILED,
	PAGE_TEXT,
	PAGE_TEXTPATH,
	PAGE_CONTROLLER,
	PAGE_ASCII,
	PAGE_TILEMAP,
	PAGE_COLLISION,
	PAGE_INTERPOLATE,
	PAGE_PARTICLES,
	PAGE_TRAILAREA,
	PAGE_BACKDROP,
	PAGE_LIGHTING2D,
	PAGE_GRAPHICS3D,
	PAGE_ASYNC,
	NUM_DEMOPAGES,
};

void DemoStatesManager::SetNewPage()
{
	if (VGlobal::p()->PostProcess)
	{
		delete VGlobal::p()->PostProcess;
		VGlobal::p()->PostProcess = NULL;
	}

	VSubState* subState;
	switch (CurrentPage)
	{
		case PAGE_ASCII:
			subState = new ASCIITestState();
			break;
		case PAGE_ASYNC:
			subState = new AsyncTestState();
			break;
		case PAGE_BACKDROP:
			subState = new BackdropState();
			break;
		case PAGE_COLLISION:
			subState = new CollisionState();
			break;
		case PAGE_CONTROLLER:
			subState = new ControllerState();
			break;
		case PAGE_GRAPHICS3D:
			subState = new Graphics3DState();
			break;
		case PAGE_INTERPOLATE:
			subState = new InterpolateState();
			break;
		case PAGE_LIGHTING2D:
			subState = new Lighting2DState();
			break;
		case PAGE_PARTICLES:
			subState = new ParticlesState();
			break;
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

	auto stateText = new VGroup(3);
	auto title = new VText(sf::Vector2f(0.0f, 2.0f), (float)VGlobal::p()->Width, "SFML VFrame", 24);
	title->Alignment = VTextAlign::ALIGNCENTRE;
	title->ScrollFactor = sf::Vector2f();
	title->ZoomFactor = 0;

	std::stringstream ss;
	ss << "Version: " << VFRAME_VERSION;
	auto version = new VText(5.0f, 10.0f, (float)VGlobal::p()->Width - 10.0f, ss.str(), 12);
	version->Alignment = VTextAlign::ALIGNRIGHT;
	version->ScrollFactor = sf::Vector2f();
	version->ZoomFactor = 0;

	auto header = new VSprite();
	header->MakeGraphic(VGlobal::p()->Width, (int)(title->Position.y + 32), sf::Color::Black);
	header->ScrollFactor = sf::Vector2f();
	header->ZoomFactor = 0;

	Cameras[0]->Reset();
	VGlobal::p()->BackgroundColor = sf::Color::Black;

	OpenSubState(subState);
	ResetSubState();

	stateText->Add(header);
	stateText->Add(title);
	stateText->Add(version);
	subState->Add(stateText);
}

void DemoStatesManager::Initialise()
{
	VSUPERCLASS::Initialise();
	SetNewPage();

#ifdef USE_GAMEPAD_API
	VGlobal::p()->Input.AddAxisInput("leftX", sf::Keyboard::D, sf::Keyboard::A, VInputHandler::XAxis::PovX);
	VGlobal::p()->Input.AddAxisInput("leftY", sf::Keyboard::S, sf::Keyboard::W, VInputHandler::XAxis::PovY);
	VGlobal::p()->Input.AddAxisInput("rightX", sf::Keyboard::L, sf::Keyboard::J, VInputHandler::XAxis::Z);
	VGlobal::p()->Input.AddAxisInput("rightY", sf::Keyboard::K, sf::Keyboard::I, VInputHandler::XAxis::V);
	VGlobal::p()->Input.AddAxisInput("LT", sf::Keyboard::E, sf::Keyboard::Q, VInputHandler::XAxis::L);
	VGlobal::p()->Input.AddAxisInput("RT", sf::Keyboard::O, sf::Keyboard::U, VInputHandler::XAxis::R);
	VGlobal::p()->Input.AddButtonInput("A", sf::Keyboard::Z, BUTTON_A);
	VGlobal::p()->Input.AddButtonInput("B", sf::Keyboard::X, BUTTON_B);
	VGlobal::p()->Input.AddButtonInput("X", sf::Keyboard::C, BUTTON_X);
	VGlobal::p()->Input.AddButtonInput("Y", sf::Keyboard::V, BUTTON_Y);
	VGlobal::p()->Input.AddButtonInput("leftShoulder", sf::Keyboard::T, BUTTON_LEFT_SHOULDER);
	VGlobal::p()->Input.AddButtonInput("rightShoulder", sf::Keyboard::Y, BUTTON_RIGHT_SHOULDER);
	VGlobal::p()->Input.AddButtonInput("leftStick", sf::Keyboard::B, BUTTON_LEFT_THUMB);
	VGlobal::p()->Input.AddButtonInput("rightStick", sf::Keyboard::N, BUTTON_RIGHT_THUMB);
	VGlobal::p()->Input.AddButtonInput("Back", sf::Keyboard::BackSpace, BUTTON_BACK);
	VGlobal::p()->Input.AddButtonInput("Start", sf::Keyboard::Return, BUTTON_START);
#elif defined(USE_SFML_JOYSTICK)
	VGlobal::p()->Input.AddAxisInput("leftX", sf::Keyboard::D, sf::Keyboard::A, sf::Joystick::Axis::X);
	VGlobal::p()->Input.AddAxisInput("leftY", sf::Keyboard::S, sf::Keyboard::W, sf::Joystick::Axis::Y);
	VGlobal::p()->Input.AddAxisInput("rightX", sf::Keyboard::L, sf::Keyboard::J, sf::Joystick::Axis::U);
	VGlobal::p()->Input.AddAxisInput("rightY", sf::Keyboard::K, sf::Keyboard::I, sf::Joystick::Axis::R);
	VGlobal::p()->Input.AddAxisInput("LT", sf::Keyboard::E, sf::Keyboard::Q, sf::Joystick::Axis::Z);
	VGlobal::p()->Input.AddAxisInput("RT", sf::Keyboard::O, sf::Keyboard::U, sf::Joystick::Axis::Z);
	VGlobal::p()->Input.AddButtonInput("A", sf::Keyboard::Z, VInputHandler::BUTTON_A);
	VGlobal::p()->Input.AddButtonInput("B", sf::Keyboard::X, VInputHandler::BUTTON_B);
	VGlobal::p()->Input.AddButtonInput("X", sf::Keyboard::C, VInputHandler::BUTTON_X);
	VGlobal::p()->Input.AddButtonInput("Y", sf::Keyboard::V, VInputHandler::BUTTON_Y);
	VGlobal::p()->Input.AddButtonInput("leftShoulder", sf::Keyboard::T, VInputHandler::BUTTON_LEFT_SHOULDER);
	VGlobal::p()->Input.AddButtonInput("rightShoulder", sf::Keyboard::Y, VInputHandler::BUTTON_RIGHT_SHOULDER);
	VGlobal::p()->Input.AddButtonInput("leftStick", sf::Keyboard::B, VInputHandler::BUTTON_LEFT_THUMB);
	VGlobal::p()->Input.AddButtonInput("rightStick", sf::Keyboard::N, VInputHandler::BUTTON_RIGHT_THUMB);
	VGlobal::p()->Input.AddButtonInput("Back", sf::Keyboard::BackSpace, VInputHandler::BUTTON_BACK);
	VGlobal::p()->Input.AddButtonInput("Start", sf::Keyboard::Return, VInputHandler::BUTTON_START);
	VGlobal::p()->Input.AddButtonInput("Home", sf::Keyboard::Home, VInputHandler::BUTTON_HOME);
#else
	VGlobal::p()->Input.AddAxisInput("leftX", sf::Keyboard::D, sf::Keyboard::A, sf::XInputDevice::XAxis::PovX);
	VGlobal::p()->Input.AddAxisInput("leftY", sf::Keyboard::S, sf::Keyboard::W, sf::XInputDevice::XAxis::PovY);
	VGlobal::p()->Input.AddAxisInput("rightX", sf::Keyboard::L, sf::Keyboard::J, sf::XInputDevice::XAxis::Z);
	VGlobal::p()->Input.AddAxisInput("rightY", sf::Keyboard::K, sf::Keyboard::I, sf::XInputDevice::XAxis::V);
	VGlobal::p()->Input.AddAxisInput("LT", sf::Keyboard::E, sf::Keyboard::Q, sf::XInputDevice::XAxis::L);
	VGlobal::p()->Input.AddAxisInput("RT", sf::Keyboard::O, sf::Keyboard::U, sf::XInputDevice::XAxis::R);
	VGlobal::p()->Input.AddButtonInput("A", sf::Keyboard::Z, sf::XInputDevice::A);
	VGlobal::p()->Input.AddButtonInput("B", sf::Keyboard::X, sf::XInputDevice::B);
	VGlobal::p()->Input.AddButtonInput("X", sf::Keyboard::C, sf::XInputDevice::X);
	VGlobal::p()->Input.AddButtonInput("Y", sf::Keyboard::V, sf::XInputDevice::Y);
	VGlobal::p()->Input.AddButtonInput("leftShoulder", sf::Keyboard::T, sf::XInputDevice::LB);
	VGlobal::p()->Input.AddButtonInput("rightShoulder", sf::Keyboard::Y, sf::XInputDevice::RB);
	VGlobal::p()->Input.AddButtonInput("leftStick", sf::Keyboard::B, sf::XInputDevice::LEFT_THUMB);
	VGlobal::p()->Input.AddButtonInput("rightStick", sf::Keyboard::N, sf::XInputDevice::RIGHT_THUMB);
	VGlobal::p()->Input.AddButtonInput("Back", sf::Keyboard::BackSpace, sf::XInputDevice::BACK);
	VGlobal::p()->Input.AddButtonInput("Start", sf::Keyboard::Return, sf::XInputDevice::START);
#endif
}

void DemoStatesManager::HandleEvents(const sf::Event& event)
{
	VSUPERCLASS::HandleEvents(event);

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Left)
		{
			CurrentPage--;

			if (CurrentPage < 0)
				CurrentPage = NUM_DEMOPAGES - 1;

			SetNewPage();
		}

		if (event.key.code == sf::Keyboard::Right)
		{
			CurrentPage++;
			CurrentPage %= NUM_DEMOPAGES;

			SetNewPage();
		}

		if (event.key.code == sf::Keyboard::Q)
		{
			VGlobal::p()->DrawDebug = !VGlobal::p()->DrawDebug;
		}

		if (event.key.code == sf::Keyboard::E)
		{
			VGlobal::p()->Antialiasing = !VGlobal::p()->Antialiasing;
		}

		if (event.key.code == sf::Keyboard::F)
		{
			VGlobal::p()->ToggleFullscreen();
		}
	}
}
