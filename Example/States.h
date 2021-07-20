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
#include "../VFrame/VRenderLayer.h"
#include "../VFrame/VTiledSprite.h"
#include "../VFrame/V9Slice.h"
#include "../VFrame/VTilemap.h"
#include "../VFrame/VTrailArea.h"
#include "../VFrame/VTextPath.h"
#include "../VFrame/VPath.h"
#include "../VFrame/VTypedText.h"
#include "../VFrame/VShape.h"
#include "../VFrame/XInputDevice.h"
#include "../VFrame/V3DScene.h"
#include "../VFrame/V3DLightShader.h"
#include "../VFrame/V3DModel.h"
#include "../VFrame/V3DObjModel.h"
#include "../VFrame/VPhysicsGroup.h"
#include "../VFrame/VTimer.h"
#include "../VFrame/VFiniteStateMachine.h"
#include "../VFrame/VCandle.h"

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
		sprite1->SetAlignment(VText::ALIGNCENTRE);

		animatedSprite = new VSprite(0, 0);
		animatedSprite->LoadGraphic("Example/Assets/Turret.png", true, 128, 128);
		animatedSprite->SetPositionAtCentre(2 * VGlobal::p()->Width / 3.0f, VGlobal::p()->Height / 2.0f - 50.0f);
		animatedSprite->Animation.AddAnimation("start", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }, 15.0f);
		animatedSprite->Animation.AddAnimation("shotgun", { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 }, 15.0f, true);
		animatedSprite->Animation.AddAnimation("railgun", { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46 }, 15.0f, true);
		animatedSprite->Animation.AddAnimation("doublegun", { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62 }, 15.0f, true);
		animatedSprite->Animation.Play("start");

		auto sprite2 = new VText(animatedSprite->Position.x, 50, animatedSprite->Size.x, "Animated Sprite", 16);
		sprite2->SetAlignment(VText::ALIGNCENTRE);

		auto text = new VText(VGlobal::p()->Width / 2.0f - 250.0f, 200, 500, "Press 1: Start Animation\nPress 2: Shotgun Animation\nPress 3: Railgun Animation\nPress 4: Double Barrel Animation", 21);

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
				VGlobal::p()->Sound->Play("select");
			}

			if (event.key.code == sf::Keyboard::Num2)
			{
				animatedSprite->Animation.Play("shotgun");
				VGlobal::p()->Sound->Play("select");
			}

			if (event.key.code == sf::Keyboard::Num3)
			{
				animatedSprite->Animation.Play("railgun");
				VGlobal::p()->Sound->Play("select");
			}

			if (event.key.code == sf::Keyboard::Num4)
			{
				animatedSprite->Animation.Play("doublegun");
				VGlobal::p()->Sound->Play("select");
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

		if (VGlobal::p()->Input->IsButtonPressed("A"))
		{
			animatedSprite->Animation.Play("start");
			VGlobal::p()->Sound->Play("select");
		}

		if (VGlobal::p()->Input->IsButtonPressed("B"))
		{
			animatedSprite->Animation.Play("shotgun");
			VGlobal::p()->Sound->Play("select");
		}

		if (VGlobal::p()->Input->IsButtonPressed("X"))
		{
			animatedSprite->Animation.Play("railgun");
			VGlobal::p()->Sound->Play("select");
		}

		if (VGlobal::p()->Input->IsButtonPressed("Y"))
		{
			animatedSprite->Animation.Play("doublegun");
			VGlobal::p()->Sound->Play("select");
		}
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
		sprite1->SetAlignment(VText::ALIGNCENTRE);

		animatedSprite = new VTiledSprite(0, 0);
		animatedSprite->LoadGraphic("Example/Assets/Waterfall.png", true, 32, 32);
		animatedSprite->Animation.AddAnimation("waterfall", { 0, 1, 2, 3 }, 15.0f, true);
		animatedSprite->Animation.Play("waterfall");
		animatedSprite->Size = sf::Vector2f(100, 100);
		animatedSprite->SetPositionAtCentre(2 * VGlobal::p()->Width / 3.0f, VGlobal::p()->Height / 2.0f);
		Add(animatedSprite);

		auto sprite2 = new VText(animatedSprite->Position.x, 80, animatedSprite->Size.x, "Animated TiledSprite", 16);
		sprite2->SetAlignment(VText::ALIGNCENTRE);

		auto text = new VText(VGlobal::p()->Width / 2.0f - 270.0f, 300, 540, "Press WASD: Resize Standard Tiled Sprite\nPress IJKL: Resize Animated Tiled Sprite", 21);

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

//Static and Animated Tiled Sprites
/*
Tiled sprites are like regular sprites, except the texture can repeat.
*/
class NineSliceState : public VSubState
{
	typedef VSubState VSUPERCLASS;

	V9Slice* nineSlice;

public:
	NineSliceState() : VSubState() {}
	~NineSliceState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		nineSlice = new V9Slice(0.0f, 0.0f, 100.0f, 100.0f, "Example/Assets/Box.png", sf::Vector2f(20.0f, 20.0f));
		nineSlice->Size = sf::Vector2f(100, 100);
		nineSlice->SetPositionAtCentre(VGlobal::p()->Width / 2.0f, VGlobal::p()->Height / 2.0f);
		Add(nineSlice);

		auto sprite1 = new VText(nineSlice->Position.x, 80, nineSlice->Size.x, "9 Slice Object", 16);
		sprite1->SetAlignment(VText::ALIGNCENTRE);

		auto text = new VText(VGlobal::p()->Width / 2.0f - 270.0f, 300, 540, "Press WASD: Resize 9-Slice Object", 21);

		Add(sprite1);
		Add(text);
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

		float x1Resize = VGlobal::p()->Input->CurrentAxisValue("leftX");
		float y1Resize = VGlobal::p()->Input->CurrentAxisValue("leftY");
		nineSlice->Size += sf::Vector2f(x1Resize, y1Resize) * dt;
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
		normalText->SetFormat("Example/Assets/DejaVuSansMono.ttf", 16, sf::Color::White, VText::ALIGNRIGHT, sf::Text::Italic | sf::Text::Underlined);
		normalText->SetText(L"!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~¡¢£¤¥¦§¨©ª«¬-®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿĀāĂăĄąĆćĈĉĊċČčĎďĐđĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłŃńŅņŇňŉŊŋŌōŎŏŐőŒœŔŕŖŗŘřŚśŜŝŞşŠšŢţŤťŦŧŨũŪūŬŭŮůŰűŲųŴŵŶŷŸŹźŻżŽžſƒǺǻǼǽǾǿȘșȚțˆˇˉ˘˙˚˛˜˝;΄΅Ά·ΈΉΊΌΎΏΐΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩΪΫάέήίΰαβγδεζηθικλμνξοπρςστυφχψωϊϋόύώЁЂЃЄЅІЇЈЉЊЋЌЎЏАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюяёђѓєѕіїјљњћќўџҐґẀẁẂẃẄẅỲỳ–—―‗‘’‚“”„†‡•…‰‹›‼‾⁄ⁿ₣₤₧€№™Ω⅛⅜⅝⅞←↑→↓↔↕↨∂∆∏∑−∙√∞∟∩∫≈≠≡≤≥⌂⌐⌠⌡─│┌┐└┘├┤┬┴┼═║╒╓╔╕╖╗╘╙╚╛╜╝╞╟╠╡╢╣╤╥╦╧╨╩╪╫╬▀▄█▌▐░▒▓■▬▲►▼◄◊○◘◙☺☻☼♀♂♠♣♥♦♪♫ﬁﬂ");
		normalText->SetWrap(VText::WRAPLETTER);
		Add(normalText);

		typedText = new VTypedText(10.0f, 50.0f, (float)VGlobal::p()->Width - 20.0f);
		typedText->SetFormat("Example/Assets/DejaVuSansMono.ttf", 16, sf::Color::White, VText::ALIGNLEFT, sf::Text::Bold);
		typedText->ResetText("Religion and politics often make some people lose all perspective and give way to ranting and raving and carrying on like emotional children");
		typedText->CursorChar = '_';
		typedText->CursorBlinkSpeed = 0.5f;
		typedText->SetWrap(VText::WRAPWORD);
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
			VGlobal::p()->Sound->Play("select");
		}
		if (VGlobal::p()->Input->IsButtonPressed("rightShoulder"))
		{
			typedText->Start(0.05f);
			VGlobal::p()->Sound->Play("select");
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
	VFiniteStateMachine* playerFSM;
	VShape* platform[2];
	VTimer* timer;
	VText* stateText;

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

		Add(tilemap);

		platform[0] = new VShape();
		platform[0]->SetRectangle(64, 32);
		platform[0]->SetFillTint(VColour::HSVtoRGB(0, 0.0f, 0.75f));
		platform[0]->SetOutlineThickness(1.0f);
		platform[0]->SetOutlineTint(sf::Color::Red);
		platform[0]->SetPositionAtCentre(24.5f * 32, 11.5f * 32.0f);
		platform[0]->Immovable = true;
		platform[0]->AllowCollisions = VObject::TOUCHNONE;
		platform[0]->Velocity.x = 32.0f;
		Add(platform[0]);

		platform[1] = new VShape();
		platform[1]->SetRectangle(64, 32);
		platform[1]->SetFillTint(VColour::HSVtoRGB(0, 0.0f, 0.75f));
		platform[1]->SetOutlineThickness(1.0f);
		platform[1]->SetOutlineTint(sf::Color::Red);
		platform[1]->SetPositionAtCentre(42.5f * 32, 7.5f * 32.0f);
		platform[1]->Immovable = true;
		platform[1]->AllowCollisions = VObject::TOUCHNONE;
		platform[1]->Velocity.y = 32.0f;
		Add(platform[1]);

		VGlobal::p()->WorldBounds.width = tilemap->Size.x;
		VGlobal::p()->WorldBounds.height = tilemap->Size.y;
		ParentState->Cameras[0]->SetToBounds(true);

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

		timer = TimeManager->AddTimer();

		enum PlayerStates
		{
			PLAYER_WALKING,
			PLAYER_JUMPING,
			PLAYER_STILL,
			PLAYER_POUND,
			PLAYER_WALL,
			NUM_PLAYER_STATES
		};

		playerControl = new VSprite();
		playerControl->LoadGraphic("Example/Assets/Player.png");
		playerControl->SetPositionAtCentre(playerPos);
		playerControl->MaxVelocity = sf::Vector2f(0, 0);
		Add(playerControl);

		stateText = new VText(10.0f, 120.0f, VGlobal::p()->Width - 20.0f);
		stateText->SetFormat("Example/Assets/DejaVuSansMono.ttf", 16, sf::Color::White, VText::ALIGNCENTRE);
		stateText->SetText("");
		stateText->SetOutlineThickness(2.0f);
		stateText->SetOutlineTint(sf::Color::Black);
		stateText->ScrollFactor *= 0.0f;
		Add(stateText);

		ParentState->Cameras[0]->Follow(playerControl, 0.5f, VCamera::PLATFORMER, 0.8f, 0.05f);
		UseParentCamera = true;

		playerFSM = new VFiniteStateMachine(playerControl, NUM_PLAYER_STATES);
		playerFSM->Add(PLAYER_WALKING, 
			[](VBase* base, float dt, void* data) -> int //Update
			{
				VObject* player = dynamic_cast<VObject*>(base);
				bool touchFloor = (player->Touching & VObject::TOUCHBOTTOM) > 0;

				if (touchFloor)
				{
					if (VGlobal::p()->Input->IsButtonPressed("A"))
						return PLAYER_JUMPING;
				}

				return -1;
			},
			[](VBase* base, void* data) //Enter
			{
				VObject* player = dynamic_cast<VObject*>(base);
				player->Drag = sf::Vector2f(500, 500);
				player->MaxVelocity = sf::Vector2f(200, 600);
				player->Acceleration.y = 980.0f;
			}
		);
		playerFSM->Add(PLAYER_JUMPING,
			[](VBase* base, float dt, void* data) -> int //Update
			{
				VObject* player = dynamic_cast<VObject*>(base);

				if ((player->Touching & VObject::TOUCHBOTTOM) > 0)
					return PLAYER_WALKING;

				if ((player->Touching & VObject::TOUCHWALL) > 0)
					return PLAYER_WALL;

				if (VGlobal::p()->Input->IsButtonPressed("A"))
					return PLAYER_STILL;

				return -1;
			},
			[](VBase* base, void* data) //Enter
			{
				VObject* player = dynamic_cast<VObject*>(base);
				player->Velocity.y = -450.0f;
				player->Touching = 0;
			},
			[](VBase* base, unsigned int state, void* data) //Exit
			{
				VSprite* player = dynamic_cast<VSprite*>(base);
				if ((player->Touching & VObject::TOUCHRIGHT) > 0)
					player->FlipX = true;

				if ((player->Touching & VObject::TOUCHLEFT) > 0)
					player->FlipX = false;
			});
		playerFSM->Add(PLAYER_STILL,
			[](VBase* base, float dt, void* data) -> int //Update
			{
				VObject* player = dynamic_cast<VObject*>(base);

				if (player->Angle > 360 || player->Angle < -360)
					return PLAYER_POUND;

				return -1;
			},
			[](VBase* base, void* data) //Enter
			{
				VSprite* player = dynamic_cast<VSprite*>(base);
				player->Drag = sf::Vector2f(0, 0);
				player->Velocity = sf::Vector2f(0, 0);
				player->MaxVelocity = sf::Vector2f(0, 0);
				player->Acceleration = sf::Vector2f(0, 0);
				player->AngleVelocity = player->FlipX ? -900.0f : 900.0f;
			},
			[](VBase* base, unsigned int state, void* data) //Exit
			{
				VObject* player = dynamic_cast<VObject*>(base);
				player->Angle = 0;
				player->MaxVelocity = sf::Vector2f(0, 800);
				player->Velocity.y = 800;
				player->AngleVelocity = 0;
			});

		playerFSM->Add(PLAYER_POUND,
			[](VBase* base, float dt, void* data) -> int //Update
			{
				VObject* player = dynamic_cast<VObject*>(base);

				if ((player->Touching & VObject::TOUCHBOTTOM) > 0)
					return PLAYER_WALKING;

				return -1;
			},
			nullptr, //Enter
			[this](VBase* base, unsigned int state, void* data) //Exit
			{
				ParentState->Cameras[0]->Shake(0.03f, 0.5f);
			});

		playerFSM->Add(PLAYER_WALL,
			[this](VBase* base, float dt, void* data) -> int //Update
			{
				VSprite* player = dynamic_cast<VSprite*>(base);

				if (VGlobal::p()->Input->IsButtonPressed("A"))
					return PLAYER_JUMPING;

				bool touchingWalls = false;
				if (player->FlipX)
					touchingWalls = tilemap->GetTileIDFromPosition(player->Position + sf::Vector2f(32.0f + 16.0f, 16.0f)) != '.';
				else
					touchingWalls = tilemap->GetTileIDFromPosition(player->Position + sf::Vector2f(-16.0f, 16.0f)) != '.';

				if ((player->Touching & VObject::TOUCHBOTTOM) > 0 || !touchingWalls)
					return PLAYER_WALKING;

				return -1;
			},
			[](VBase* base, void* data) //Enter
			{
				VSprite* player = dynamic_cast<VSprite*>(base);
				player->MaxVelocity *= 0.9f;
				player->MaxVelocity.x = 0.0f;
			},
			[](VBase* base, unsigned int state, void* data) //Exit
			{
				VSprite* player = dynamic_cast<VSprite*>(base);
				player->Drag.x = 200;
				player->MaxVelocity = sf::Vector2f(200, 600);

				if (state == PLAYER_JUMPING)
				{
					if (player->FlipX)
						player->Velocity.x = -player->MaxVelocity.x;
					else
						player->Velocity.x = player->MaxVelocity.x;
				}
			});
	}

	virtual void Cleanup()
	{
		VSUPERCLASS::Cleanup();
		VGlobal::p()->WorldBounds.width = static_cast<float>(VGlobal::p()->Width);
		VGlobal::p()->WorldBounds.height = static_cast<float>(VGlobal::p()->Height);
	}

	virtual void Update(float dt)
	{
		sf::String stateNames[]
		{
			"PLAYER_WALKING",
			"PLAYER_JUMPING",
			"PLAYER_STILL",
			"PLAYER_POUND",
			"PLAYER_WALL",
			"NUM_PLAYER_STATES"
		};

		stateText->SetText(stateNames[playerFSM->GetCurrentState()]);

		playerFSM->Update(dt);
		
		bool touchFloor = (playerControl->Touching & VObject::TOUCHBOTTOM) > 0;
		playerControl->Acceleration.x = 0;
		if (VGlobal::p()->Input->CurrentAxisValue("leftX") < 0)
			playerControl->Acceleration.x -= playerControl->MaxVelocity.x * (touchFloor ? 4 : 3);
		if (VGlobal::p()->Input->CurrentAxisValue("leftX") > 0)
			playerControl->Acceleration.x += playerControl->MaxVelocity.x * (touchFloor ? 4 : 3);

		if (playerControl->Velocity.x != 0)
			playerControl->FlipX = playerControl->Velocity.x < 0;

		VSUPERCLASS::Update(dt);

		if (timer->Seconds() > 2.0f)
		{
			platform[0]->Velocity.x *= -1;
			platform[1]->Velocity.y *= -1;
			timer->Restart();
		}

		VGlobal::p()->Collides(playerControl, tilemap);
		VGlobal::p()->Collides(playerControl, platform[0], std::bind(&TilemapState::CloudPlatform, this, std::placeholders::_1, std::placeholders::_2));
		VGlobal::p()->Collides(playerControl, platform[1], std::bind(&TilemapState::CloudPlatform, this, std::placeholders::_1, std::placeholders::_2));
	}

	void CloudPlatform(VBase* player, VBase* platform)
	{
		VObject* o = dynamic_cast<VObject*>(player);
		VObject* p = dynamic_cast<VObject*>(platform);

		if (VGlobal::p()->Input->CurrentAxisValue("leftY") > 20.0f) //Down Press
		{
			p->AllowCollisions = VObject::TOUCHNONE;
		}
		else if (o->Position.y < p->Position.y)
		{
			p->AllowCollisions = VObject::TOUCHTOP;
		}
	}
};

