#include "VInputHandler.h"

VInputHandler::~VInputHandler()
{
	buttonInputs.clear();
	axisInputs.clear();

#ifdef USE_GAMEPAD_API
	GamepadShutdown();
#endif
}

void VInputHandler::AddButtonInput(sf::String name, int key, int gamepad, int mouse)
{
	ButtonInput button;
	button.key = (sf::Keyboard::Key)key;

#ifdef USE_GAMEPAD_API
	button.gamepad = (GAMEPAD_BUTTON)(gamepad);
#elif defined(USE_SFML_JOYSTICK)
	button.gamepad = (sf::Joystick::Axis)gamepad;
#else
	if (gamepad > 0)
		button.gamepad = (sf::XInputDevice::XButton)((unsigned short)gamepad);
	else
		button.gamepad = (sf::XInputDevice::XButton)0;
#endif

	button.mouse = (sf::Mouse::Button)mouse;

	button.down = false;
	button.pressed = false;
	button.released = false;

	buttonInputs[name] = button;
}

void VInputHandler::AddAxisInput(sf::String name, int keyA, int keyB, int gamepad)
{
	AxisInput axis;
	axis.keyA = (sf::Keyboard::Key)keyA;
	axis.keyB = (sf::Keyboard::Key)keyB;

#ifdef USE_GAMEPAD_API
	axis.gamepad = (XAxis)gamepad;
#elif defined(USE_SFML_JOYSTICK)
	axis.gamepad = (sf::Joystick::Axis)gamepad;
#else
	axis.gamepad = (sf::XInputDevice::XAxis)gamepad;
#endif

	axis.lastValue = 0;
	axis.value = 0;

	axisInputs[name] = axis;
}

bool VInputHandler::IsGamepadActive()
{
	return isGamepadActive;
}

bool VInputHandler::IsButtonPressed(sf::String name)
{
	if (buttonInputs.find(name) != buttonInputs.end())
	{
		return buttonInputs[name].pressed;
	}

	return false;
}

bool VInputHandler::IsButtonDown(sf::String name)
{
	if (buttonInputs.find(name) != buttonInputs.end())
	{
		return buttonInputs[name].down;
	}

	return false;
}

bool VInputHandler::IsButtonUp(sf::String name)
{
	if (buttonInputs.find(name) != buttonInputs.end())
	{
		return IsButtonDown(name);
	}

	return false;
}

bool VInputHandler::IsButtonReleased(sf::String name)
{
	if (buttonInputs.find(name) != buttonInputs.end())
	{
		return buttonInputs[name].released;
	}

	return false;
}

float VInputHandler::CurrentAxisValue(sf::String name)
{
	if (axisInputs.find(name) != axisInputs.end())
	{
		return axisInputs[name].value;
	}

	return 0;
}

float VInputHandler::LastAxisValue(sf::String name)
{
	if (axisInputs.find(name) != axisInputs.end())
	{
		return axisInputs[name].lastValue;
	}

	return 0;
}

float VInputHandler::ScrollWheelDelta()
{
	return scrollWheel;
}

void VInputHandler::ResetScrollWheel()
{
	scrollWheel = 0;
}

void VInputHandler::HandleEvents(const sf::Event& event)
{
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		scrollWheel = event.mouseWheelScroll.delta;
	}
}

