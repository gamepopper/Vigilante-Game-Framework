#pragma once
#include "../VFrame/VState.h"
#include "../VFrame/VBackdrop.h"
#include "../VFrame/VCamera.h"
#include "../VFrame/VColour.h"
#include "../VFrame/VContent.h"
#include "../VFrame/VEmitter.h"
#include "../VFrame/VGlobal.h"
#include "../VFrame/VInterpolate.h"
#include "../VFrame/VRenderGroup.h"
#include "../VFrame/VTiledSprite.h"
#include "../VFrame/VTilemap.h"
#include "../VFrame/VTrailArea.h"
#include "../VFrame/VTextPath.h"
#include "../VFrame/VTypedText.h"
#include "../VFrame/XInputDevice.h"
#include "../VFrame/V3DScene.h"
#include "../VFrame/V3DModel.h"
#include "../VFrame/V3DObjModel.h"
#include "../VFrame/VTimer.h"

#include <iostream>
#include <sstream>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>

using std::cout;
using std::endl;
using std::vector;

//Static and Animated Sprites
/*
This demonstrates static sprites and animated frame by frame system.
*/
class SpriteState : public VSubState
{
	typedef VSubState VSUPERCLASS;

	VSprite* standardSprite;
	VSprite* animatedSprite;

public:
	SpriteState() : VSubState() {}
	~SpriteState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		standardSprite = new VSprite(0, 0, "Example/Assets/gp.png");
		standardSprite->SetPositionAtCentre(VGlobal::p()->Width / 3.0f, VGlobal::p()->Height / 2.0f - 50.0f);

		auto sprite1 = new VText(standardSprite->Position.x, 50, standardSprite->Size.x, "Regular Sprite", 16);
		sprite1->Alignment = VTextAlign::ALIGNCENTRE;
		sprite1->ApplyChanges();

		animatedSprite = new VSprite(0, 0);
		animatedSprite->LoadGraphic("Example/Assets/Turret.png", true, 128, 128);
		animatedSprite->SetPositionAtCentre(2 * VGlobal::p()->Width / 3.0f, VGlobal::p()->Height / 2.0f - 50.0f);
		animatedSprite->Animation.AddAnimation("start", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }, 15.0f);
		animatedSprite->Animation.AddAnimation("shotgun", { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 }, 15.0f, true);
		animatedSprite->Animation.AddAnimation("railgun", { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46 }, 15.0f, true);
		animatedSprite->Animation.AddAnimation("doublegun", { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62 }, 15.0f, true);
		animatedSprite->Animation.Play("start");

		auto sprite2 = new VText(animatedSprite->Position.x, 50, animatedSprite->Size.x, "Animated Sprite", 16);
		sprite2->Alignment = VTextAlign::ALIGNCENTRE;
		sprite2->ApplyChanges();

		auto text = new VText(standardSprite->Position.x, 200, 400, "Press 1: Start Animation\nPress 2: Shotgun Animation\nPress 3: Railgun Animation\nPress 4: Double Barrel Animation", 21);
		text->ApplyChanges();

		Add(standardSprite);
		Add(animatedSprite);
		Add(sprite1);
		Add(sprite2);
		Add(text);
	}

	virtual void HandleEvents(const sf::Event& event)
	{
		if (event.type == event.KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Num1)
			{
				animatedSprite->Animation.Play("start");
			}

			if (event.key.code == sf::Keyboard::Num2)
			{
				animatedSprite->Animation.Play("shotgun");
			}

			if (event.key.code == sf::Keyboard::Num3)
			{
				animatedSprite->Animation.Play("railgun");
			}

			if (event.key.code == sf::Keyboard::Num4)
			{
				animatedSprite->Animation.Play("doublegun");
			}
		}
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

		float x1Resize = VGlobal::p()->Input->CurrentAxisValue("leftX");
		float y1Resize = VGlobal::p()->Input->CurrentAxisValue("leftY");
		float x2Resize = VGlobal::p()->Input->CurrentAxisValue("rightX");
		float y2Resize = VGlobal::p()->Input->CurrentAxisValue("rightY");

		standardSprite->Size += sf::Vector2f(x1Resize / 100, y1Resize / 100);
		animatedSprite->Size += sf::Vector2f(x2Resize / 100, y2Resize / 100);
	}
};

//Static and Animated Tiled Sprites
/*
Tiled sprites are like regular sprites, except the texture can repeat.
*/
class TiledState : public VSubState
{
	typedef VSubState VSUPERCLASS;

	VTiledSprite* standardSprite;
	VTiledSprite* animatedSprite;

public:
	TiledState() : VSubState() {}
	~TiledState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		standardSprite = new VTiledSprite(0, 0, "Example/Assets/gp.png");
		standardSprite->Size = sf::Vector2f(100, 100);
		standardSprite->SetPositionAtCentre(VGlobal::p()->Width / 3.0f, VGlobal::p()->Height / 2.0f);
		Add(standardSprite);

		auto sprite1 = new VText(standardSprite->Position.x, 80, standardSprite->Size.x, "Regular TiledSprite", 16);
		sprite1->Alignment = VTextAlign::ALIGNCENTRE;
		sprite1->ApplyChanges();

		animatedSprite = new VTiledSprite(0, 0);
		animatedSprite->LoadGraphic("Example/Assets/Waterfall.png", true, 32, 32);
		animatedSprite->Animation.AddAnimation("waterfall", { 0, 1, 2, 3 }, 15.0f, true);
		animatedSprite->Animation.Play("waterfall");
		animatedSprite->Size = sf::Vector2f(100, 100);
		animatedSprite->SetPositionAtCentre(2 * VGlobal::p()->Width / 3.0f, VGlobal::p()->Height / 2.0f);
		Add(animatedSprite);

		auto sprite2 = new VText(animatedSprite->Position.x, 80, animatedSprite->Size.x, "Animated TiledSprite", 16);
		sprite2->Alignment = VTextAlign::ALIGNCENTRE;
		sprite2->ApplyChanges();

		auto text = new VText(standardSprite->Position.x - 40, 300, 480, "Press WASD: Resize Standard Tiled Sprite\nPress IJKL: Resize Animated Tiled Sprite", 21);
		text->ApplyChanges();

		Add(sprite1);
		Add(sprite2);
		Add(text);
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

		float x1Resize = VGlobal::p()->Input->CurrentAxisValue("leftX");
		float y1Resize = VGlobal::p()->Input->CurrentAxisValue("leftY");
		float x2Resize = VGlobal::p()->Input->CurrentAxisValue("rightX");
		float y2Resize = VGlobal::p()->Input->CurrentAxisValue("rightY");

		standardSprite->Size += sf::Vector2f(x1Resize, y1Resize) * dt;
		animatedSprite->Size += sf::Vector2f(x2Resize, y2Resize) * dt;
	}
};

//Regular and Typewriter Text
/*
Demonstrates text styles, wrapping and typewriting effect.
*/
class TextState : public VSubState
{
	typedef VSubState VSUPERCLASS;