//Collision
/*
Test both overlap and colliding, in either circle or rectangle collision.
*/
class CollisionState : public VSubState
{
	typedef VSubState VSUPERCLASS;

	VShape* playerControl;
	VGroup* overlapGroup;
	VGroup* collideGroup;

	float timer = 0.0f;

public:
	CollisionState() : VSubState() {}
	~CollisionState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		VRenderLayer* layer = new VRenderLayer();

		playerControl = new VShape();
		playerControl->SetRectangle(32, 32);
		playerControl->SetPositionAtCentre(VGlobal::p()->Width / 2.0f, 300.0f);
		playerControl->Drag = sf::Vector2f(500, 500);
		playerControl->MaxVelocity = sf::Vector2f(200, 200);
		layer->Add(playerControl);

		overlapGroup = new VGroup(2);
		auto sprite1 = new VShape();
		sprite1->SetRectangle(32, 32);
		sprite1->SetFillTint(sf::Color::Black);
		sprite1->SetOutlineThickness(2.0f);
		sprite1->SetOutlineTint(sf::Color::Cyan);
		sprite1->SetPositionAtCentre(128, VGlobal::p()->Height / 2.0f);
		auto sprite2 = new VShape();
		sprite2->SetCircle(16);
		sprite2->SetFillTint(sf::Color::Black);
		sprite2->SetOutlineThickness(2.0f);
		sprite2->SetOutlineTint(sf::Color::Green);
		sprite2->SetPositionAtCentre(256, VGlobal::p()->Height / 2.0f);
		overlapGroup->Add(sprite1);
		overlapGroup->Add(sprite2);

