#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#ifdef USE_GAMEPAD_API
#define GAMEPAD_STATIC_LIB
#include "gamepad.h"
#else
#include "XInputDevice.h"
#endif

#include <map>
#include <iostream>

class VInputHandler
{
#ifdef USE_GAMEPAD_API
public:
	enum XAxis
	{
		PovX,
		PovY,
		Z,
		V,
		L,
		R
	};
#endif

protected:
	struct ButtonInput
	{
		sf::Keyboard::Key key;
#ifdef USE_GAMEPAD_API
		GAMEPAD_BUTTON gamepad;
#else
		sf::XInputDevice::XButton gamepad;
#endif
		sf::Mouse::Button mouse;

		bool pressed;
		bool down;
		bool released;
	};

	struct AxisInput
	{
		sf::Keyboard::Key keyA;
		sf::Keyboard::Key keyB;
#ifdef USE_GAMEPAD_API
		XAxis gamepad;
#else
		sf::XInputDevice::XAxis gamepad;
#endif

		float lastValue;
		float value;
	};

	std::map<sf::String, ButtonInput> buttonInputs;
	std::map<sf::String, AxisInput> axisInputs;

	bool isGamepadActive = false;
	float scrollWheel = 0;
	sf::Vector2i lastMousePos = sf::Vector2i();

public:

#ifdef USE_GAMEPAD_API
	VInputHandler()
	{
		GamepadInit();
	}
#else
	VInputHandler() = default;
#endif

	virtual ~VInputHandler();

	int ControllerNo = 0;

	void AddButtonInput(sf::String mame, int key = -1, int gamepad = -1, int mouse = -1);
	void AddAxisInput(sf::String name, int keyA = -1, int keyB = -1, int gamepad = -1);

	bool IsGamepadActive();

	bool IsButtonPressed(sf::String name);
	bool IsButtonDown(sf::String name);
	bool IsButtonUp(sf::String name);
	bool IsButtonReleased(sf::String name);

	float CurrentAxisValue(sf::String name);
	float LastAxisValue(sf::String name);

	float ScrollWheelDelta();
	void ResetScrollWheel();

	void HandleEvents(const sf::Event& event);
	void Update(float dt);
};

