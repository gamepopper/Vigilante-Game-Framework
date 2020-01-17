/**
* @file    VInputHandler.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2018 Tim Stoddard
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* @section DESCRIPTION
*
* Input handling system for keyboard, mice and controllers.
*/

#pragma once
#include <SFML/Window.hpp>
#include <stdint.h>

///Use Sean Middleditch's Multiplatform XInput Library.
#ifdef USE_GAMEPAD_API
#define GAMEPAD_STATIC_LIB
#include "depend/gamepad.h"
///Use SFML's Joystick class.
#elif defined(USE_SFML_JOYSTICK)
#include <SFML/Window/Joystick.hpp>
#else
///Use XInput Device objects.
#include "XInputDevice.h"
#endif

#include <map>
#include <iostream>

///Maximum amount of controllers.
#define CONTROLLER_COUNT 4

///Processes inputs from keyboard, mouse and gamepad controllers.
class VInputHandler
{
public:
#ifdef USE_GAMEPAD_API
	///Gamepad Axis Properties
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
	///Button Types
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
	///Button Input Data
	struct ButtonInput
	{
		///Keyboard Key Code
		sf::Keyboard::Key key; 
#if	defined(USE_GAMEPAD_API)
		///Gamepad Button
		GAMEPAD_BUTTON gamepad;
#elif defined(USE_SFML_JOYSTICK)
		///Gamepad Button
		int gamepad;
#else
		///Gamepad Button
		sf::XInputDevice::XButton gamepad;
#endif
		///Mouse Button
		sf::Mouse::Button mouse;

		///True if pressed at frame.
		bool pressed[CONTROLLER_COUNT];
		///True if down.
		bool down[CONTROLLER_COUNT];
		///True if released at frame.
		bool released[CONTROLLER_COUNT];
	};

	///Axis Input Data
	struct AxisInput
	{
		///Key Code for Negative Axis Input
		sf::Keyboard::Key keyA;
		///Key Code for Positive Axis Input
		sf::Keyboard::Key keyB;
#ifdef USE_GAMEPAD_API
		///Gamepad Axis
		XAxis gamepad;
#elif defined(USE_SFML_JOYSTICK)
		///Gamepad Axis
		sf::Joystick::Axis gamepad;
#else
		///Gamepad Axis
		sf::XInputDevice::XAxis gamepad;
#endif
		///Previous frame's axis value.
		float lastValue[CONTROLLER_COUNT];
		///Current frame's axis value.
		float value[CONTROLLER_COUNT];
	};

	///Set of button inputs.
	std::map<sf::String, ButtonInput> buttonInputs;
	///Set of axis inputs.
	std::map<sf::String, AxisInput> axisInputs;

	///Defines if gamepads are currently active.
	bool isGamepadActive = false;
	///Current scroll wheel amount.
	float scrollWheel = 0;
	///Previous mouse position, useful for testing mouse movement.
	sf::Vector2i lastMousePos = sf::Vector2i();
	///Reference to window for mouse inputs.
	sf::Window* window = nullptr;
	
#ifdef USE_SFML_JOYSTICK
	///SFML requires you to keep record of the joystick ID for each player.
	int JoystickID[CONTROLLER_COUNT];
#endif

public:

	VInputHandler(sf::Window* window);
	virtual ~VInputHandler();

	/**
	* Set the input values for a game button.
	* @param name Name of input type.
	* @param key Key Code for Input.
	* @param gamepad Gamepad value for Input.
	* @param mouse Mouse value for Input.
	*/
	void SetButtonInput(const sf::String& name, int key = -1, int gamepad = -1, int mouse = -1);

	/**
	* Sets the input values for a game axis. (i.e. analog sticks)
	* @param name Name of input type.
	* @param keyA Key Code for the negative axis input.
	* @param keyB Key Code for the positive axis input.
	* @param gamepad Gamepad value for axis Input.
	*/
	void SetAxisInput(const sf::String& name, int keyA = -1, int keyB = -1, int gamepad = -1);

	/**
	* Gets specific Button input data.
	* @param name Name of input.
	* @param input Reference to Button Input.
	* @return True if reference has been found and set to the input parameter.
	*/
	bool GetButtonInput(const sf::String& name, ButtonInput& input);

	/**
	* Gets specific Axis input data.
	* @param name Name of input.
	* @param input Reference to Axis Input.
	* @return True if reference has been found and set to the input parameter.
	*/
	bool GetAxisInput(const sf::String& name, AxisInput& input);

	///@return True if any gamepad input is recieved, returns false if other input is recieved.
	bool IsGamepadActive();

#ifdef USE_SFML_JOYSTICK
	/**
	* Gets JoystickID when using sf::Joystick.
	*/
	int GetJoystickID(int ControllerIndex);
#endif

	/**
	* Checks if a button has applied a press for a single frame.
	* @param name Name of Input.
	* @param ControllerIndex Player Index for specific controller. (e.g. 0 = Player 1, 1 = Player 2 ect)
	* @return True if button has been pressed.
	*/
	bool IsButtonPressed(const sf::String& name, int ControllerIndex = 0);

	/**
	* Checks if a button is pushed down.
	* @param name Name of Input.
	* @param ControllerIndex Player Index for specific controller. (e.g. 0 = Player 1, 1 = Player 2 ect)
	* @return True if button is down.
	*/
	bool IsButtonDown(const sf::String& name, int ControllerIndex = 0);

	/**
	* Checks if a button is not pushed down.
	* @param name Name of Input.
	* @param ControllerIndex Player Index for specific controller. (e.g. 0 = Player 1, 1 = Player 2 ect)
	* @return True if button is up.
	*/
	bool IsButtonUp(const sf::String& name, int ControllerIndex = 0);

	/**
	* Checks if a button has applied a release for a single frame.
	* @param name Name of Input.
	* @param ControllerIndex Player Index for specific controller. (e.g. 0 = Player 1, 1 = Player 2 ect)
	* @return True if button has been released.
	*/
	bool IsButtonReleased(const sf::String& name, int ControllerIndex = 0);

	/**
	* Gets the Axis value of the current frame.
	* @param name Name of Input.
	* @param ControllerIndex Player Index for specific controller. (e.g. 0 = Player 1, 1 = Player 2 ect)
	* @return Axis value with range [-100 - 100], 0 if controller is not index.
	*/
	float CurrentAxisValue(const sf::String& name, int ControllerIndex = 0);

	/**
	* Gets the Axis value of the last frame.
	* @param name Name of Input.
	* @param ControllerIndex Player Index for specific controller. (e.g. 0 = Player 1, 1 = Player 2 ect)
	* @return Axis value with range [-100 - 100], 0 if controller is not index.
	*/
	float LastAxisValue(const sf::String& name, int ControllerIndex = 0);

	///@return The scroll wheel delta from the mouse.
	float ScrollWheelDelta();
	///Resets the scroll wheel delta to 0.
	void ResetScrollWheel();

	///@param event The current event that can be processed.
	void HandleEvents(const sf::Event& event);

	/**
	* Updates the inputs.
	* @param dt Delta Time between the current and previous frames of the game.
	*/
	void Update(float dt);
};