	VText* normalText;
	VTypedText* typedText;

public:
	TextState() : VSubState() {}
	~TextState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		normalText = new VText(10.0f, 120.0f, VGlobal::p()->Width - 20.0f);
		normalText->SetFormat("Example/Assets/DejaVuSansMono.ttf", 16, sf::Color::White, VTextAlign::ALIGNRIGHT, sf::Text::Italic | sf::Text::Underlined);
		normalText->Text = L"!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~¡¢£¤¥¦§¨©ª«¬-®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿĀāĂăĄąĆćĈĉĊċČčĎďĐđĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłŃńŅņŇňŉŊŋŌōŎŏŐőŒœŔŕŖŗŘřŚśŜŝŞşŠšŢţŤťŦŧŨũŪūŬŭŮůŰűŲųŴŵŶŷŸŹźŻżŽžſƒǺǻǼǽǾǿȘșȚțˆˇˉ˘˙˚˛˜˝;΄΅Ά·ΈΉΊΌΎΏΐΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩΪΫάέήίΰαβγδεζηθικλμνξοπρςστυφχψωϊϋόύώЁЂЃЄЅІЇЈЉЊЋЌЎЏАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюяёђѓєѕіїјљњћќўџҐґẀẁẂẃẄẅỲỳ–—―‗‘’‚“”„†‡•…‰‹›‼‾⁄ⁿ₣₤₧€№™Ω⅛⅜⅝⅞←↑→↓↔↕↨∂∆∏∑−∙√∞∟∩∫≈≠≡≤≥⌂⌐⌠⌡─│┌┐└┘├┤┬┴┼═║╒╓╔╕╖╗╘╙╚╛╜╝╞╟╠╡╢╣╤╥╦╧╨╩╪╫╬▀▄█▌▐░▒▓■▬▲►▼◄◊○◘◙☺☻☼♀♂♠♣♥♦♪♫ﬁﬂ";
		normalText->Wrap = WRAPLETTER;
		normalText->ApplyChanges();
		Add(normalText);

		typedText = new VTypedText(10.0f, 50.0f, (float)VGlobal::p()->Width - 20.0f);
		typedText->SetFormat("Example/Assets/DejaVuSansMono.ttf", 16, sf::Color::White, VTextAlign::ALIGNLEFT, sf::Text::Bold);
		typedText->ResetText("Religion and politics often make some people lose all perspective and give way to ranting and raving and carrying on like emotional children");
		typedText->CursorChar = '_';
		typedText->CursorBlinkSpeed = 0.5f;
		typedText->ShowCursor = true;
		typedText->Start(0.05f);
		Add(typedText);
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

		if (VGlobal::p()->Input->IsButtonPressed("leftShoulder"))
		{
			typedText->Erase(0.02f);
		}
		if (VGlobal::p()->Input->IsButtonPressed("rightShoulder"))
		{
			typedText->Start(0.05f);
		}
	}
};

//Tilemap
/*
Autotiling and tilemap collision. It's also possible to have more than one wall graphic for autotiling.
*/
class TilemapState : public VSubState
{
	typedef VSubState VSUPERCLASS;

	VTilemap* tilemap;
	VSprite* playerControl;
	VSprite* platform[2];
	VTimer* timer;
	bool wallJump = false;

public:
	TilemapState() : VSubState() {}
	~TilemapState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		tilemap = new VTilemap();
		tilemap->LoadFromCSV("Example/Assets/level.csv", "Example/Assets/Walls.png", 32, 32, true, { '#' });
		tilemap->SetTileRenderID('#');
		tilemap->SetTileRenderID('$');
		tilemap->SetTileRenderID('~', 0, 1);
		tilemap->SetTileCollisionID('$', SidesTouching::TOUCHALL, std::bind(&TilemapState::WallJump, this, std::placeholders::_1, std::placeholders::_2));

		Add(tilemap);

		platform[0] = new VSprite();
		platform[0]->MakeGraphic(64, 32, VColour::HSVtoRGB(0, 0.0f, 0.75f), 1.0f, sf::Color::Red);
		platform[0]->SetPositionAtCentre(24.5f * 32, 10.5f * 32.0f);
		platform[0]->Immovable = true;
		platform[0]->Velocity.x = 32.0f;
		Add(platform[0]);

		platform[1] = new VSprite();
		platform[1]->MakeGraphic(64, 32, VColour::HSVtoRGB(0, 0.0f, 0.75f), 1.0f, sf::Color::Red);
		platform[1]->SetPositionAtCentre(42.5f * 32, 7.5f * 32.0f);
		platform[1]->Immovable = true;
		platform[1]->Velocity.y = 32.0f;
		Add(platform[1]);

		VGlobal::p()->WorldBounds.width = tilemap->Size.x;
		VGlobal::p()->WorldBounds.height = tilemap->Size.y;

		sf::Vector2f playerPos;
		for (int y = 0; y < tilemap->Size.y / tilemap->TileSize.y; y++)
		{
			for (int x = 0; x < tilemap->Size.x / tilemap->TileSize.x; x++)
			{
				if (tilemap->GetTileID(x, y) == '@')
				{
					playerPos = (sf::Vector2f((float)x, (float)y) * 32.0f) + sf::Vector2f(16.0f, 16.0f);
					break;
				}
			}
		}

		playerControl = new VSprite();
		playerControl->MakeGraphic(32, 32, sf::Color::White);
		playerControl->SetPositionAtCentre(playerPos);
		playerControl->Drag = sf::Vector2f(500, 500);
		playerControl->MaxVelocity = sf::Vector2f(200, 600);
		Add(playerControl);

		ParentState->Cameras[0]->Follow(playerControl, 0.5f, PLATFORMER, 0.8f, 0.05f);
		UseParentCamera = true;

		timer = new VTimer();
	}

	virtual void Cleanup()
	{
		VSUPERCLASS::Cleanup();
		VGlobal::p()->WorldBounds.width = static_cast<float>(VGlobal::p()->Width);
		VGlobal::p()->WorldBounds.height = static_cast<float>(VGlobal::p()->Height);
	}

	virtual void Update(float dt)
	{
		bool touchFloor = (playerControl->Touching & SidesTouching::TOUCHBOTTOM) > 0;

		for (unsigned int i = 0; i < 2; i++)
		{
			sf::Vector2f playerFeet = playerControl->Position + sf::Vector2f(0, playerControl->Size.y + 5.0f);

			if (playerFeet.y > platform[i]->Position.y &&
				playerFeet.y < platform[i]->Position.y + platform[1]->Size.y &&
				playerFeet.x < platform[i]->Position.x + platform[1]->Size.x &&
				playerFeet.x > platform[i]->Position.x - playerControl->Size.x)
			{
				touchFloor = true;
			}
		}
		
		if (touchFloor)
		{
			if (VGlobal::p()->Input->IsButtonPressed("A"))
			{
				playerControl->Velocity.y = -450.0f;

				if (wallJump)
				{
					if ((playerControl->Touching & SidesTouching::TOUCHLEFT) > 0)
					{
						playerControl->Velocity.x = playerControl->MaxVelocity.x;
					}
					else if ((playerControl->Touching & SidesTouching::TOUCHRIGHT) > 0)
					{
						playerControl->Velocity.x = -playerControl->MaxVelocity.x;
					}
				}
			}
		}

		playerControl->Acceleration.x = 0;
		if (VGlobal::p()->Input->CurrentAxisValue("leftX") < 0)
			playerControl->Acceleration.x -= playerControl->MaxVelocity.x * (touchFloor ? 4 : 3);
		if (VGlobal::p()->Input->CurrentAxisValue("leftX") > 0)
			playerControl->Acceleration.x += playerControl->MaxVelocity.x * (touchFloor ? 4 : 3);

		if (VGlobal::p()->Input->IsButtonDown("A"))
			playerControl->Acceleration.y = 700.0f;
		else
			playerControl->Acceleration.y = 980.0f;

		VSUPERCLASS::Update(dt);

		if (timer->Seconds() > 2.0f)
		{
			platform[0]->Velocity.x *= -1;
			platform[1]->Velocity.y *= -1;
			timer->Restart();
		}

		wallJump = false;
		VGlobal::p()->Collides(playerControl, tilemap);
		VGlobal::p()->Collides(playerControl, platform[0], std::bind(&TilemapState::CloudPlatform, this, std::placeholders::_1, std::placeholders::_2));
		VGlobal::p()->Collides(playerControl, platform[1], std::bind(&TilemapState::CloudPlatform, this, std::placeholders::_1, std::placeholders::_2));
	}

	void CloudPlatform(VBase* player, VBase* platform)
	{
		VObject* o = dynamic_cast<VObject*>(player);
		VObject* p = dynamic_cast<VObject*>(platform);

		if (VGlobal::p()->Input->CurrentAxisValue("leftY") > 20.0f)
		{
			p->AllowCollisions = SidesTouching::TOUCHNONE;
		}
		else if (o->Position.y < p->Position.y)
		{
			p->AllowCollisions = SidesTouching::TOUCHTOP;
		}
	}

	void WallJump(VObject* tile, VObject* object)
	{
		object->Velocity *= 0.9f;
		object->Touching |= SidesTouching::TOUCHBOTTOM;
		wallJump = true;
	}
};