		collideGroup = new VGroup(2);
		auto sprite3 = new VShape();
		sprite3->SetCircle(16, 6); //Use second parameter to make a specified equilateral sided shape.
		sprite3->SetFillTint(sf::Color::Black);
		sprite3->SetOutlineThickness(2.0f);
		sprite3->SetOutlineTint(sf::Color::Magenta);
		sprite3->SetPositionAtCentre(384, VGlobal::p()->Height / 2.0f);
		sprite3->Drag = sf::Vector2f(250, 250);

		auto sprite4 = new VShape();
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
		sprite4->SetConvex(starPoints);
		sprite4->SetFillTint(sf::Color::Black);
		sprite4->SetOutlineThickness(2.0f);
		sprite4->SetOutlineTint(sf::Color::Yellow);
		sprite4->SetPositionAtCentre(512, VGlobal::p()->Height / 2.0f);
		sprite4->Immovable = true;

		collideGroup->Add(sprite3);
		collideGroup->Add(sprite4);

		layer->Add(overlapGroup);
		layer->Add(collideGroup);

		auto overlapText = new VText(sprite1->Position.x, 100.0f, 160, "Overlap These", 16);
		overlapText->SetFormat("Example/Assets/DejaVuSansMono.ttf", 16);
		overlapText->SetAlignment(VText::ALIGNCENTRE);
		auto collideText = new VText(sprite3->Position.x, 100.0f, 160, "Collide These", 16);
		collideText->SetFormat("Example/Assets/DejaVuSansMono.ttf", 16);
		collideText->SetAlignment(VText::ALIGNCENTRE);

