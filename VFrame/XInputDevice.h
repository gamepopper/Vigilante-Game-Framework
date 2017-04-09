#pragma once

namespace sf
{
	class XInputDevice
	{
	public:
		enum
		{
			Count = 4
		};

		enum XButton
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

		enum XAxis
		{
			PovX,
			PovY,
			Z,
			V,
			L,
			R
		};

		static bool isConnected(unsigned int joystick);
		static unsigned int getButtonCount(unsigned int joystick);
		static bool isAnyButtonPressed(unsigned int joystick);
		static bool isButtonPressed(unsigned int joystick, unsigned short button);
		static bool isAnyConnected();
		static float getAxisPosition(unsigned int joystick, XAxis axis);
	};
}
#if !(defined _WIN32 || defined __MINGW32__)
#warning "XInputDevice does not work on non-windows platforms"
#endif
