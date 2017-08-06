#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <stdint.h>

#ifdef USE_GAMEPAD_API
#define GAMEPAD_STATIC_LIB
#include "depend/gamepad.h"
#elif defined(USE_SFML_JOYSTICK)
#include <SFML/Window/Joystick.hpp>
#else
#include "XInputDevice.h"
#endif

#include <map>
#include <iostream>

#define CONTROLLER_COUNT 4

class VInputHandler
{
public:
#ifdef USE_GAMEPAD_API
	enum XAxis : unsigned char
	{
		PovX,
		PovY,
		Z,
		V,
		L,
		R
	};
#elif defined(USE_SFML_JOYSTICK)
	enum Button : unsigned char
	{
		BUTTON_A,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_LEFT_SHOULDER,
		BUTTON_RIGHT_SHOULDER,
		BUTTON_BACK,
		BUTTON_START,
		BUTTON_LEFT_THUMB,
		BUTTON_RIGHT_THUMB,
		BUTTON_HOME,
		BUTTON_DPAD_UP,
		BUTTON_DPAD_DOWN,
		BUTTON_DPAD_LEFT,
		BUTTON_DPAD_RIGHT,
		BUTTON_COUNT,
	};
#endif

protected:
	struct ButtonInput
	{
		sf::Keyboard::Key key;
#ifdef USE_GAMEPAD_API
		GAMEPAD_BUTTON gamepad;
#elif defined(USE_SFML_JOYSTICK)
		int gamepad;
#else
		sf::XInputDevice::XButton gamepad;
#endif
		sf::Mouse::Button mouse;

		bool pressed[CONTROLLER_COUNT];
		bool down[CONTROLLER_COUNT];
		bool released[CONTROLLER_COUNT];
	};

	struct AxisInput
	{
		sf::Keyboard::Key keyA;
		sf::Keyboard::Key keyB;
#ifdef USE_GAMEPAD_API
		XAxis gamepad;
#elif defined(USE_SFML_JOYSTICK)
		sf::Joystick::Axis gamepad;
#else
		sf::XInputDevice::XAxis gamepad;
#endif

		float lastValue[CONTROLLER_COUNT];
		float value[CONTROLLER_COUNT];
	};

	std::map<sf::String, ButtonInput> buttonInputs;
	std::map<sf::String, AxisInput> axisInputs;

	bool isGamepadActive = false;
	float scrollWheel = 0;
	sf::Vector2i lastMousePos = sf::Vector2i();
	
#ifdef USE_SFML_JOYSTICK
	int JoystickID[CONTROLLER_COUNT];
#endif

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

	void SetButtonInput(const sf::String& name, int key = -1, int gamepad = -1, int mouse = -1);
	void SetAxisInput(const sf::String& name, int keyA = -1, int keyB = -1, int gamepad = -1);

	bool GetButtonInput(const sf::String& name, ButtonInput& input);
	bool GetAxisInput(const sf::String& name, AxisInput& input);

	bool IsGamepadActive();

#ifdef USE_SFML_JOYSTICK
	int GetJoystickID(int ControllerIndex);
#endif

	bool IsButtonPressed(const sf::String& name, int ControllerIndex = 0);
	bool IsButtonDown(const sf::String& name, int ControllerIndex = 0);
	bool IsButtonUp(const sf::String& name, int ControllerIndex = 0);
	bool IsButtonReleased(const sf::String& name, int ControllerIndex = 0);

	float CurrentAxisValue(const sf::String& name, int ControllerIndex = 0);
	float LastAxisValue(const sf::String& name, int ControllerIndex = 0);

	float ScrollWheelDelta();
	void ResetScrollWheel();

	void HandleEvents(const sf::Event& event);
	void Update(float dt);
};

