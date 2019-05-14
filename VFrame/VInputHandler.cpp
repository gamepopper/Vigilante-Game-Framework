#include "VInputHandler.h"
#include <cstring>

VInputHandler::~VInputHandler()
{
	buttonInputs.clear();
	axisInputs.clear();

#ifdef USE_GAMEPAD_API
	GamepadShutdown();
#endif
}

void VInputHandler::SetButtonInput(const sf::String& name, int key, int gamepad, int mouse)
{
	if (buttonInputs.find(name) != buttonInputs.end())
	{
		if (buttonInputs[name].key == key &&
			buttonInputs[name].gamepad == gamepad &&
			buttonInputs[name].mouse == mouse)
		{
			return;
		}
	}

	ButtonInput button;
	button.key = (sf::Keyboard::Key)key;

#ifdef USE_GAMEPAD_API
	button.gamepad = (GAMEPAD_BUTTON)(gamepad);
#elif defined(USE_SFML_JOYSTICK)
	button.gamepad = (sf::Joystick::Axis)gamepad;
	sf::Joystick::update();
#else
	if (gamepad > 0)
		button.gamepad = (sf::XInputDevice::XButton)((unsigned short)gamepad);
	else
		button.gamepad = (sf::XInputDevice::XButton)0;
#endif

	button.mouse = (sf::Mouse::Button)mouse;

	memset(button.down, NULL, sizeof(button.down));
	memset(button.pressed, NULL, sizeof(button.pressed));
	memset(button.released, NULL, sizeof(button.released));

	buttonInputs[name] = button;
}

void VInputHandler::SetAxisInput(const sf::String& name, int keyA, int keyB, int gamepad)
{
	if (axisInputs.find(name) != axisInputs.end())
	{
		if (axisInputs[name].keyA == keyA &&
			axisInputs[name].gamepad == gamepad &&
			axisInputs[name].keyB == keyB)
		{
			return;
		}
	}

	AxisInput axis;
	axis.keyA = (sf::Keyboard::Key)keyA;
	axis.keyB = (sf::Keyboard::Key)keyB;

#ifdef USE_GAMEPAD_API
	axis.gamepad = (XAxis)gamepad;
#elif defined(USE_SFML_JOYSTICK)
	axis.gamepad = (sf::Joystick::Axis)gamepad;
	sf::Joystick::update();
#else
	axis.gamepad = (sf::XInputDevice::XAxis)gamepad;
#endif

	memset(axis.lastValue, NULL, sizeof(axis.lastValue));
	memset(axis.value, NULL, sizeof(axis.value));

	axisInputs[name] = axis;
}

bool VInputHandler::GetButtonInput(const sf::String& name, ButtonInput& input)
{
	if (buttonInputs.find(name) != buttonInputs.end())
	{
		input = buttonInputs[name];
		return true;
	}

	return false;
}

bool VInputHandler::GetAxisInput(const sf::String& name, AxisInput& input)
{
	if (axisInputs.find(name) != axisInputs.end())
	{
		input = axisInputs[name];
		return true;
	}

	return false;
}

bool VInputHandler::IsGamepadActive()
{
	return isGamepadActive;
}

#ifdef USE_SFML_JOYSTICK
int VInputHandler::GetJoystickID(int ControllerIndex)
{
	if (ControllerIndex < 0 || ControllerIndex >= CONTROLLER_COUNT)
		return -1;

	return JoystickID[ControllerIndex];
}
#endif

bool VInputHandler::IsButtonPressed(const sf::String& name, int controller)
{
	if (buttonInputs.find(name) != buttonInputs.end())
	{
		return buttonInputs[name].pressed[controller];
	}

	return false;
}

bool VInputHandler::IsButtonDown(const sf::String& name, int controller)
{
	if (buttonInputs.find(name) != buttonInputs.end())
	{
		return buttonInputs[name].down[controller];
	}

	return false;
}

bool VInputHandler::IsButtonUp(const sf::String& name, int controller)
{
	if (buttonInputs.find(name) != buttonInputs.end())
	{
		return !IsButtonDown(name);
	}

	return false;
}

bool VInputHandler::IsButtonReleased(const sf::String& name, int controller)
{
	if (buttonInputs.find(name) != buttonInputs.end())
	{
		return buttonInputs[name].released[controller];
	}

	return false;
}

float VInputHandler::CurrentAxisValue(const sf::String& name, int controller)
{
	if (axisInputs.find(name) != axisInputs.end())
	{
		return axisInputs[name].value[controller];
	}

	return 0;
}