		layer->Add(overlapText);
		layer->Add(collideText);

		Add(layer);
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

		sf::View defaultView = ParentState->DefaultCamera->GetDefaultView();
		if (!VObject::TestInView(ParentState->DefaultCamera->GetView(), &defaultView, playerControl))
		{
			VGlobal::p()->BackgroundColor = sf::Color::White;
		}

		timer += dt;
	}

	void overlapResponse(VObject* player, VObject* object)
	{
		VShape* sprite = dynamic_cast<VShape*>(object);
		VGlobal::p()->BackgroundColor = sprite->GetOutlineTint();
	}
};

//Path
/*
Tests the VPath class
*/
class PathState : public VSubState
{
	typedef VSubState VSUPERCLASS;

	VSprite* playerControl;
	VPath* playerPath;
	VGroup* pathPoints;
	VText* stateText;
	float timer = 0.0f;

public:
	PathState() : VSubState() {}
	~PathState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		pathPoints = new VGroup();
		Add(pathPoints);

		playerControl = new VSprite(0.0f, 0.0f, "Example/Assets/Player.png");
		playerControl->SetPositionAtCentre(100.0f, VGlobal::p()->Height / 2.0f);
		Add(playerControl);

		stateText = new VText(10.0f, 60.0f, VGlobal::p()->Width - 20.0f);
		stateText->SetFormat("Example/Assets/DejaVuSansMono.ttf", 12, sf::Color::White, VText::ALIGNLEFT);
		stateText->SetText("");
		stateText->SetOutlineThickness(2.0f);
		stateText->SetOutlineTint(sf::Color::Black);
		stateText->ScrollFactor *= 0.0f;
		Add(stateText);

		playerPath = new VPath();
		playerPath->SetUpdateAngle(true);
		playerPath->SetPathType(VPath::LINE);
		playerPath->SetInterpolationType(VInterpolate::OutElastic);
	}

	void AddPoint(sf::Vector2f p)
	{
		pathPoints->exists = true;
		pathPoints->alive = true;

		VShape* point = dynamic_cast<VShape*>(pathPoints->FirstDead());
		if (point)
		{
			point->Revive();
		}
		else
		{
			point = new VShape();
			point->SetCircle(5.0f);
			point->SetFillTint(sf::Color::Red);
			pathPoints->Add(point);
		}

		point->SetPositionAtCentre(p);
	}

	virtual void HandleEvents(const sf::Event& event)
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (playerPath->GetNumPoints() == 0)
				{
					playerPath->AddPoint(playerControl->Position + (playerControl->Size / 2.0f));
					AddPoint(playerControl->Position + (playerControl->Size / 2.0f));
				}

				sf::Vector2f mousePos = VGlobal::p()->GetMousePosition();
				playerPath->AddPoint(mousePos);
				AddPoint(mousePos);
				playerPath->SetSpeed(1.0f / (playerPath->GetNumPoints() - 1));
			}
		}

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Space)
			{
				playerPath->StartFollowing(playerControl, std::bind(&PathState::clearPath, this));
			}

			if (event.key.code == sf::Keyboard::O)
			{
				playerPath->SetPathType(playerPath->GetPathType() == VPath::LINE ? VPath::CURVE : VPath::LINE);
			}

			if (event.key.code == sf::Keyboard::K)
			{
				playerPath->SetInterpolationType((VInterpolate::VInterpolateType)(((int)playerPath->GetInterpolationType() - 1 + VInterpolate::NumInterpolationTypes) % VInterpolate::NumInterpolationTypes));
			}

			if (event.key.code == sf::Keyboard::L)
			{
				playerPath->SetInterpolationType((VInterpolate::VInterpolateType)(((int)playerPath->GetInterpolationType() + 1 + VInterpolate::NumInterpolationTypes) % VInterpolate::NumInterpolationTypes));
			}
		}
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);
		playerPath->Update(dt);

		std::string lineTypes[VPath::NUMPATHTYPES] = {
			"LINE", "CURVE"
		};

		std::string interpolationTypes[VInterpolate::NumInterpolationTypes] = {
			"InBack", "OutBack", "InOutBack", "InBounce", "OutBounce", "InOutBounce",
			"InCirc", "OutCirc", "InOutCirc", "InCubic", "OutCubic", "InOutCubic",
			"InElastic", "OutElastic", "InOutElastic", "InExpo", "OutExpo", "InOutExpo",
			"Linear", "InQuad", "OutQuad", "InOutQuad", "InQuart", "OutQuart", "InOutQuart",
			"InQuint", "OutQuint", "InOutQuint", "InSine", "OutSine", "InOutSine",
		};

		sf::String s;
		s = "Line Type (O): " + lineTypes[playerPath->GetPathType()] + "\n";
		s += "Interpolation Type (K/L): " + interpolationTypes[playerPath->GetInterpolationType()];

		stateText->SetText(s);
	}

	virtual void Draw(sf::RenderTarget& RenderTarget)
	{
		playerPath->Draw(RenderTarget);
		VSUPERCLASS::Draw(RenderTarget);
	}

	void clearPath()
	{
		playerPath->ClearPath();
		pathPoints->Kill();
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
			float x = (i % 15) * 40.0f;
			float y = (i / 15) * 40.0f;

			VText* t = new VText(25.0f + x, 80.0f + y - 6, 20.0f, std::to_string(i) + ":", 12);

			VShape* b = new VShape(40.0f + x, 70.0f + y);
			b->SetRectangle(20, 20);

			text->Add(t);
			buttons->Add(b);
		}

		int buttonOffset = (buttonCount / 10) + 1;

		for (unsigned int i = 0; i < axisCount; i++)
		{
			float x = (i % 12) * 40.0f;
			float y = (i / 12) * 40.0f;

			VText* t = new VText(25.0f + x, 120.0f + y + (buttonOffset * 40.0f) - 6.0f, 20.0f, std::to_string(i) + ":", 12);

			VShape* a = new VShape(40.0f + x, 120.0f + y + (buttonOffset * 40.0f));
			a->SetRectangle(20, 50);

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
			VShape* b = dynamic_cast<VShape*>(buttons->GetGroupItem(i));

			if (GamepadButtonDown(GAMEPAD_0, (GAMEPAD_BUTTON)i))
			{
				b->SetFillTint(sf::Color::White);
			}
			else
			{
				b->SetFillTint(VColour::HSVtoRGB(0.0f, 0.0f, 0.6f));
			}
		}

		for (unsigned int i = 0; i < axisCount; i++)
		{
			VShape* b = dynamic_cast<VShape*>(axis->GetGroupItem(i));

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
			VShape* b = dynamic_cast<VShape*>(buttons->GetGroupItem(i));

			if (sf::Joystick::isButtonPressed(VGlobal::p()->Input->GetJoystickID(0), i))
			{
				b->SetFillTint(sf::Color::White);
			}
			else
			{
				b->SetFillTint(VColour::HSVtoRGB(0.0f, 0.0f, 0.6f));
			}
		}

		for (unsigned int i = 0; i < axisCount; i++)
		{
			VShape* b = dynamic_cast<VShape*>(axis->GetGroupItem(i));
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
			VShape* b = dynamic_cast<VShape*>(buttons->GetGroupItem(i));

			if (sf::XInputDevice::isButtonPressed(0, buttonIDs[i]))
			{
				b->SetFillTint(sf::Color::White);
			}
			else
			{
				b->SetFillTint(VColour::HSVtoRGB(0.0f, 0.0f, 0.6f));
			}
		}

		for (unsigned int i = 0; i < axisCount; i++)
		{
			VShape* b = dynamic_cast<VShape*>(axis->GetGroupItem(i));
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
		render->RenderOutside = VRenderGroup::VRENDERGROUP_BEFORE;
		render->Sprite->SetPositionAtCentre(320, 200);

		VPostEffect* postEffect = new VPostEffect("Example/Assets/AlphaThreshold.frag", sf::Shader::Fragment);
		postEffect->SetParameter("threshold", 0.85f);
		postEffect->SetParameter("smoothness", 0.40f);
		postEffect->SetParameter("unpremultiply", 1.0f);
		render->PostEffect = std::move(std::unique_ptr<VPostEffect>(postEffect));

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
		copyright->SetAlignment(VText::ALIGNCENTRE);

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
		cLayer1->SetFormat("Example/Assets/DejaVuSansMono.ttf", 13, sf::Color(255, 255, 255, 24), VText::ALIGNLEFT);
		cLayer1->ResetText(ws.str());
		cLayer1->CharactersPerIteration = 4;
		cLayer1->SetSound("type", 20.0f, "Example/Assets/type.ogg");
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
		cLayer2->SetFormat("Example/Assets/DejaVuSansMono.ttf", 13, sf::Color(0, 255, 0, 64), VText::ALIGNLEFT);
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
		cLayer3->SetFormat("Example/Assets/DejaVuSansMono.ttf", 13, sf::Color(0, 255, 0, 200), VText::ALIGNLEFT);
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
		VGlobal::p()->PostProcess = nullptr;
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);
	}
};

