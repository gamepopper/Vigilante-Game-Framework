#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "XInputDevice.h"

#include <map>
#include <iostream>

class VInputHandler
{
protected:
	struct ButtonInput
	{
		sf::Keyboard::Key key;
		sf::XInputDevice::XButton gamepad;
		sf::Mouse::Button mouse;

		bool pressed;
		bool down;
		bool released;
	};

	struct AxisInput
	{
		sf::Keyboard::Key keyA;
		sf::Keyboard::Key keyB;
		sf::XInputDevice::XAxis gamepad;

		float lastValue;
		float value;
	};

	std::map<sf::String, ButtonInput> buttonInputs;
	std::map<sf::String, AxisInput> axisInputs;

	bool isGamepadActive = false;

	sf::Vector2i lastMousePos = sf::Vector2i();

public:
	VInputHandler() = default;
	virtual ~VInputHandler();

	int ControllerNo = 0;

	void AddButtonInput(sf::String mame, int key = -1, short gamepad = -1, int mouse = -1);
	void AddAxisInput(sf::String name, int keyA = -1, int keyB = -1, int gamepad = -1);

	bool IsGamepadActive();

	bool IsButtonPressed(sf::String name);
	bool IsButtonDown(sf::String name);
	bool IsButtonUp(sf::String name);
	bool IsButtonReleased(sf::String name);

	float CurrentAxisValue(sf::String name);
	float LastAxisValue(sf::String name);

	void Update(float dt);
};