float VInputHandler::LastAxisValue(const sf::String& name, int controller)
{
	if (axisInputs.find(name) != axisInputs.end())
	{
		return axisInputs[name].lastValue[controller];
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
	for (int i = 0; (i < sf::Joystick::Count && count < CONTROLLER_COUNT); i++)
	{
		JoystickID[i] = -1;
		if (sf::Joystick::isConnected(i))
		{
			JoystickID[count++] = i;
		}
	}
#endif

	for (int i = 0; i < CONTROLLER_COUNT; i++)
	{
		for (std::map<sf::String, ButtonInput>::iterator button = buttonInputs.begin(); button != buttonInputs.end(); ++button)
		{
			ButtonInput& b = button->second;

			b.pressed[i] = false;
			b.released[i] = false;

			if (sf::Keyboard::isKeyPressed(b.key) || 
				sf::Mouse::isButtonPressed(b.mouse) || 
#ifdef USE_GAMEPAD_API
				GamepadButtonDown((GAMEPAD_DEVICE)i, b.gamepad))
#elif defined(USE_SFML_JOYSTICK)
				(JoystickID[i] >= 0 && b.gamepad >= 0 && sf::Joystick::isButtonPressed(JoystickID[i], b.gamepad)))
#else
				sf::XInputDevice::isButtonPressed(i, b.gamepad))
#endif
			{
				if (!b.down[i])
					b.pressed[i] = true;
				b.down[i] = true;

				isGamepadActive = !(sf::Keyboard::isKeyPressed(b.key) || sf::Mouse::isButtonPressed(b.mouse));
			}
			else
			{
				if (b.down[i])
					b.released[i] = true;
				b.down[i] = false;
			}
		}

		for (std::map<sf::String, AxisInput>::iterator axis = axisInputs.begin(); axis != axisInputs.end(); ++axis)
		{
			AxisInput& a = axis->second;

			a.lastValue[i] = a.value[i];
			a.value[i] = 0;

			if (a.keyA >= 0)
			{
				if (sf::Keyboard::isKeyPressed(a.keyA))
				{
					a.value[i] = 100.0f;
					isGamepadActive = false;
				}
			}

			if (a.keyB >= 0)
			{
				if (sf::Keyboard::isKeyPressed(a.keyB))
				{
					a.value[i] = -100.0f;
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
					GamepadStickNormXY((GAMEPAD_DEVICE)i, STICK_LEFT, &val1, &val2);
					break;
				}
				case PovY:
				{
					GamepadStickNormXY((GAMEPAD_DEVICE)i, STICK_LEFT, &val2, &val1);
					val1 *= -1;
					break;
				}
				case Z:
				{
					GamepadStickNormXY((GAMEPAD_DEVICE)i, STICK_RIGHT, &val1, &val2);
					break;
				}
				case V:
				{
					GamepadStickNormXY((GAMEPAD_DEVICE)i, STICK_RIGHT, &val2, &val1);
					val1 *= -1;
					break;
				}
				case L:
				{
					val1 = GamepadTriggerValue((GAMEPAD_DEVICE)i, TRIGGER_LEFT) / 32767.0f;
					break;
				}
				case R:
				{
					val1 = GamepadTriggerValue((GAMEPAD_DEVICE)i, TRIGGER_RIGHT) / 32767.0f;
					break;
				}
				}

				val1 *= 100.0f;

				if ((int)val1 != 0)
				{
					a.value[i] = val1;
					isGamepadActive = true;
				}

#elif defined(USE_SFML_JOYSTICK)
				float axis = JoystickID[i] >= 0 && a.gamepad >= 0 ? sf::Joystick::getAxisPosition(JoystickID[i], a.gamepad) : 0.0f;
				if (axis != 0.0f) //Deadzone
				{
					a.value[i] = axis;
					isGamepadActive = true;
				}
				else
				{
					a.value[i] = 0;
				}
#else
				float axis = sf::XInputDevice::getAxisPosition(i, a.gamepad);
				if (axis != 0)
				{
					a.value[i] = axis;
					isGamepadActive = true;
				}
#endif
			}
		}
	}

	sf::Vector2i mousePosition = sf::Mouse::getPosition();

	if (mousePosition - lastMousePos != sf::Vector2i())
	{
		isGamepadActive = false;
	}

	lastMousePos = mousePosition;
}