#include "ScoreboardEffect.h"

#ifndef VFRAME_NO_3D
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
	V3DScene* scene = nullptr;
	V3DLightShader* lightShader = nullptr;
	V3DModel* lightModel = nullptr;
	sf::Vector3f lightPos;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		sf::ContextSettings settings;
		settings.depthBits = 24;

		scene = new V3DScene(0.0f, 0.0f, VGlobal::p()->Width, VGlobal::p()->Height, settings);
		
		float aspectRatio = scene->Sprite->Size.x / scene->Sprite->Size.y;
		std::unique_ptr<V3DCamera> MainCamera = std::make_unique<V3DPerspectiveCamera>(sf::Vector3f(0.0f, 0.0f, 0.0f), 50.0f, aspectRatio, 0.1f, 100.0f);
		scene->Camera.push_back(std::move(MainCamera));
		scene->Camera[0]->LookAt(sf::Vector3f(0.0f, 0.0f, 10.0f));
		//scene->Shader = std::make_unique<V3DShader>();

		std::unique_ptr<V3DLightShader> lightShader = std::make_unique<V3DLightShader>();
		if (lightShader)
		{
			lightShader->Lights[0] = std::make_unique<V3DLight>(LightType::SPOT, lightPos, sf::Vector3f(), sf::Vector3f(0, 1, 0), 20.0f, 0.1f, 0.001f);
			lightShader->Lights[1] = std::make_unique<V3DLight>(LightType::DIRECTION, sf::Vector3f(-1, 1, 0), sf::Vector3f(), sf::Vector3f(1, 1, 1), 20.0f, 0.2f, 0.002f);
		}
		scene->Shader = std::move(lightShader);

		V3DVertexArray vertices;
		std::vector<unsigned int> indices = 
		{	
			2, 1, 0, 3, 2, 0, //front
			4, 5, 6, 4, 6, 7, //right
			8, 9, 10, 8, 10, 11, //back
			14, 13, 12, 15, 14, 12, //left
			16, 17, 18, 16, 18, 19, //upper
			22, 21, 20, 23, 22, 20  //bottom
		};

		//front
		vertices.emplace_back(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0, 0.0, -1.0),	glm::vec4(1, 1, 1, 1), glm::vec2(0, 0));
		vertices.emplace_back(glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.0, 0.0, -1.0),	glm::vec4(1, 1, 1, 1), glm::vec2(1, 0));
		vertices.emplace_back(glm::vec3(0.5f, 0.5f, -0.5f),		glm::vec3(0.0, 0.0, -1.0),	glm::vec4(1, 1, 1, 1), glm::vec2(1, 1));
		vertices.emplace_back(glm::vec3(-0.5f, 0.5f, -0.5f),	glm::vec3(0.0, 0.0, -1.0),	glm::vec4(1, 1, 1, 1), glm::vec2(0, 1));

		//right
		vertices.emplace_back(glm::vec3(0.5f, 0.5f, -0.5f),		glm::vec3(1.0, 0.0, 0.0),	glm::vec4(1, 0, 0, 1), glm::vec2(1, 1));
		vertices.emplace_back(glm::vec3(0.5f, 0.5f, 0.5f),		glm::vec3(1.0, 0.0, 0.0),	glm::vec4(1, 0, 0, 1), glm::vec2(0, 1));
		vertices.emplace_back(glm::vec3(0.5f, -0.5f, 0.5f),		glm::vec3(1.0, 0.0, 0.0),	glm::vec4(1, 0, 0, 1), glm::vec2(0, 0));
		vertices.emplace_back(glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(1.0, 0.0, 0.0),	glm::vec4(1, 0, 0, 1), glm::vec2(1, 0));

		//back
		vertices.emplace_back(glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(0.0, 0.0, 1.0),	glm::vec4(0, 1, 0, 1), glm::vec2(0, 0));
		vertices.emplace_back(glm::vec3(0.5f, -0.5f, 0.5f),		glm::vec3(0.0, 0.0, 1.0),	glm::vec4(0, 1, 0, 1), glm::vec2(1, 0));
		vertices.emplace_back(glm::vec3(0.5f, 0.5f, 0.5f),		glm::vec3(0.0, 0.0, 1.0),	glm::vec4(0, 1, 0, 1), glm::vec2(1, 1));
		vertices.emplace_back(glm::vec3(-0.5f, 0.5f, 0.5f),		glm::vec3(0.0, 0.0, 1.0),	glm::vec4(0, 1, 0, 1), glm::vec2(0, 1));

		//left
		vertices.emplace_back(glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(-1.0, 0.0, 0.0),	glm::vec4(0, 0, 1, 1), glm::vec2(0, 0));
		vertices.emplace_back(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(-1.0, 0.0, 0.0),	glm::vec4(0, 0, 1, 1), glm::vec2(1, 0));
		vertices.emplace_back(glm::vec3(-0.5f, 0.5f, -0.5f),	glm::vec3(-1.0, 0.0, 0.0),	glm::vec4(0, 0, 1, 1), glm::vec2(1, 1));
		vertices.emplace_back(glm::vec3(-0.5f, 0.5f, 0.5f),		glm::vec3(-1.0, 0.0, 0.0),	glm::vec4(0, 0, 1, 1), glm::vec2(0, 1));

		//upper
		vertices.emplace_back(glm::vec3(0.5f, -0.5f, 0.5f),		glm::vec3(0.0, -1.0, 0.0),	glm::vec4(1, 1, 0, 1), glm::vec2(0, 1));
		vertices.emplace_back(glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(0.0, -1.0, 0.0),	glm::vec4(1, 1, 0, 1), glm::vec2(1, 1));
		vertices.emplace_back(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0, -1.0, 0.0),	glm::vec4(1, 1, 0, 1), glm::vec2(1, 0));
		vertices.emplace_back(glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.0, -1.0, 0.0),	glm::vec4(1, 1, 0, 1), glm::vec2(0, 0));

		//bottom
		vertices.emplace_back(glm::vec3(-0.5f, 0.5f, -0.5f),	glm::vec3(0.0, 1.0, 0.0),	glm::vec4(0, 1, 1, 1), glm::vec2(0, 1));
		vertices.emplace_back(glm::vec3(0.5f, 0.5f, -0.5f),		glm::vec3(0.0, 1.0, 0.0),	glm::vec4(0, 1, 1, 1), glm::vec2(1, 1));
		vertices.emplace_back(glm::vec3(0.5f, 0.5f, 0.5f),		glm::vec3(0.0, 1.0, 0.0),	glm::vec4(0, 1, 1, 1), glm::vec2(1, 0));
		vertices.emplace_back(glm::vec3(-0.5f, 0.5f, 0.5f),		glm::vec3(0.0, 1.0, 0.0),	glm::vec4(0, 1, 1, 1), glm::vec2(0, 0));

		V3DModel* model = new V3DModel(1.0f, 0.0f, 5.0f);
		model->LoadTexture("Example/Assets/texture.jpg");
		model->Material->Specular = sf::Vector3f(1.0f, 1.0f, 1.0f);
		model->Material->Shininess = 80.0f;
		model->LoadModelData(vertices, indices);
		model->Rotation.x = -20.0f;
		model->AngleVelocity.y = 22.5f;
		scene->Add(model);

		V3DObjModel* obj = new V3DObjModel(-1.0f, 0.0f, 5.0f);
		obj->LoadModelData("Example/Assets/Nimrud/nimrud.obj");
		obj->Scale = sf::Vector3f(0.2f, 0.2f, 0.2f);
		obj->AngleVelocity.y = -45.0f;
		obj->Rotation.x = -90.0f;
		scene->Add(obj);

		lightModel = new V3DModel();
		lightModel->LoadModelData(vertices, indices);
		lightModel->Position = lightPos;
		lightModel->Scale = sf::Vector3f(0.2f, 0.2f, 0.2f);
		lightModel->Material->Specular = sf::Vector3f(1.0f, 1.0f, 1.0f);
		lightModel->Material->Shininess = 80.0f;
		scene->Add(lightModel);

		Add(scene);

		/*VPostEffectMultipass* multipass = new VPostEffectMultipass(2);
		ScoreboardEffect* scoreboard = new ScoreboardEffect();
		scoreboard->SetPointSize(0.75f);
		scoreboard->SetPointAmount(VGlobal::p()->Height / 10);
		scoreboard->SetBlur(true, 0.3f);
		multipass->AddPostEffect(scoreboard);
		BloomPostEffect* bloom = new BloomPostEffect();
		bloom->SetBloomFactor(0.4f);
		multipass->AddPostEffect(bloom);
		VGlobal::p()->PostProcess = scoreboard;*/
	}

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);
		timer += dt;

		lightPos.x = sinf(timer) * 2.5f;
		lightPos.y = -sinf(timer) * 2.5f;
		lightPos.z = cosf(timer) * 2.5f;

		if (lightShader)
		{
			if (lightShader->Lights[0]->Type == LightType::DIRECTION)
				lightShader->Lights[0]->Position = lightPos;
			else
				lightShader->Lights[0]->Position = lightPos + sf::Vector3f(0.0f, 0.0f, 5.0f);

			lightShader->Lights[0]->Dir = -lightPos;
		}
		lightModel->Position = lightPos + sf::Vector3f(0.0f, 0.0f, 5.0f);
	}
};
#endif