//Collision
/*
Test both overlap and colliding, in either circle or rectangle collision.
*/
class CollisionState : public VSubState
{
	typedef VSubState VSUPERCLASS;

	VSprite* playerControl;
	VGroup* overlapGroup;
	VGroup* collideGroup;

	float timer = 0.0f;

public:
	CollisionState() : VSubState() {}
	~CollisionState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		playerControl = new VSprite();
		playerControl->MakeGraphic(32, 32, sf::Color::White);
		playerControl->SetPositionAtCentre(VGlobal::p()->Width / 2.0f, 300.0f);
		playerControl->Drag = sf::Vector2f(500, 500);
		playerControl->MaxVelocity = sf::Vector2f(200, 200);
		Add(playerControl);

		overlapGroup = new VGroup(2);
		auto sprite1 = new VSprite();
		sprite1->MakeGraphic(32, 32, sf::Color::Transparent, 2, sf::Color::White);
		sprite1->Tint = sf::Color::Cyan;
		sprite1->SetPositionAtCentre(128, VGlobal::p()->Height / 2.0f);
		auto sprite2 = new VSprite();
		sprite2->MakeGraphicCircle(16, sf::Color::Transparent, 2, sf::Color::White);
		sprite2->SetPositionAtCentre(256, VGlobal::p()->Height / 2.0f);
		sprite2->Tint = sf::Color::Green;
		overlapGroup->Add(sprite1);
		overlapGroup->Add(sprite2);

		collideGroup = new VGroup(2);
		auto sprite3 = new VSprite();
		sprite3->MakeGraphicSided(16, 6, sf::Color::Black, 2, sf::Color::White);
		sprite3->SetPositionAtCentre(384, VGlobal::p()->Height / 2.0f);
		sprite3->Tint = sf::Color::Magenta;
		sprite3->Drag = sf::Vector2f(250, 250);

		auto sprite4 = new VSprite();
		std::vector<sf::Vector2f> starPoints;
		starPoints.push_back(sf::Vector2f(16, 0));
		starPoints.push_back(sf::Vector2f(22, 12));
		starPoints.push_back(sf::Vector2f(32, 12));
		starPoints.push_back(sf::Vector2f(24, 20));
		starPoints.push_back(sf::Vector2f(32, 32));
		starPoints.push_back(sf::Vector2f(16, 24));
		starPoints.push_back(sf::Vector2f(0, 32));
		starPoints.push_back(sf::Vector2f(8, 20));
		starPoints.push_back(sf::Vector2f(0, 12));
		starPoints.push_back(sf::Vector2f(10, 12));
		sprite4->MakeGraphicConvex(starPoints, sf::Color::Black, 2, sf::Color::White);
		sprite4->SetPositionAtCentre(512, VGlobal::p()->Height / 2.0f);
		sprite4->Tint = sf::Color::Yellow;
		sprite4->Immovable = true;

		collideGroup->Add(sprite3);
		collideGroup->Add(sprite4);

		Add(overlapGroup);
		Add(collideGroup);

		auto overlapText = new VText(sprite1->Position.x, 100.0f, 160, "Overlap These", 16);
		overlapText->Alignment = VTextAlign::ALIGNCENTER;
		overlapText->ApplyChanges();
		auto collideText = new VText(sprite3->Position.x, 100.0f, 160, "Collide These", 16);
		collideText->Alignment = VTextAlign::ALIGNCENTER;
		collideText->ApplyChanges();

		Add(overlapText);
		Add(collideText);
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

		playerControl->Acceleration = sf::Vector2f(VGlobal::p()->Input->CurrentAxisValue("leftX") * 3, VGlobal::p()->Input->CurrentAxisValue("leftY") * 3);

		VGlobal::p()->BackgroundColor = sf::Color::Black;
		using namespace std::placeholders;
		VGlobal::p()->Overlaps(playerControl, overlapGroup, std::bind(&CollisionState::overlapResponse, this, _1, _2));
		VGlobal::p()->Collides(playerControl, collideGroup, std::bind(&CollisionState::overlapResponse, this, _1, _2));
		VGlobal::p()->Collides(collideGroup);

		if (!ParentState->Cameras[0]->IsObjectInView(playerControl))
		{
			VGlobal::p()->BackgroundColor = sf::Color::White;
		}

		timer += dt;
	}

	void overlapResponse(VObject* player, VObject* object)
	{
		VSprite* sprite = dynamic_cast<VSprite*>(object);
		VGlobal::p()->BackgroundColor = sprite->Tint;
	}
};

//Controller
/*
Test controllers
*/
class ControllerState : public VSubState
{
	typedef VSubState VSUPERCLASS;

	unsigned int buttonCount;
	unsigned int axisCount;

	VGroup* buttons;
	VGroup* axis;
	VGroup* text;

public:
	ControllerState() : VSubState() {}
	~ControllerState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

#ifdef USE_GAMEPAD_API
		buttonCount = BUTTON_COUNT;
		axisCount = VInputHandler::R + 1;
#elif defined(USE_SFML_JOYSTICK)
		buttonCount = sf::Joystick::getButtonCount(VGlobal::p()->Input->GetJoystickID(0));
		axisCount = sf::Joystick::Axis::PovY + 1;
#else
		buttonCount = 14;
		axisCount = sf::XInputDevice::R + 1;
#endif
		buttons = new VGroup(buttonCount);
		axis = new VGroup(axisCount);
		text = new VGroup(buttonCount + axisCount);

		for (unsigned int i = 0; i < buttonCount; i++)
		{
			float x = (i % 10) * 60.0f;
			float y = (i / 10) * 60.0f;

			VText* t = new VText(20.0f + x, 80.0f + y - 6, 20.0f, std::to_string(i) + ":", 12);
			t->SetFormat("Example/Assets/DejaVuSansMono.ttf", 12, sf::Color::White, VTextAlign::ALIGNLEFT);

			VSprite* b = new VSprite(40.0f + x, 70.0f + y);

			if (i == 0)
			{
				b->MakeGraphic(30, 30, sf::Color::White);
			}
			else
			{
				b->LoadGraphicFromTexture(*dynamic_cast<VSprite*>(buttons->GetGroupItem(0))->GetTexture());
			}

			text->Add(t);
			buttons->Add(b);
		}

		int buttonOffset = (buttonCount / 10) + 1;

		for (unsigned int i = 0; i < axisCount; i++)
		{
			float x = (i % 10) * 60.0f;
			float y = (i / 10) * 60.0f;

			VText* t = new VText(20.0f + x, 120.0f + y + (buttonOffset * 60.0f) - 6.0f, 20.0f, std::to_string(i) + ":", 12);
			t->SetFormat("Example/Assets/DejaVuSansMono.ttf", 12, sf::Color::White, VTextAlign::ALIGNLEFT);

			VSprite* a = new VSprite(40.0f + x, 120.0f + y + (buttonOffset * 60.0f));

			if (i == 0)
			{
				a->MakeGraphic(30, 50, sf::Color::White);
			}
			else
			{
				a->LoadGraphicFromTexture(*dynamic_cast<VSprite*>(axis->GetGroupItem(0))->GetTexture());
			}

			a->Origin = sf::Vector2f(0, 0);

			text->Add(t);
			axis->Add(a);
		}

		Add(buttons);
		Add(axis);
		Add(text);
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

#ifdef USE_GAMEPAD_API

