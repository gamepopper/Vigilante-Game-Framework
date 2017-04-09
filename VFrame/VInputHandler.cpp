#include "VInputHandler.h"

VInputHandler::~VInputHandler()
{
	buttonInputs.clear();
	axisInputs.clear();
}

void VInputHandler::AddButtonInput(sf::String name, int key, int gamepad, int mouse)
{
	ButtonInput button;
	button.key = (sf::Keyboard::Key)key;
	if (gamepad > 0)
		button.gamepad = (sf::XInputDevice::XButton)((unsigned short)gamepad);
	else
		button.gamepad = sf::XInputDevice::NONE;

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
	axis.gamepad = (sf::XInputDevice::XAxis)gamepad;
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

void VInputHandler::Update(float dt)
{
	for (std::map<sf::String, ButtonInput>::iterator button = buttonInputs.begin(); button != buttonInputs.end(); ++button)
	{
		ButtonInput& b = button->second;

		b.pressed = false;
		b.released = false;

		if (sf::Keyboard::isKeyPressed(b.key) || sf::XInputDevice::isButtonPressed(ControllerNo, b.gamepad) || sf::Mouse::isButtonPressed(b.mouse))
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
			if (sf::XInputDevice::getAxisPosition(ControllerNo, a.gamepad) != 0)
			{
				a.value = sf::XInputDevice::getAxisPosition(ControllerNo, a.gamepad);
				isGamepadActive = true;
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
