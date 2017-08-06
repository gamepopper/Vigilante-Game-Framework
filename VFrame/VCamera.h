#pragma once
#include "VObject.h"
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <functional>

enum CameraFollowMethod : unsigned char
{
	LOCKON,
	TOPDOWN,
	PLATFORMER
};

class VCamera
{
protected:
	sf::View view;
	sf::RectangleShape block;

	bool isFollowing = false;
	VObject* followObject = nullptr;
	CameraFollowMethod followMethod = LOCKON;
	float followLead = 0;
	float followLerp = 0;
	float deadZone = 0.5f;

	bool isShaking = false;
	float shakeX = 0;
	float shakeY = 0;
	float shakeLength = 0;
	sf::Clock shakeTimer;
	std::function<void()> OnShakeComplete = nullptr;

	sf::Clock fadeFlashTimer;

	bool isFading = false;
	bool fadeIn = false;
	float fadeTime = 0.0f;
	sf::Color fadeColour = sf::Color::Black;
	std::function<void()> OnFadeComplete = nullptr;

	bool isFlashing = false;
	std::function<void()> OnFlashComplete = nullptr;

	bool boundToWorld = true;

	sf::Vector2f LastPosition;

public:
	sf::Vector2f Position;
	float Rotation = 0.0f;
	float Zoom = 1.0f;

	VCamera()
	{
		Reset();
	}
	VCamera(sf::View& View) : view(View)
	{
		
	}
	virtual ~VCamera()
	{
		followObject = nullptr;
		OnShakeComplete = nullptr;
		OnFadeComplete = nullptr;
		OnFlashComplete = nullptr;
	}

	//Follow a game object.
	void Follow(VObject* target, float deadzone = 0.5f, CameraFollowMethod followmethod = LOCKON, float lead = 0.0f, float lerp = 0.0f);

	//Stop following a game object.
	void StopFollow();

	//Shake for a specific amount of time. Use OnComplete to call a function after shaking has completed.
	bool Shake(float intensity, float time = 1.0f, std::function<void()> OnComplete = nullptr);
	bool Shake(sf::Vector2f intensity, float time = 1.0f, std::function<void()> OnComplete = nullptr);

	//Make Camera Fade in or out. Use OnComplete to call a function after shaking has completed.
	bool Fade(sf::Color colour = sf::Color::Black, float time = 1.0f, bool fadein = false, std::function<void()> OnComplete = nullptr);
	//Make Camera Flash a certain colour. Use OnComplete to call a function after shaking has completed.
	bool Flash(sf::Color colour = sf::Color::Black, float time = 1.0f, std::function<void()> OnComplete = nullptr);

	//Sets whether camera should bound to the FlxG::p()->WorldBounds
	void SetToBounds(bool bound)
	{
		boundToWorld = bound;
	}

	//Sets camera to display from a specific section of the viewport (default is FloatRect(0, 0, 1, 1)).
	void SetViewport(sf::FloatRect &viewport)
	{
		view.setViewport(viewport);
	}

	//Tests if a game object is in view.
	bool IsObjectInView(VObject* object);

	void Update(float ft);
	void Render(sf::RenderTarget &RenderTarget);
	void Reset();

	bool IsFading()		{ return isFading;		}
	bool IsFlashing()	{ return isFlashing;	}
	bool IsShaking()	{ return isShaking;		}

	sf::View const& GetView()
	{
		return view;
	}
};

class VCameraList
{
public:
	//Default view, do not modify as this can be used to reset the Camera list to the default single camera.
	static sf::View Default;
};