		for (unsigned int i = 0; i < buttonCount; i++)
		{
			VSprite* b = dynamic_cast<VSprite*>(buttons->GetGroupItem(i));

			if (GamepadButtonDown(GAMEPAD_0, (GAMEPAD_BUTTON)i))
			{
				b->Tint = sf::Color::White;
			}
			else
			{
				b->Tint = VColour::HSVtoRGB(0.0f, 0.0f, 0.6f);
			}
		}

		for (unsigned int i = 0; i < axisCount; i++)
		{
			VSprite* b = dynamic_cast<VSprite*>(axis->GetGroupItem(i));

			float val1 = 0, val2 = 0;

			switch (i)
			{
				case VInputHandler::PovX:
				{
					GamepadStickNormXY(GAMEPAD_0, STICK_LEFT, &val1, &val2);
					break;
				}
				case VInputHandler::PovY:
				{
					GamepadStickNormXY(GAMEPAD_0, STICK_LEFT, &val2, &val1);
					val1 *= -1;
					break;
				}
				case VInputHandler::Z:
				{
					GamepadStickNormXY(GAMEPAD_0, STICK_RIGHT, &val1, &val2);
					break;
				}
				case VInputHandler::V:
				{
					GamepadStickNormXY(GAMEPAD_0, STICK_RIGHT, &val2, &val1);
					val1 *= -1;
					break;
				}
				case VInputHandler::L:
				{
					val1 = GamepadTriggerValue(GAMEPAD_0, TRIGGER_LEFT) / 255;
					break;
				}
				case VInputHandler::R:
				{
					val1 = GamepadTriggerValue(GAMEPAD_0, TRIGGER_RIGHT) / 255;
					break;
				}
			}

			b->Scale.y = val1;
		}

#elif defined(USE_SFML_JOYSTICK)

		for (unsigned int i = 0; i < buttonCount; i++)
		{
			VSprite* b = dynamic_cast<VSprite*>(buttons->GetGroupItem(i));

			if (sf::Joystick::isButtonPressed(VGlobal::p()->Input->GetJoystickID(0), i))
			{
				b->Tint = sf::Color::White;
			}
			else
			{
				b->Tint = VColour::HSVtoRGB(0.0f, 0.0f, 0.6f);
			}
		}

		for (unsigned int i = 0; i < axisCount; i++)
		{
			VSprite* b = dynamic_cast<VSprite*>(axis->GetGroupItem(i));
			b->Scale.y = sf::Joystick::getAxisPosition(VGlobal::p()->Input->GetJoystickID(0), (sf::Joystick::Axis)i) / 100.0f;
		}
#else
		unsigned short buttonIDs[14] = 
		{
			sf::XInputDevice::XButton::DPAD_UP,
			sf::XInputDevice::XButton::DPAD_DOWN,
			sf::XInputDevice::XButton::DPAD_LEFT,
			sf::XInputDevice::XButton::DPAD_RIGHT,
			sf::XInputDevice::XButton::START,
			sf::XInputDevice::XButton::BACK,
			sf::XInputDevice::XButton::LEFT_THUMB,
			sf::XInputDevice::XButton::RIGHT_THUMB,
			sf::XInputDevice::XButton::LB,
			sf::XInputDevice::XButton::RB,
			sf::XInputDevice::XButton::A,
			sf::XInputDevice::XButton::B,
			sf::XInputDevice::XButton::X,
			sf::XInputDevice::XButton::Y,
		};

		for (unsigned int i = 0; i < buttonCount; i++)
		{
			VSprite* b = dynamic_cast<VSprite*>(buttons->GetGroupItem(i));

			if (sf::XInputDevice::isButtonPressed(0, buttonIDs[i]))
			{
				b->Tint = sf::Color::White;
			}
			else
			{
				b->Tint = VColour::HSVtoRGB(0.0f, 0.0f, 0.6f);
			}
		}

		for (unsigned int i = 0; i < axisCount; i++)
		{
			VSprite* b = dynamic_cast<VSprite*>(axis->GetGroupItem(i));
			b->Scale.y = sf::XInputDevice::getAxisPosition(0, (sf::XInputDevice::XAxis)i);
			b->Scale.y = abs(b->Scale.y) > 1.0f ? b->Scale.y / 100.0f : b->Scale.y;
		}
#endif
	}
};

//Particle Effects
/*
Both explosive and constant particles.
The middle effect is uses a VRenderGroup to apply a post render effect.
*/
class ParticlesState : public VSubState
{
	typedef VSubState VSUPERCLASS;

	VEmitter* emitter;
	VEmitter* explosion1;
	VEmitter* explosion2;
	VRenderGroup* render;

public:
	ParticlesState() : VSubState() {}
	~ParticlesState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		render = new VRenderGroup(0, 0, 200, 200);
		render->Sprite->Drag = sf::Vector2f(500, 500);
		render->Sprite->MaxVelocity = sf::Vector2f(200, 200);
		render->RenderOutside = true;
		render->Sprite->SetPositionAtCentre(320, 200);

		VPostEffect* postEffect = new VPostEffect("Example/Assets/AlphaThreshold.frag", sf::Shader::Fragment);
		postEffect->SetParameter("threshold", 0.85f);
		postEffect->SetParameter("smoothness", 0.40f);
		postEffect->SetParameter("unpremultiply", 1.0f);
		render->PostEffect = postEffect;

		emitter = new VEmitter(320, 280);
		emitter->LoadParticlesFromFile(75, "Example/Assets/Smoke.png");
		emitter->Circular = true;
		emitter->Constant = true;
		emitter->Size = sf::Vector2f(32, 8);
		emitter->EmittingAngle.A = -125;
		emitter->EmittingAngle.B = -55;
		emitter->SpeedRange.A = 5;
		emitter->SpeedRange.B = 10;
		emitter->AccelerationRange.A.y = -100;
		emitter->AccelerationRange.B.y = -50;
		emitter->AngleVelocityRange.A = 50;
		emitter->AngleVelocityRange.B = 120;
		emitter->AlphaRange.A = 1.0f;
		emitter->AlphaRange.B = 0.0f;
		emitter->ScaleRange.B = sf::Vector2f(2, 2);
		emitter->ColourRange.A.A = sf::Color(255, 128, 0, 255);
		emitter->ColourRange.A.B = sf::Color(255, 255, 0, 255);
		emitter->ColourRange.B.A = sf::Color(128, 0, 0, 255);
		emitter->ColourRange.B.B = sf::Color(200, 0, 0, 255);
		emitter->Lifespan.A = 0.5f;
		emitter->Lifespan.B = 1.5f;
		emitter->Frequency = 1.0f / 60.0f;
		emitter->Start();
		emitter->RenderState.blendMode = sf::BlendAdd;
		render->Add(emitter);

		explosion1 = new VEmitter(160, 240);
		explosion1->MakeParticles(20, 2, 2);
		explosion1->Explosion = true;
		explosion1->Circular = false;
		explosion1->Lifespan.A = 1;
		explosion1->Lifespan.B = 1;
		explosion1->AlphaRange.B = 0.0f;
		explosion1->Start();
		Add(explosion1);

		explosion2 = new VEmitter(480, 240);
		explosion2->MakeParticles(20, 2, 2);
		explosion2->Explosion = true;
		explosion2->Circular = true;
		explosion2->Lifespan.A = 1;
		explosion2->Lifespan.B = 1;
		explosion2->ColourRange.A.A = sf::Color::Black;
		explosion2->ColourRange.A.B = sf::Color::White;
		explosion2->ColourRange.B.A = sf::Color::Black;
		explosion2->ColourRange.B.B = sf::Color::White;
		explosion2->AlphaRange.B = 0.0f;
		explosion2->Start();
		Add(explosion2);
		Add(render);
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

		if (!explosion1->exists)
		{
			explosion1->Start();
			explosion2->Start();
		}
	}
};

#include "BloomPostEffect.h"