class TrailAreaState : public VSubState
{
	typedef VSubState VSUPERCLASS;
public:
	TrailAreaState() : VSubState() {}
	~TrailAreaState() = default;
	VShape* playerControl;
	VTrailArea* render;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		playerControl = new VShape();
		playerControl->SetRectangle(32, 32);
		playerControl->SetPositionAtCentre(VGlobal::p()->Width / 2.0f, VGlobal::p()->Height / 2.0f);
		playerControl->Drag = sf::Vector2f(500, 500);
		playerControl->MaxVelocity = sf::Vector2f(200, 200);

		render = new VTrailArea(VGlobal::p()->Width / 2.0f, 0, VGlobal::p()->Width / 2, VGlobal::p()->Height);
		render->AlphaMultiplier = 0.99f;
		render->Delay = 0.05f;
		render->Add(playerControl);
		render->RenderOutside = VRenderGroup::VRENDERGROUP_BEFORE;
		Add(render);

		VShape* separator = new VShape();
		separator->SetRectangle(2, 320);
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

	VShape* ease[VInterpolate::NumInterpolationTypes];

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
		text->SetAlignment(VText::ALIGNLEFT);
		Add(text);

		for (int i = 0; i < VInterpolate::NumInterpolationTypes; i++)
		{
			ease[i] = new VShape();
			ease[i]->SetRectangle(6, 6);
			ease[i]->SetPositionAtCentre(100.0f, 50.0f + (i * 10.1f));
			Add(ease[i]);

			text->SetText(text->GetText() + VInterpolateNames[i] + "\n");
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

		if (sf::Touch::isDown(0))
		{
			mousePos = sf::Vector2f(sf::Touch::getPosition(0));
		}

		mousePos -= sprite->Position;
		lighting2DShader.setUniform("light", sf::Vector3f(mousePos.x / sprite->Size.x, mousePos.y / sprite->Size.y, lightZ));
	}
};

#ifndef VFRAME_NO_CANDLE
class CandleState : public VSubState
{
	typedef VSubState VSUPERCLASS;
public:
	CandleState() : VSubState() {}
	~CandleState() = default;

	VCandleArea* area;
	VLight* light;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		VGlobal::p()->BackgroundColor = sf::Color::White;

		area = new VCandleArea(true, sf::Vector2f(0.f, 0.f), sf::Vector2f(VGlobal::p()->Width, VGlobal::p()->Height));
		area->SetAreaColor(sf::Color::Black);
		area->SetAreaOpacity(0.75f);

		light = area->AddRadialLight();
		light->SetRange(300);

		VEdge* edge = area->MakeLineEdge();
		edge->Position = sf::Vector2f(VGlobal::p()->Width / 2.f, VGlobal::p()->Height / 2.f);
		edge->Size = sf::Vector2f(-10.f, 50.f);

