#include "VBase.h"

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <cstdio>
#include <cstring>

#if defined(_WIN32)
#include <windows.h>
#endif

#ifdef _DEBUG
int VBase::DebugObjectCount = 0;
#endif

void VBase::VLog(const char* format, ...)
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf_s(buf, _vscprintf(format, args), format, args); // buf-3 is room for CR/LF/NUL

	int len = strlen(buf);
	buf[len++] = '\n';
	buf[len++] = '\0';

	printf(buf, args);
	va_end(args);

	wchar_t output[4096];
	std::mbstowcs(output, buf, strlen(buf) + 1);

#ifdef _MSC_VER
	OutputDebugString(output);
#endif
}