//Recreation of the Cogmind Logo
/*
Three layers of VTypedText with a bloom.
*/
class ASCIITestState : public VSubState
{
	typedef VSubState VSUPERCLASS;

public:
	ASCIITestState() : VSubState() {}
	~ASCIITestState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		std::wstringstream ws;
		auto copyright = new VText(10.0f, VGlobal::p()->Height - 18.0f, (float)VGlobal::p()->Width - 20.0f, "Remade by hand by Gamepopper.\tOriginal logo by GridSageGames.", 14);
		copyright->Alignment = VTextAlign::ALIGNLEFT;

		ws = std::wstringstream();
		ws << L"                                                 ▄" << std::endl <<
			L"                                                 |" << std::endl <<
			L"                                       ·         |               · ·" << std::endl <<
			L"                                       |         |             ·─┼ |" << std::endl <<
			L"                    |    ▄      ▄      |         |               | ┼─·" << std::endl <<
			L"        ╡                | ___  |      |         |               ▄ ▄" << std::endl <<
			L"        |                ╘[    ]╛   ┌─< >▄_▄    ▄╒█ ╕" << std::endl <<
			L"                                                                       __" << std::endl <<
			L"     ╓  ─  ═════                  ╒═       /     ╒█ ═╕                  ║" << std::endl <<
			L"     ▀  └  ─────  ≡≡════   ╚╚╚══════ ══ ╜╜             ──┐───           " << std::endl <<
			L"     ║             |───      |     ─   ║                 ╛     ┌    ──  ───" << std::endl <<
			L"     ║           !!|         |       ─ ╨── ─┐    |  :          |        " << std::endl <<
			L"   ----                                ║                       |        " << std::endl <<
			L"     ║          |  ┌ ─       ┤    ║· ·      |   ||   | ────" << std::endl <<
			L"--────────╜╜────            ───                ─┘|   '      |   |" << std::endl <<
			L"     ╙──┬────╖  |  |         |    ║ |  ║┴ | ┌┼···┤          | ▬ ╞:═╝╝" << std::endl <<
			L"        ^    ║  ·  \\|┐       |    ≡─── ╫────|·────────────┐ | ▬ ╞:═╝╝" << std::endl <<
			L"             ╨──┤ ┌─o──      ·    └┐| |▀  └─|┼┐        +──|╒╪ ▬ ╪╛" << std::endl <<
			L"                └───╨─────·       └┐| |▀    ''|           └─[ ▬ ]" << std::endl <<
			L"                  |█:█            └─|┐|▀      |               ╨──────--" << std::endl <<
			L"                    ╥              |└└┴╜      ┼";

		VTypedText* cLayer1 = new VTypedText(10.0f, 40.0f, (float)VGlobal::p()->Width);
		cLayer1->SetFormat("Example/Assets/DejaVuSansMono.ttf", 13, sf::Color(255, 255, 255, 24), VTextAlign::ALIGNLEFT);
		cLayer1->ResetText(ws.str());
		cLayer1->CharactersPerIteration = 4;
		Add(cLayer1);

		ws = std::wstringstream();
		ws << L"" << std::endl <<
			L"" << std::endl <<
			L"" << std::endl <<
			L"" << std::endl <<
			L"" << std::endl <<
			L"                    |                        ║                              " << std::endl <<
			L"         ▄ ▄        |      ___         _    _█_      ┌─[. . .]   ▄ ▄        " << std::endl <<
			L"               =                   :                 ╘                 __   " << std::endl <<
			L"         | |     ╔══════ ═ ╝╝╝       ≡≡ ≡≡   ║               |   · ·        " << std::endl <<
			L"  ───■ ╗ ¦ ¦     ≡       ┌ ╖     ┌·    x     ║ ╧ ═════ █     ·   | |    ■───" << std::endl <<
			L"       ╙──────── ╜     █ : ╨·· · ┴ ─ ─/ \\_ __▄ ╞ ═════ █     ·   | |  /     " << std::endl <<
			L"         ¦ ¦           █ : ═╕         |      | ╡       ══════╛   ▄▄▄_/      " << std::endl <<
			L"               =         |            |    :         : █                    " << std::endl <<
			L"         ▀ ▀        |  --^--        · |   ·  |        ·    ─╥ ·──╨╨--       " << std::endl <<
			L"                ·───+───╥┴─·   ·──╥─┴─────|──┘        ┼       └──---        " << std::endl <<
			L"                    ¦   ˝             |  ─┼           ¦                     ";

		VTypedText* cLayer2 = new VTypedText(10.0f, 40.0f, (float)VGlobal::p()->Width);
		cLayer2->SetFormat("Example/Assets/DejaVuSansMono.ttf", 13, sf::Color(0, 255, 0, 64), VTextAlign::ALIGNLEFT);
		cLayer2->ResetText(ws.str());
		cLayer2->CharactersPerIteration = 4;
		Add(cLayer2);

		ws = std::wstringstream();
		ws << L"" << std::endl <<
			L"" << std::endl <<
			L"" << std::endl <<
			L"" << std::endl <<
			L"" << std::endl <<
			L"" << std::endl <<
			L"" << std::endl <<
			L"       ╔═══════ ╔═══════╗ ╔════════ ╔══╬══╗ ════╦════ ╔═══════╗ ╔╪═╪══╗ " << std::endl <<
			L"      ╔╝        ║       ║ ║         ║  ║  ║     ║     ║       ║ ║     ╚╗" << std::endl <<
			L"      ║         ║       ║ ║         ║     ║     ║     ║       ║ ║      ║" << std::endl <<
			L"      ║         ║       ╠ ║       ║ ║     ║     ║     ║       ║ ║      ║" << std::endl <<
			L"      ╚╗        ║       ╠ ║       ║ ║     ║     ║     ║       ║ ║     ╔╝" << std::endl <<
			L"       ╚═══════ ╚═══════╝ ╚═══════╝ ║     ║ ════╩════ ║       ║ ╚═════╝ " << std::endl <<
			L"" << std::endl <<
			L"" << std::endl <<
			L"";

		VTypedText* cLayer3 = new VTypedText(10.0f, 40.0f, (float)VGlobal::p()->Width);
		cLayer3->SetFormat("Example/Assets/DejaVuSansMono.ttf", 13, sf::Color(0, 255, 0, 200), VTextAlign::ALIGNLEFT);
		cLayer3->ResetText(ws.str());
		cLayer3->CharactersPerIteration = 4;
		Add(cLayer3);

		cLayer1->Start(0.01f);
		cLayer2->Start(0.02f);
		cLayer3->Start(0.04f);

		Add(copyright);

		BloomPostEffect* bloom = new BloomPostEffect();
		bloom->SetBloomFactor(0.8f);
		VGlobal::p()->PostProcess = std::move(std::unique_ptr<VPostEffectBase>(bloom));
	}

	virtual void Cleanup()
	{
		VSUPERCLASS::Cleanup();
		VGlobal::p()->PostProcess.reset();
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);
	}
};

//3D Graphics
/*
OpenGL 3D Graphics rendering. Limited but possible for 2.5D projects if I get around to writing a better model renderer.
*/
class Graphics3DState : public VSubState
{
	typedef VSubState VSUPERCLASS;

public:
	Graphics3DState() : VSubState() {}
	~Graphics3DState() = default;