		edge = area->MakeLineEdge();
		edge->Position = sf::Vector2f((VGlobal::p()->Width / 2.f) - 10.f, (VGlobal::p()->Height / 2.f) + 50.f);
		edge->Size = sf::Vector2f(10.f, 0.f);

		VObject* box = area->MakeBoxEdge();
		box->Position = sf::Vector2f(200.f, 100.f);
		box->Size = sf::Vector2f(50.f, 50.f);

		box = area->MakeBoxEdge();
		box->Position = sf::Vector2f(VGlobal::p()->Width - 200.f, 100.f);
		box->Size = sf::Vector2f(50.f, 50.f);

		box = area->MakeBoxEdge();
		box->Position = sf::Vector2f(100.f, 250.f);
		box->Size = sf::Vector2f(50.f, 50.f);

		box = area->MakeBoxEdge();
		box->Position = sf::Vector2f(200.f, 300.f);
		box->Size = sf::Vector2f(50.f, 50.f);

		box = area->MakeBoxEdge();
		box->Position = sf::Vector2f(320.f, 300.f);
		box->Size = sf::Vector2f(50.f, 50.f);

		box = area->MakeBoxEdge();
		box->Position = sf::Vector2f(440.f, 300.f);
		box->Size = sf::Vector2f(50.f, 50.f);

		box = area->MakeBoxEdge();
		box->Position = sf::Vector2f(540.f, 250.f);
		box->Size = sf::Vector2f(50.f, 50.f);

		Add(area);
	}

	virtual void Update(float dt)
	{
		light->SetPosition(VGlobal::p()->GetMousePosition());
		VSUPERCLASS::Update(dt);
	}
};
#endif

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

	virtual void Update(float dt)
	{
		VSUPERCLASS::Update(dt);

		ParentState->Cameras[0]->Position.x = 320.0f + (sinf(clock.getElapsedTime().asSeconds()) * 120.0f);
		ParentState->Cameras[0]->Position.y = 180.0f + (cosf(clock.getElapsedTime().asSeconds()) * 80.0f);
		ParentState->Cameras[0]->Zoom = 0.75f + (sinf(clock.getElapsedTime().asSeconds() * 0.5f) * 0.25f);
	}
};

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
			t->SetText("Pixelation Art Jam - GDC 2017 ");

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
		VGlobal::p()->PostProcess = nullptr;
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

	virtual void Destroy()
	{
		VSUPERCLASS::Destroy();
		Vertices.clear();
	}

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
	VShape* circle[CircleCount];

	sf::Clock timer;

	std::future<void> future;

