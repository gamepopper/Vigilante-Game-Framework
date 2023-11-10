#include "VBase.h"

#if defined(_WIN32)
#include <windows.h>
#endif

#include <stdarg.h>
#include <ctype.h>
#include <cstdio>
#include <cstring>
#include <fstream>

#ifdef _DEBUG
int VBase::DebugObjectCount = 0;
#endif

wchar_t output[4096];
const wchar_t* VBase::VString(const char* format, ...)
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL

	int len = strlen(buf);
	buf[len] = '\0';
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	for (unsigned int i = 0; i < strlen(buf); ++i)
		output[i] = (wchar_t)buf[i];
	output[strlen(buf)] = L'\0';

	return output;
}

void VBase::VLog(const char* format, ...)
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL

	int len = strlen(buf);
	buf[len] = '\n';
	buf[len + 1] = '\0';

	printf(buf, args);
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	for (unsigned int i = 0; i < strlen(buf); ++i)
		output[i] = (wchar_t)buf[i];
	output[strlen(buf)] = L'\0';

#ifdef _MSC_VER
	OutputDebugStringW(output);
#endif
}

std::wostream& wendl(std::wostream& out)
{
	// needed for binary mode files to retain Windows-style newline "\x0D\x0A"
	return out.put(L'\r').put(L'\n').flush();
}//wendl

void VBase::VLogError(const char* format, ...)
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL

	int len = strlen(buf);
	buf[len] = '\n';
	buf[len + 1] = '\0';

	printf(buf, args);
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	for (unsigned int i = 0; i < strlen(buf); ++i)
		output[i] = (wchar_t)buf[i];
	output[strlen(buf)] = L'\0';

	std::wfstream outfile;

	outfile.open("error.txt", std::ios::out | std::ios::binary);
	outfile << output << wendl;
	outfile.close();

#ifdef _MSC_VER
	OutputDebugStringW(output);
#endif

#ifdef __linux__
	/*if (gtk_init_check(0, NULL))
	{
		GtkWidget *dialog = gtk_message_dialog_new(NULL,
			GTK_DIALOG_MODAL,
			GTK_MESSAGE_ERROR,
			Buttons::OK,
			"%s",
			output);
		gtk_window_set_title(GTK_WINDOW(dialog), "Error");
		Selection selection = getSelection(gtk_dialog_run(GTK_DIALOG(dialog)));

		gtk_widget_destroy(GTK_WIDGET(dialog));
		while (g_main_context_iteration(NULL, false));
	}*/
	//throw output;
#else
	MessageBoxW(NULL, output, NULL, MB_TASKMODAL | MB_ICONERROR | MB_OK);
	DebugBreak();
	//throw std::exception(buf);
#endif
}

//Source http://www.cplusplus.com/articles/4z18T05o/
void VBase::VClearLog()
{
#if defined(_WIN32)
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
#endif

#ifdef __linux__
	for (int n = 0; n < 10; n++)
		printf("\n\n\n\n\n\n\n\n\n\n");
#endif
}

#if !defined(VFRAME_NO_3D) && defined(_DEBUG)
#ifndef NO_GLEW
#include <GL/glew.h>
#endif
#include <SFML/OpenGL.hpp>

#define GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION 0x0506

void VBase::VCheckGLError(const char* file, unsigned int line, const char* expression)
{
	// Get the last error
	GLenum errorCode = glGetError();

	if (errorCode != GL_NO_ERROR)
	{
		std::string fileString = file;
		std::string error = "Unknown error";
		std::string description = "No description";

		// Decode the error code
		switch (errorCode)
		{
			case GL_INVALID_ENUM:
			{
				error = "GL_INVALID_ENUM";
				description = "An unacceptable value has been specified for an enumerated argument.";
				break;
			}

			case GL_INVALID_VALUE:
			{
				error = "GL_INVALID_VALUE";
				description = "A numeric argument is out of range.";
				break;
			}

			case GL_INVALID_OPERATION:
			{
				error = "GL_INVALID_OPERATION";
				description = "The specified operation is not allowed in the current state.";
				break;
			}

			case GL_STACK_OVERFLOW:
			{
				error = "GL_STACK_OVERFLOW";
				description = "This command would cause a stack overflow.";
				break;
			}

			case GL_STACK_UNDERFLOW:
			{
				error = "GL_STACK_UNDERFLOW";
				description = "This command would cause a stack underflow.";
				break;
			}

			case GL_OUT_OF_MEMORY:
			{
				error = "GL_OUT_OF_MEMORY";
				description = "There is not enough memory left to execute the command.";
				break;
			}

			case GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION:
			{
				error = "GL_INVALID_FRAMEBUFFER_OPERATION";
				description = "The object bound to FRAMEBUFFER_BINDING is not \"framebuffer complete\".";
				break;
			}
		}

		// Log the error
		VBase::VLog("An internal OpenGL call failed in %s (%d).\nExpression\n%s\nError Description:\n\t%s\n%s", fileString.substr(fileString.find_last_of("\\/") + 1).c_str(), line, expression, error.c_str(), description.c_str());
	}
}
#endif