	float timer = 0.0f;
	V3DScene* scene;
	V3DObject* model;
	V3DObject* moon;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		//std::vector<GLfloat> cube =
		//{
		//	// positions    // uv	// normals	//colors
		//	-80, 80, -80,	1, 0,	0, 1, 0,	1, 1, 1, 1,
		//	-80, 80, 80,	1, 1,	0, 1, 0,	1, 1, 1, 1,
		//	80, 80, 80,		0, 1,	0, 1, 0,	1, 1, 1, 1,
		//	80, 80, 80,		0, 1,	0, 1, 0,	1, 1, 1, 1,
		//	80, 80, -80,	0, 0,	0, 1, 0,	1, 1, 1, 1,
		//	-80, 80, -80,	1, 0,	0, 1, 0,	1, 1, 1, 1,
		//	-80, -80, -80,	0, 0,	0, -1, 0,	1, 1, 1, 1,
		//	80, -80, -80,	1, 0,	0, -1, 0,	1, 1, 1, 1,
		//	80, -80, 80,	1, 1,	0, -1, 0,	1, 1, 1, 1,
		//	80, -80, 80,	1, 1,	0, -1, 0,	1, 1, 1, 1,
		//	-80, -80, 80,	0, 1,	0, -1, 0,	1, 1, 1, 1,
		//	-80, -80, -80,	0, 0,	0, -1, 0,	1, 1, 1, 1,
		//	-80, 80, -80,	0, 0,	0, 0, -1,	1, 1, 1, 1,
		//	80, 80, -80,	1, 0,	0, 0, -1,	1, 1, 1, 1,
		//	80, -80, -80,	1, 1,	0, 0, -1,	1, 1, 1, 1,
		//	80, -80, -80,	1, 1,	0, 0, -1,	1, 1, 1, 1,
		//	-80, -80, -80,	0, 1,	0, 0, -1,	1, 1, 1, 1,
		//	-80, 80, -80,	0, 0,	0, 0, -1,	1, 1, 1, 1,
		//	80, 80, -80,	0, 0,	1, 0, 0,	1, 1, 1, 1,
		//	80, 80, 80,		1, 0,	1, 0, 0,	1, 1, 1, 1,
		//	80, -80, 80,	1, 1,	1, 0, 0,	1, 1, 1, 1,
		//	80, -80, 80,	1, 1,	1, 0, 0,	1, 1, 1, 1,
		//	80, -80, -80,	0, 1,	1, 0, 0,	1, 1, 1, 1,
		//	80, 80, -80,	0, 0,	1, 0, 0,	1, 1, 1, 1,
		//	80, 80, 80,		0, 0,	0, 0, 1,	1, 1, 1, 1,
		//	-80, 80, 80,	1, 0,	0, 0, 1,	1, 1, 1, 1,
		//	-80, -80, 80,	1, 1,	0, 0, 1,	1, 1, 1, 1,
		//	-80, -80, 80,	1, 1,	0, 0, 1,	1, 1, 1, 1,
		//	80, -80, 80,	0, 1,	0, 0, 1,	1, 1, 1, 1,
		//	80, 80, 80,		0, 0,	0, 0, 1,	1, 1, 1, 1,
		//	-80, 80, 80,	0, 0,	-1, 0, 0,	1, 1, 1, 1,
		//	-80, 80, -80,	1, 0,	-1, 0, 0,	1, 1, 1, 1,
		//	-80, -80, -80,	1, 1,	-1, 0, 0,	1, 1, 1, 1,
		//	-80, -80, -80,	1, 1,	-1, 0, 0,	1, 1, 1, 1,
		//	-80, -80, 80,	0, 1,	-1, 0, 0,	1, 1, 1, 1,
		//	-80, 80, 80,	0, 0,	-1, 0, 0,	1, 1, 1, 1,
		//};

		/*V3DModel* obj = new V3DModel();
		obj->LoadModelData(cube, 0, 5, 3, 8);
		obj->SetMaterial(sf::Color::White, sf::Color::White, 30.0f);
		obj->LoadTexture("Example/Assets/texture.jpg", true);
		obj->Scale = sf::Vector3f(0.01f, 0.01f, 0.01f);
		model = obj;*/

		model = new V3DObjModel();
		model->LoadModelData("Example/Assets/Low-Poly-Racing-Car.obj");

		scene = new V3DScene(0.0f, 0.0f, VGlobal::p()->Width, VGlobal::p()->Height);
		scene->SetLight(GL_LIGHT0, sf::Color(0, 0, 0), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Vector3f(1.0f, 0.0f, -1.0f));
		scene->SetGlobalLight(sf::Color(50, 50, 50));
		scene->Camera = std::make_unique<V3DPerspectiveCamera>(45.0f, 1.0f, 700.0f);
		scene->Add(model);
		scene->Add(moon);
		Add(scene);

		VPostEffectMultipass* multipass = new VPostEffectMultipass(2);
		ScoreboardEffect* scoreboard = new ScoreboardEffect();
		scoreboard->SetPointSize(0.75f);
		scoreboard->SetPointAmount(VGlobal::p()->Height / 10);
		scoreboard->SetBlur(true, 0.3f);
		multipass->AddPostEffect(scoreboard);
		/*BloomPostEffect* bloom = new BloomPostEffect();
		bloom->SetBloomFactor(0.4f);
		multipass->AddPostEffect(bloom);
		VGlobal::p()->PostProcess = scoreboard;*/
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);
		timer += dt * 15.0f;

		float x = 0;
		float y = 0;
		model->Position = sf::Vector3f(x, y, -5.0f);
		model->Rotation = sf::Vector3f(0.0f, -timer, 0);

		x -= cosf(timer / 27.322f) * 5.655f;
		float z = sinf(timer / 27.322f) * 5.655f;
		moon->Position = sf::Vector3f(x, y, z - 5.0f);
		moon->Rotation = sf::Vector3f(0.0f, -timer / 27, 0);

		scene->Camera->Position.x += VGlobal::p()->Input->CurrentAxisValue("leftX") * 0.01f * dt;
		scene->Camera->Position.y += VGlobal::p()->Input->CurrentAxisValue("leftY") * 0.01f * dt;
		scene->Camera->Rotate.x += VGlobal::p()->Input->CurrentAxisValue("rightX") * dt;
		scene->Camera->Rotate.y += VGlobal::p()->Input->CurrentAxisValue("rightY") * dt;
	}
};

class TrailAreaState : public VSubState
{
	typedef VSubState VSUPERCLASS;
public:
	TrailAreaState() : VSubState() {}
	~TrailAreaState() = default;
	VSprite* playerControl;
	VTrailArea* render;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		playerControl = new VSprite();
		playerControl->MakeGraphic(32, 32, sf::Color::White);
		playerControl->SetPositionAtCentre(VGlobal::p()->Width / 2.0f, VGlobal::p()->Height / 2.0f);
		playerControl->Drag = sf::Vector2f(500, 500);
		playerControl->MaxVelocity = sf::Vector2f(200, 200);

		render = new VTrailArea(VGlobal::p()->Width / 2.0f, 0, VGlobal::p()->Width / 2, VGlobal::p()->Height);
		render->Sprite->Drag = sf::Vector2f(500, 500);
		render->Sprite->MaxVelocity = sf::Vector2f(200, 200);
		render->AlphaMultiplier = 0.99f;
		render->Delay = 0.05f;
		render->Add(playerControl);
		render->RenderOutside = true;
		Add(render);

		VSprite* separator = new VSprite();
		separator->MakeGraphic(2, 320, sf::Color::White);
		separator->SetPositionAtCentre(VGlobal::p()->Width / 2.0f, 190.0f);
		Add(separator);
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);
		playerControl->Acceleration = sf::Vector2f(VGlobal::p()->Input->CurrentAxisValue("leftX") * 3, VGlobal::p()->Input->CurrentAxisValue("leftY") * 3);
	}
};

//Interpolation system
/*
Demonstration of all the different kind of easing functions, originally written by Robert Penning.
*/
class InterpolateState : public VSubState
{
	typedef VSubState VSUPERCLASS;
public:
	InterpolateState() : VSubState() {}
	~InterpolateState() = default;

	VSprite* ease[VInterpolate::NumInterpolationTypes];

	float a = 200.0f;						//Start
	float b = VGlobal::p()->Width - 200.0f; //Destination
	float d = 2.0f;							//Time taken

