/**
* @file    XInputDevice.h
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
* Class for handling input of XInput (i.e. Xbox 360/One) Gamepad Controllers. (Only used if USE_GAMEPAD_API preprocessor is not defined).
*/

#pragma once

#ifndef USE_GAMEPAD_API
namespace sf
{
	///Class for directly accessing and managing XInput controls (Windows Only!)
	class XInputDevice
	{
	public:
		///Maximum Amount of Gamepads.
		enum : unsigned char
		{
			Count = 4
		};

		///All the buttons of a single gamepad and their respective value.
		enum XButton : unsigned short
		{
			DPAD_UP		= 0x0001,
			DPAD_DOWN	= 0x0002,
			DPAD_LEFT	= 0x0004,
			DPAD_RIGHT	= 0x0008,
			START		= 0x0010,
			BACK		= 0x0020,
			LEFT_THUMB	= 0x0040,
			RIGHT_THUMB	= 0x0080,
			LB			= 0x0100,
			RB			= 0x0200,
			A			= 0x1000,
			B			= 0x2000,
			X			= 0x4000,
			Y			= 0x8000
		};

		///All the available axis types.
		enum XAxis : unsigned char
		{
			PovX,
			PovY,
			Z,
			V,
			L,
			R
		};

		/**
		* Returns true if a specific gamepad is connected.
		* @param joystick Player Gamepad Index. Must be less than the XInputDevice::Count value.
		* @return Returns true if a gamepad is connected.
		*/
		static bool isConnected(unsigned int joystick);

		/**
		* Returns the amount of available buttons on this gamepad.
		* @param joystick Player Gamepad Index. Must be less than the XInputDevice::Count value.
		* @return Returns number of available buttons on the gamepad.
		*/
		static unsigned int getButtonCount(unsigned int joystick);

		/**
		* Tests if any button on a gamepad has been pressed.
		* @param joystick Player Gamepad Index. Must be less than the XInputDevice::Count value.
		* @return Returns true if any button has been pressed.
		*/
		static bool isAnyButtonPressed(unsigned int joystick);
		
		/**
		* Tests if a specific button on a gamepad has been pressed.
		* @param joystick Player Gamepad Index. Must be less than the XInputDevice::Count value.
		* @param button The button you want to test, use XButton for which button.
		* @return Returns true if a specific button at a specific joystick/player index is currently being pressed. Returns false if not connected.
		*/
		static bool isButtonPressed(unsigned int joystick, unsigned short button);

		/**
		* Tests if any gamepad is connected at that point.
		* @return Returns true if any gamepad is connected.
		*/
		static bool isAnyConnected();

		/**
		* Return the value of a specified axis on a gamepad.
		* @param joystick Player Gamepad Index. Must be less than the XInputDevice::Count value.
		* @param axis The axis you want to test, use XAxis for which axis.
		* @return Returns a float between -1.0f and 1.0f for the axis. Returns 0 if one is not connected.
		*/
		static float getAxisPosition(unsigned int joystick, XAxis axis);
	};
}
#if !(defined _WIN32 || defined __MINGW32__)
#warning "XInputDevice does not work on non-windows platforms"
#endif
#endif
