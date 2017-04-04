#include "States.h"

enum DemoPages
{
	PAGE_SPRITE,
	PAGE_TILED,
	PAGE_TEXT,
	PAGE_TEXTPATH,
	PAGE_ASCII,
	PAGE_TILEMAP,
	PAGE_COLLISION,
	PAGE_INTERPOLATE,
	PAGE_PARTICLES,
	PAGE_TRAILAREA,
	PAGE_BACKDROP,
	PAGE_LIGHTING2D,
	PAGE_GRAPHICS3D,
	NUM_DEMOPAGES,
};

void DemoStatesManager::SetNewPage()
{
	VSubState* subState;
	switch (CurrentPage)
	{
		case PAGE_ASCII: 
			subState = new ASCIITestState();
			break;
		case PAGE_BACKDROP:
			subState = new BackdropState();
			break;
		case PAGE_COLLISION:
			subState = new CollisionState();
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

	VGlobal::p()->Input.AddAxisInput("horizontal",	sf::Keyboard::D, sf::Keyboard::A, sf::XInputDevice::PovX);
	VGlobal::p()->Input.AddAxisInput("vertical",	sf::Keyboard::S, sf::Keyboard::W, sf::XInputDevice::PovY); 
}

void DemoStatesManager::HandleEvents(sf::Event event)
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