	sf::Clock timer;
	bool forward = true;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		sf::String VInterpolateNames[] =
		{
			"InBack",
			"OutBack",
			"InOutBack",
			"InBounce",
			"OutBounce",
			"InOutBounce",
			"InCirc",
			"OutCirc",
			"InOutCirc",
			"InCubic",
			"OutCubic",
			"InOutCubic",
			"InElastic",
			"OutElastic",
			"InOutElastic",
			"InExpo",
			"OutExpo",
			"InOutExpo",
			"Linear",
			"InQuad",
			"OutQuad",
			"InOutQuad",
			"InQuart",
			"OutQuart",
			"InOutQuart",
			"InQuint",
			"OutQuint",
			"InOutQuint",
			"InSine",
			"OutSine",
			"InOutSine",
		};

		auto text = new VText(15.0f, 50.0f - 4, (float)VGlobal::p()->Width - 20.0f, "", 9);
		text->Alignment = VTextAlign::ALIGNLEFT;
		text->ApplyChanges();
		Add(text);

		for (int i = 0; i < VInterpolate::NumInterpolationTypes; i++)
		{
			ease[i] = new VSprite();
			if (i == 0)
				ease[i]->MakeGraphic(6, 6, sf::Color::White);
			else
				ease[i]->LoadGraphicFromTexture(*ease[0]->GetTexture());

			ease[i]->SetPositionAtCentre(100.0f, 50.0f + (i * 10.1f));
			Add(ease[i]);

			text->Text += VInterpolateNames[i] + "\n";
		}
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

		float t = timer.getElapsedTime().asSeconds();
		if (t > d)
		{
			forward = !forward;
			timer.restart();
			t = 0.0f;
		}

		for (int i = 0; i < VInterpolate::NumInterpolationTypes; i++)
		{
			if (forward)
			{
				ease[i]->Position.x = VInterpolate::Float(a, b, t, d, (VInterpolate::VInterpolateType)i);
			}
			else
			{
				ease[i]->Position.x = VInterpolate::Float(b, a, t, d, (VInterpolate::VInterpolateType)i);
			}
		}
	}
};

//2D Lighting Shader
/*
To show that sprites can still use shaders without the need of VPostEffect
*/
class Lighting2DState : public VSubState
{
	typedef VSubState VSUPERCLASS;
public:
	Lighting2DState() : VSubState() {}
	~Lighting2DState() = default;
	VSprite* sprite;
	sf::Texture normalTex;
	sf::Shader lighting2DShader;

	float lightZ = 0.5f;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		sprite = new VSprite(0, 0, "Example/Assets/diffuse.png");
		sprite->SetPositionAtCentre(sf::Vector2f(320.0f, 180.0f));
		Add(sprite);

		normalTex.loadFromFile("Example/Assets/normal.png");

		lighting2DShader.loadFromFile("Example/Assets/Lighting.frag", sf::Shader::Fragment);
		lighting2DShader.setUniform("texture", sf::Shader::CurrentTexture);
		lighting2DShader.setUniform("height", normalTex);
		lighting2DShader.setUniform("light", sf::Vector3f(0.5f, 0.5f, 0.0f));
		lighting2DShader.setUniform("intensity", 1.0f);
		sprite->RenderState.shader = &lighting2DShader;
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

		sf::Vector2f mousePos = VGlobal::p()->GetMousePosition();
		mousePos -= sprite->Position;
		lighting2DShader.setUniform("light", sf::Vector3f(mousePos.x / sprite->Size.x, mousePos.y / sprite->Size.y, lightZ));
	}
};


//Backdrops
/*
Repeating sprite that will render on the entire game window, and will scroll based on the camera.
*/
class BackdropState : public VSubState
{
	typedef VSubState VSUPERCLASS;

	sf::Clock clock;

public:
	BackdropState() : VSubState() {}
	~BackdropState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		VBackdrop* backdrop[] =
		{
			new VBackdrop(0.1f, 0.1f, true, true, "Example/Assets/Backdrop.png"),
			new VBackdrop(0.2f, 0.2f, true, true, "Example/Assets/Backdrop.png"),
			new VBackdrop(0.3f, 0.3f, true, true, "Example/Assets/Backdrop.png"),
			new VBackdrop(0.4f, 0.4f, true, true, "Example/Assets/Backdrop.png"),
			new VBackdrop(0.5f, 0.5f, true, true, "Example/Assets/Backdrop.png"),
			new VBackdrop(0.6f, 0.6f, true, true, "Example/Assets/Backdrop.png"),
			new VBackdrop(0.7f, 0.7f, true, true, "Example/Assets/Backdrop.png"),
			new VBackdrop(0.8f, 0.8f, true, true, "Example/Assets/Backdrop.png"),
			new VBackdrop(0.9f, 0.9f, true, true, "Example/Assets/Backdrop.png"),
			new VBackdrop(1.0f, 1.0f, true, true, "Example/Assets/Backdrop.png"),
		};

		int length = sizeof(backdrop) / sizeof(VBackdrop*);

		for (int i = 0; i < length; i++)
		{
			backdrop[i]->CameraScroll = true;
			backdrop[i]->Scale *= backdrop[i]->ScrollFactor.x;
			backdrop[i]->SetTint(VColour::HSVtoRGB(0.0f, 0.0f, (i + 1.0f) / (length + 1)));
			Add(backdrop[i]);
		}

		UseParentCamera = true;
		ParentState->Cameras[0]->SetToBounds(false);
	}

	virtual void Cleanup()
	{
		VSUPERCLASS::Cleanup();
		ParentState->Cameras[0]->SetToBounds(true);

		ParentState->Cameras[0]->Position.x = 320.0f;
		ParentState->Cameras[0]->Position.y = 180.0f;
		ParentState->Cameras[0]->Zoom = 1.0f;
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

		ParentState->Cameras[0]->Position.x = 320.0f + (sinf(clock.getElapsedTime().asSeconds()) * 120.0f);
		ParentState->Cameras[0]->Position.y = 180.0f + (cosf(clock.getElapsedTime().asSeconds()) * 80.0f);
		ParentState->Cameras[0]->Zoom = 0.75f + (sinf(clock.getElapsedTime().asSeconds() * 0.5f) * 0.25f);
	}
};

//class DissolveState : public VSubState
//{
//	typedef VSubState VSUPERCLASS;
//
//public:
//	DissolveState() : VSubState() {}
//	~DissolveState() = default;
//
//	VPostEffect* postEffect;
//	VRenderGroup* vfx;
//
//	sf::Clock clock;
//
//	virtual void Initialise()
//	{
//		VSUPERCLASS::Initialise();
//
//		postEffect = new VPostEffect("Example/Assets/Dissolve.frag", sf::Shader::Fragment);
//		postEffect->SetInputTextureName("texture");
//		postEffect->SetParameter("textureSize", (float)VGlobal::p()->Width, (float)VGlobal::p()->Height);
//		postEffect->SetParameter("minAcc", 60.0f);
//		postEffect->SetParameter("maxAcc", 80.0f);
//		postEffect->SetParameter("sectionWidth", 1);
//		postEffect->SetParameter("seed", 0);
//		postEffect->SetParameter("bgColor", sf::Color::Red);
//
//		vfx = new VRenderGroup(0, 0, VGlobal::p()->Width, VGlobal::p()->Height);
//
//		VSprite* background = new VSprite();
//		background->LoadGraphic("Example/Assets/AWOO.png");
//
//		vfx->PostEffect = postEffect;
//		vfx->Add(background);
//		Add(vfx);
//	}
//
//	virtual void Update(float dt)
//	{
//		VSUPERCLASS::Update(dt);
//		postEffect->SetParameter("time", clock.getElapsedTime().asSeconds());
//	}
//};

#define TWOPI 6.283185307f

//Text Path
/*
Special text class that makes text display along a path.
*/
class WordShapeState : public VSubState
{
	typedef VSubState VSUPERCLASS;

public:
	WordShapeState() : VSubState() {}
	~WordShapeState() = default;

	static const int TEXT_COUNT = 20;
	static const int POINT_COUNT = (672 / 32);