public:
	AsyncTestState() : VSubState() {}
	~AsyncTestState() = default;

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		for (int i = 0; i < CircleCount; i++)
		{
			VShape* c = new VShape();
			c->SetCircle(5);

			circle[i] = c;
			Add(c);
		}

		pixel = new PixelMapRender();
		Add(pixel);

		normalText = new VText(0.0f, VGlobal::p()->Height / 2.0f, VGlobal::p()->Width * 1.0f, "GENERATING MAP");
		normalText->SetFormat("Example/Assets/DejaVuSansMono.ttf", 32, sf::Color::White, VText::ALIGNCENTER);
		Add(normalText);

        future = std::async(std::launch::async, std::bind(&AsyncTestState::LoadMap, this));
		VGlobal::p()->Async->LaunchAsyncFunction(future);
	}

	void LoadMap()
	{
		int height = VGlobal::p()->Height;
		int width = VGlobal::p()->Width;

		pixel->Vertices.setPrimitiveType(sf::Points);
		pixel->Vertices.resize(width * height);
		pixel->Size = sf::Vector2f(sf::Vector2i(width, height));
		std::vector<float> map = PerlinNoise::GenerateFloat(width, height, 3.0f, 7, 0.5f, 42);

		int pixelCount = 0;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int index = (y * width) + x;
				float value = map[index];

				pixel->Vertices[pixelCount].position = sf::Vector2f(x + 0.5f, y + 0.5f);
				pixel->Vertices[pixelCount].color = sf::Color(
					sf::Uint8(255 * value),
					sf::Uint8(255 * value),
					sf::Uint8(255 * value),
					255);
				pixelCount++;

				normalText->SetText("LOADING MAP " + std::to_string(pixelCount * 100 / (width * height)) + "%");
			}
		}

		map.clear();
		map.shrink_to_fit();

		VGlobal::p()->Async->SyncToMainLoop(std::bind(&AsyncTestState::OnLoadedMap, this));
	}

	void OnLoadedMap()
	{
		normalText->SetFillTint(sf::Color::Black);
		normalText->SetText("LOAD COMPLETE");

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

#ifndef VFRAME_NO_PHYSICS
class PhysicsState : public VSubState
{
	VPhysicsGroup* group;

public:
	typedef VSubState VSUPERCLASS;
	PhysicsState() : VSubState() {}

	virtual void Initialise()
	{
		VSUPERCLASS::Initialise();

		VShape* box = new VShape(VGlobal::p()->Width - 100.0f - 30.0f, 200.0f);
		box->SetRectangle(30.0f, 30.0f);
		box->SetFillTint(sf::Color::Yellow);
		box->Elasticity = 0.5f;
		box->Angle = 45.0f;

		VShape* box2 = new VShape(100.0f, 200.0f);
		box2->SetRectangle(30.0f, 30.0f);
		box2->SetFillTint(sf::Color::Yellow);
		box2->Elasticity = 0.5f;
		box2->Angle = 45.0f;

		VShape* slope = new VShape(150.0f, 200.0f);
		slope->SetRectangle(120.0f, 1.0f);
		slope->Origin = sf::Vector2f(0.0f, 0.5f);
		slope->SetFillTint(sf::Color::White);
		slope->Elasticity = 1.0f;
		slope->Angle = 20.0f;

		VShape* slope1 = new VShape(150.0f + cosf(slope->Angle * (3.1415926f / 180.0f)) * 120.0f, 200.0f + sinf(slope->Angle * (3.1415926f / 180.0f)) * 120.0f);
		slope1->SetRectangle(120.0f, 1.0f);
		slope1->Origin = sf::Vector2f(0.0f, 0.5f);
		slope1->SetFillTint(sf::Color::White);
		slope1->Elasticity = 1.0f;
		slope1->Angle = 0.0f;

		VShape* slope2 = new VShape(270.0f + cosf(slope->Angle * (3.1415926f / 180.0f)) * 120.0f, 200.0f + sinf(slope->Angle * (3.1415926f / 180.0f)) * 120.0f);
		slope2->SetRectangle(120.0f, 1.0f);
		slope2->Origin = sf::Vector2f(0.0f, 0.5f);
		slope2->SetFillTint(sf::Color::White);
		slope2->Elasticity = 1.0f;
		slope2->Angle = -20.0f;

		VShape* ground = new VShape(0.0f, VGlobal::p()->Height - 30.0f);
		ground->SetRectangle((float)VGlobal::p()->Width, 30.0f);
		ground->SetFillTint(sf::Color::Cyan);
		ground->Elasticity = 0.5f;

		Add(ground);

		group = new VPhysicsGroup();
		group->SetGravity(sf::Vector2f(0.0f, 200.0f));

		VGlobal::p()->Sound->Load("Example/Assets/bounce.ogg", "bounce");
		for (int i = 0; i < 50; i++)
		{
			VShape* circle = new VShape(VGlobal::p()->Random->GetFloat(VGlobal::p()->Width - 100.0f, 100.0f), VGlobal::p()->Random->GetFloat(100.0f));
			circle->SetCircle(10.0f);
			circle->Elasticity = 0.8f;
			circle->SetFillTint(VColour::HSVtoRGB(VGlobal::p()->Random->GetFloat(360.0f), 1.0f, 0.8f));
			Add(circle);

			VPhysicsObject* ball = group->AddObject(circle, VPhysicsObject::DYNAMIC, VPhysicsObject::CIRCLE);
			ball->SetFriction(0.7f);
			ball->SetDensity(0.5f);

			group->SetCollisionCallback(circle, slope, std::bind(&PhysicsState::BounceCallback, this, std::placeholders::_1, std::placeholders::_2), VPhysicsGroup::SEPARATE, true);
			group->SetCollisionCallback(circle, slope1, std::bind(&PhysicsState::BounceCallback, this, std::placeholders::_1, std::placeholders::_2), VPhysicsGroup::SEPARATE, true);
			group->SetCollisionCallback(circle, slope2, std::bind(&PhysicsState::BounceCallback, this, std::placeholders::_1, std::placeholders::_2), VPhysicsGroup::SEPARATE, true);
		}

		float width = 10.0f;
		float height = 15.0f;
		float spacing = width * 0.3f;
		for (int i = 0; i < 8; i++)
		{
			VPhysicsObject* prev = NULL;

			if (i > 1 && i < 6)
				continue;

			for (int j = 0; j < 10; j++)
			{
				sf::Vector2f pos((i+1) * (VGlobal::p()->Width / 9.0f), (j + 0.5f) * height - (j + 1)*spacing);
				VShape* box = new VShape(pos.x - (width/2), pos.y - (height), width, height);
				box->SetFillTint(VColour::HSVtoRGB(VGlobal::p()->Random->GetFloat(360.0f), 1.0f, 0.6f));
				Add(box);

				VPhysicsObject* chain = group->AddObject(box, VPhysicsObject::DYNAMIC);
				chain->SetFriction(0.8f);

				VPhysicsSlideJoint* joint = nullptr;
				if (j == 0)
					joint = new VPhysicsSlideJoint(chain->GetBody(), group->GetBody(), sf::Vector2f(0.0f, height / 2.0f), sf::Vector2f(pos.x, 0.0f), 0.0f, spacing);
				else
					joint = new VPhysicsSlideJoint(chain, prev, sf::Vector2f(0.0f, height / 2.0f), sf::Vector2f(0.0f, -height / 2.0f), 0.0f, spacing);

				joint->SetCollideBodies(false);
				group->AddJoint(joint);

				prev = chain;
			}
		}

		group->AddObject(ground, VPhysicsObject::STATIC)->SetFriction(1.0f);
		group->AddObject(new VObject(-10.0f, 0.0f, 10.0f, (float)VGlobal::p()->Height), VPhysicsObject::STATIC);
		group->AddObject(new VObject((float)VGlobal::p()->Width, 0.0f, 10.0f, (float)VGlobal::p()->Height), VPhysicsObject::STATIC);

		VPhysicsObject* boxPhysics = group->AddObject(box, VPhysicsObject::DYNAMIC);
		boxPhysics->SetFriction(0.5f);
		group->AddJoint(new VPhysicsPivotJoint(boxPhysics->GetBody(), group->GetBody(), box->Position + (box->Size / 2.0f)));

		boxPhysics = group->AddObject(box2, VPhysicsObject::DYNAMIC);
		boxPhysics->SetFriction(0.5f);
		group->AddJoint(new VPhysicsPivotJoint(boxPhysics->GetBody(), group->GetBody(), box2->Position + (box2->Size / 2.0f)));

		VPhysicsObject* slopePhysics = group->AddObject(slope, VPhysicsObject::KINEMATIC, VPhysicsObject::LINE, { sf::Vector2f(0.0f, 0.0f), sf::Vector2f(cosf(slope->Angle * (3.1415926f / 180.0f)) * 120.0f, sinf(slope->Angle * (3.1415926f / 180.0f)) * 120.0f) });
		slopePhysics->SetFriction(1.0f);
		slopePhysics = group->AddObject(slope1, VPhysicsObject::KINEMATIC, VPhysicsObject::LINE, { sf::Vector2f(0.0f, 0.0f), sf::Vector2f(cosf(slope1->Angle * (3.1415926f / 180.0f)) * 120.0f, sinf(slope1->Angle * (3.1415926f / 180.0f)) * 120.0f) });
		slopePhysics->SetFriction(1.0f);
		slopePhysics = group->AddObject(slope2, VPhysicsObject::KINEMATIC, VPhysicsObject::LINE, { sf::Vector2f(0.0f, 0.0f), sf::Vector2f(cosf(slope2->Angle * (3.1415926f / 180.0f)) * 120.0f, sinf(slope2->Angle * (3.1415926f / 180.0f)) * 120.0f) });
		slopePhysics->SetFriction(1.0f);

		Add(box);
		Add(box2);
		Add(slope);
		Add(slope1);
		Add(slope2);
		Add(group);
	}

	virtual void HandleEvents(const sf::Event& event)
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos = VGlobal::p()->GetMousePosition();

				float MaxDist = 100.0f;
				float Impulse = 10.0f;
				for (int i = 0; i < group->Length(); i++)
				{
					VPhysicsObject* ball = dynamic_cast<VPhysicsObject*>(group->GetGroupItem(i));

					if (ball && ball->GetShapeType() == VPhysicsObject::CIRCLE)
					{
						sf::Vector2f ballPos = ball->GetBaseObject()->Position + (ball->GetBaseObject()->Size / 2.0f);
						sf::Vector2f diff = mousePos - ballPos; 
						float length = sqrtf((diff.x * diff.x) + (diff.y * diff.y));
						if (length > MaxDist)
						{
							ballPos = sf::Vector2f();
						}
						else
						{
							diff = (diff / length) * (MaxDist - length) * Impulse;
							ball->ApplyImpulseAtWorldPoint(-diff, mousePos);
						}
					}
				}
			}
		}
	}

	bool BounceCallback(VPhysicsObject* circle, VPhysicsObject* other)
	{
		float speed = sqrtf((circle->GetBaseObject()->Velocity.x * circle->GetBaseObject()->Velocity.x) + (circle->GetBaseObject()->Velocity.y * circle->GetBaseObject()->Velocity.y));
		if (speed > 1000)
			speed = 1000;

		speed /= 10.0f;
		VGlobal::p()->Sound->Play("bounce", speed);

		return true;
	}
};
#endif

class DemoStatesManager : public VState
{
public:
	typedef VState VSUPERCLASS;

	VGroup* stateText = nullptr;

	int CurrentPage = 0;
	void SetNewPage();

	DemoStatesManager() : VState() {}
	virtual void Initialise() override;
	virtual void HandleEvents(const sf::Event& event) override;
};