void VInputHandler::Update(float dt)
{
#ifdef USE_GAMEPAD_API
	GamepadUpdate();
#elif defined(USE_SFML_JOYSTICK)
	int count = 0;
	for (int i = 0; i < sf::Joystick::Count; i++)
	{
		JoystickID[i] = -1;
		if (sf::Joystick::isConnected(i))
		{
			JoystickID[count++] = i;
		}
	}
#endif

	for (std::map<sf::String, ButtonInput>::iterator button = buttonInputs.begin(); button != buttonInputs.end(); ++button)
	{
		ButtonInput& b = button->second;

		b.pressed = false;
		b.released = false;

#ifdef USE_GAMEPAD_API
		if (sf::Keyboard::isKeyPressed(b.key) || GamepadButtonTriggered((GAMEPAD_DEVICE)ControllerNo, b.gamepad) || sf::Mouse::isButtonPressed(b.mouse))
#elif defined(USE_SFML_JOYSTICK)
		if (sf::Keyboard::isKeyPressed(b.key) || sf::Joystick::isButtonPressed(JoystickID[ControllerNo], b.gamepad) || sf::Mouse::isButtonPressed(b.mouse))
#else
		if (sf::Keyboard::isKeyPressed(b.key) || sf::XInputDevice::isButtonPressed(ControllerNo, b.gamepad) || sf::Mouse::isButtonPressed(b.mouse))
#endif
		{
			if (!b.down)
				b.pressed = true;
			b.down = true;

			isGamepadActive = !(sf::Keyboard::isKeyPressed(b.key) || sf::Mouse::isButtonPressed(b.mouse));
		}
		else
		{
			if (b.down)
				b.released = true;
			b.down = false;
		}
	}

	for (std::map<sf::String, AxisInput>::iterator axis = axisInputs.begin(); axis != axisInputs.end(); ++axis)
	{
		AxisInput& a = axis->second;

		a.lastValue = a.value;
		a.value = 0;

		if (a.keyA >= 0)
		{
			if (sf::Keyboard::isKeyPressed(a.keyA))
			{
				a.value = 100.0f;
				isGamepadActive = false;
			}
		}
		
		if (a.keyB >= 0)
		{
			if (sf::Keyboard::isKeyPressed(a.keyB))
			{
				a.value = -100.0f;
				isGamepadActive = false;
			}
		}

		if (a.gamepad >= 0)
		{
#ifdef USE_GAMEPAD_API
			float val1 = 0, val2 = 0;

			switch (a.gamepad)
			{
				case PovX:
				{
					GamepadStickNormXY((GAMEPAD_DEVICE)ControllerNo, STICK_LEFT, &val1, &val2);
					break;
				}
				case PovY:
				{
					GamepadStickNormXY((GAMEPAD_DEVICE)ControllerNo, STICK_LEFT, &val2, &val1);
					val1 *= -1;
					break;
				}
				case Z:
				{
					GamepadStickNormXY((GAMEPAD_DEVICE)ControllerNo, STICK_RIGHT, &val1, &val2);
					break;
				}
				case V:
				{
					GamepadStickNormXY((GAMEPAD_DEVICE)ControllerNo, STICK_RIGHT, &val2, &val1);
					val1 *= -1;
					break;
				}
				case L:
				{
					val1 = GamepadTriggerValue((GAMEPAD_DEVICE)ControllerNo, TRIGGER_LEFT) / 32767.0f;
					break;
				}
				case R:
				{
					val1 = GamepadTriggerValue((GAMEPAD_DEVICE)ControllerNo, TRIGGER_RIGHT) / 32767.0f;
					break;
				}
			}

			val1 *= 100.0f;

			if ((int)val1 != 0)
			{
				a.value = val1;
				isGamepadActive = true;
			}
#elif defined(USE_SFML_JOYSTICK)
			if (abs(sf::Joystick::getAxisPosition(ControllerNo, a.gamepad)) > 20.0f) //Deadzone
			{
				a.value = sf::Joystick::getAxisPosition(JoystickID[ControllerNo], a.gamepad);
				isGamepadActive = true;
			}
			else
			{
				a.value = 0;
			}
#else
			if (sf::XInputDevice::getAxisPosition(ControllerNo, a.gamepad) != 0)
			{
				a.value = sf::XInputDevice::getAxisPosition(ControllerNo, a.gamepad);
				isGamepadActive = true;
			}
#endif
		}
	}

	sf::Vector2i mousePosition = sf::Mouse::getPosition();

	if (mousePosition - lastMousePos != sf::Vector2i())
	{
		isGamepadActive = false;
	}

	lastMousePos = mousePosition;
}