	VTextPath* pathText[TEXT_COUNT];
	sf::Clock clock;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		for (int i = 0; i < TEXT_COUNT; i++)
		{
			VTextPath* t = new VTextPath(0.0f, (i * 18) + 8.0f, 640.0f);
			t->SetFormat("Example/Assets/DejaVuSansMono.ttf", 16, sf::Color::White, VTextAlign::ALIGNLEFT);
			t->Text = "Pixelation Art Jam - GDC 2017 ";
			t->ApplyChanges();

			if (i % 2)
				t->SetFillTint(VColour::HSVtoRGB(200.0f, 0.75f, 0.75f));
			else
				t->SetFillTint(VColour::HSVtoRGB(0.0f, 0.0f, 1.0f));

			for (int j = 0; j < POINT_COUNT; j++)
			{
				t->AddPoint(sf::Vector2f(j * 32.0f, 0.0f));
			}
			t->SetCurveType(VTextPath::PATH_ANCHOR);

			pathText[i] = t;
			Add(pathText[i]);
		}

		BloomPostEffect* bloom = new BloomPostEffect();
		bloom->SetBloomFactor(0.8f);
		VGlobal::p()->PostProcess = std::move(std::unique_ptr<VPostEffectBase>(bloom));
	}

	virtual void Cleanup()
	{
		VSUPERCLASS::Cleanup();
		VGlobal::p()->PostProcess.reset();
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

		float t = clock.getElapsedTime().asSeconds() / 5.0f;

		sf::Vector2f points[POINT_COUNT];
		for (int j = 0; j < POINT_COUNT; j++)
		{
			float sinF = (float)sinf(clock.getElapsedTime().asSeconds() + (TWOPI * ((float)j / POINT_COUNT)));
			points[j] = sf::Vector2f(j * 32.0f, sinF * 16.0f);
		}

		for (int i = 0; i < TEXT_COUNT; i++)
		{
			VTextPath* text = pathText[i];
			text->SetPathOffset(((i % 2 == 0) ? t : -t) + (0.005f * i));

			for (int j = 0; j < POINT_COUNT; j++)
			{
				text->UpdatePoint(j, points[j].x, points[j].y);
			}
		}
	}
};

class PixelMapRender : public VObject
{
	typedef VObject VSUPERCLASS;

public:
	sf::VertexArray Vertices;
	sf::RenderStates RenderState = sf::RenderStates::Default;

	virtual void Draw(sf::RenderTarget &RenderTarget)
	{
		VSUPERCLASS::Draw(RenderTarget);
		sf::Transformable transformable;
		transformable.setPosition(Position);
		transformable.setRotation(Angle);
		RenderState.transform = transformable.getTransform();
		RenderTarget.draw(Vertices, RenderState);
	}
};

#include "PerlinNoise.h"
//Async Function Test
class AsyncTestState : public VSubState
{
	typedef VSubState VSUPERCLASS;

private:
	PixelMapRender* pixel;
	VText* normalText;

	static const int CircleCount = 10;
	VSprite* circle[CircleCount];

	sf::Clock timer;

public:
	AsyncTestState() : VSubState() {}
	~AsyncTestState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		for (int i = 0; i < CircleCount; i++)
		{
			VSprite* c = new VSprite();
			if (i == 0)
			{
				c->MakeGraphicCircle(5, sf::Color::White);
			}
			else
			{
				c->LoadGraphicFromTexture(*circle[0]->GetTexture());
			}

			circle[i] = c;
			Add(c);
		}

		pixel = new PixelMapRender();
		Add(pixel);

		normalText = new VText(0.0f, VGlobal::p()->Height / 2.0f, VGlobal::p()->Width * 1.0f);
		normalText->Text = L"LOADING MAP";
		normalText->SetFormat("Example/Assets/DejaVuSansMono.ttf", 32, sf::Color::White, VTextAlign::ALIGNCENTER);
		Add(normalText);

        auto f = std::async(std::launch::async, std::bind(&AsyncTestState::LoadMap, this));
		VGlobal::p()->Async->LaunchAsyncFunction(f);
	}

	void LoadMap()
	{
		int height = VGlobal::p()->Height * 2;
		int width = VGlobal::p()->Width * 2;

		pixel->Vertices.setPrimitiveType(sf::Quads);
		pixel->Vertices.resize(width * height * 4);
		pixel->Size = sf::Vector2f(sf::Vector2i(width, height));
		std::vector<float> map = PerlinNoise::GenerateFloat(width, height, 3.0f, 7, 0.5f, 42);

		int pixelCount = 0;
		for (int y = 0; y < height; y++)
			for (int x = 0; x < width; x++)
			{
				int index = (y * width) + x;
				float value = map[index];

				int p = pixelCount * 4;
				pixel->Vertices[p + 0].position = sf::Vector2f(sf::Vector2i(x + 0, y + 0));
				pixel->Vertices[p + 0].color = sf::Color(
					sf::Uint8(255 * value),
					sf::Uint8(255 * value),
					sf::Uint8(255 * value),
					255);
				pixel->Vertices[p + 1].position = sf::Vector2f(sf::Vector2i(x + 1, y + 0));
				pixel->Vertices[p + 1].color = sf::Color(
					sf::Uint8(255 * value),
					sf::Uint8(255 * value),
					sf::Uint8(255 * value),
					255);
				pixel->Vertices[p + 2].position = sf::Vector2f(sf::Vector2i(x + 1, y + 1));
				pixel->Vertices[p + 2].color = sf::Color(
					sf::Uint8(255 * value),
					sf::Uint8(255 * value),
					sf::Uint8(255 * value),
					255);
				pixel->Vertices[p + 3].position = sf::Vector2f(sf::Vector2i(x + 0, y + 1));
				pixel->Vertices[p + 3].color = sf::Color(
					sf::Uint8(255 * value),
					sf::Uint8(255 * value),
					sf::Uint8(255 * value),
					255);

				pixelCount++;
			}

		VGlobal::p()->Async->SyncToMainLoop(std::bind(&AsyncTestState::OnLoadedMap, this));
	}

	void OnLoadedMap()
	{
		normalText->SetFillTint(sf::Color::Black);
		normalText->Text = "LOAD COMPLETE";

		for (int i = 0; i < CircleCount; i++)
		{
			circle[i]->Kill();
		}
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

		sf::Vector2f centrePosition = sf::Vector2f(VGlobal::p()->Width / 2.0f, VGlobal::p()->Height / 2.0f + 100.0f);
		float offset = 3.1415926f / CircleCount;
		float radius = 50.0f;

		pixel->Velocity = sf::Vector2f(VGlobal::p()->Input->CurrentAxisValue("leftX"), VGlobal::p()->Input->CurrentAxisValue("leftY"));

		if (pixel->Position.x > 0)
			pixel->Position.x = 0;
		if (pixel->Position.x + pixel->Size.x < VGlobal::p()->Width)
			pixel->Position.x = VGlobal::p()->Width - pixel->Size.x;
		if (pixel->Position.y > 0)
			pixel->Position.y = 0;
		if (pixel->Position.y + pixel->Size.y < VGlobal::p()->Height)
			pixel->Position.y = VGlobal::p()->Height - pixel->Size.y;

		for (int i = 0; i < CircleCount; i++)
		{
			float angle = offset * i;
			float sin = sinf(timer.getElapsedTime().asSeconds() + angle);

			sf::Vector2f dir = sf::Vector2f(cosf(angle), -sinf(angle));
			circle[i]->SetPositionAtCentre(centrePosition + (dir * (sin * radius)));
		}
	}
};


class DemoStatesManager : public VState
{
public:
	typedef VState VSUPERCLASS;

	int CurrentPage = 0;
	void SetNewPage();

	DemoStatesManager() : VState() {}
	virtual void Initialise() override;
	virtual void HandleEvents(const sf::Event& event) override;
};
