#include "XInputDevice.h"

#if !defined(USE_GAMEPAD_API) && !defined(USE_SFML_JOYSTICK)
#ifdef __MINGW32__
#define _in
#define _out
#define _reserved
#endif

#if defined _WIN32 || defined __MINGW32__
#include <Windows.h>
#include <XInput.h>

//#pragma comment(lib, "XInput.lib")
#pragma comment(lib, "XInput9_1_0.lib")

namespace sf
{
	bool XInputDevice::isConnected(unsigned int joystick)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		auto result = XInputGetState(joystick, &state);
		return (result == ERROR_SUCCESS);
	}

	bool XInputDevice::isAnyConnected()
	{
		for (int i = 0; i < 4; ++i)
		{
			if (isConnected(i))
				return true;
		}

		return false;
	}

	unsigned int XInputDevice::getButtonCount(unsigned int joystick)
	{
		return 14;
	}

	bool XInputDevice::isAnyButtonPressed(unsigned int joystick)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		auto result = XInputGetState(joystick, &state);

		if (result != ERROR_SUCCESS)
		{
			return false;
		}

		return state.Gamepad.wButtons > 0;
	}

	bool XInputDevice::isButtonPressed(unsigned int joystick, unsigned short button)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		auto result = XInputGetState(joystick, &state);

		if (result != ERROR_SUCCESS)
		{
			return false;
		}

		return (state.Gamepad.wButtons & button) != 0;
	}

	float XInputDevice::getAxisPosition(unsigned int joystick, XAxis axis)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		auto result = XInputGetState(joystick, &state);

		if (result != ERROR_SUCCESS)
		{
			return 0.0f;
		}

		if (axis == XAxis::PovX || axis == XAxis::PovY)
		{
			if (state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				return 0.0f;
			}

			if (axis == XAxis::PovX)
				return static_cast<float>(state.Gamepad.sThumbLX / 327);
			else
				return static_cast<float>(-state.Gamepad.sThumbLY / 327);
		}
		if (axis == XAxis::Z || axis == XAxis::V)
		{
			if (state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			{
				return 0.0f;
			}

			if (axis == XAxis::Z)
				return static_cast<float>(state.Gamepad.sThumbRX / 327);
			else
				return static_cast<float>(-state.Gamepad.sThumbRY / 327);
		}
		if (axis == XAxis::L)
		{
			return static_cast<float>(state.Gamepad.bLeftTrigger / 255);
		}
		if (axis == XAxis::R)
		{
			return static_cast<float>(state.Gamepad.bRightTrigger / 255);
		}

		return 0.0f;
	}
}
#else 
namespace sf{
	bool XInputDevice::isConnected(unsigned int joystick) {
		return false;
	}

	bool XInputDevice::isAnyConnected() {
		return false;
	}

	unsigned int XInputDevice::getButtonCount(unsigned int joystick) {
		return 0;
	}

	bool XInputDevice::isAnyButtonPressed(unsigned int joystick) {
		return false;
	}

	bool XInputDevice::isButtonPressed(unsigned int joystick, unsigned short button) {
		return false;
	}

	float XInputDevice::getAxisPosition(unsigned int joystick, XAxis axis) {
		return 0.0f;
	}
}
#endif
#endif
