#include "VBase.h"

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <term.h>
#include <curses.h>
#include <gtk/gtk.h>
#endif

#include <stdarg.h>
#include <ctype.h>
#include <cstdio>
#include <cstring>


#ifdef _DEBUG
int VBase::DebugObjectCount = 0;
#endif

void VBase::VLog(const char* format, ...)
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL

	int len = strlen(buf);
	char* con = new char[len + 2];

	strcpy(con, buf);
	con[len] = '\n';
	con[len + 1] = '\0';

	printf(con, args);
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	wchar_t output[4096];
	std::mbstowcs(output, buf, strlen(buf) + 1);

#ifdef _MSC_VER
	OutputDebugString(output);
#endif

	delete[] con;
}

void VBase::VLogError(const char* format, ...)
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL

	int len = strlen(buf);
	char* con = new char[len + 2];

	strcpy(con, buf);
	con[len] = '\n';
	con[len + 1] = '\0';

	printf(con, args);
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	wchar_t output[4096];
	std::mbstowcs(output, buf, strlen(buf) + 1);

#ifdef _MSC_VER
	OutputDebugString(output);
#endif

#ifdef __linux__
	if (gtk_init_check(0, NULL))
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
	}
	throw output;
#else
	MessageBoxW(NULL, output, NULL, MB_TASKMODAL | MB_ICONERROR | MB_OK);
	throw std::exception(con);
#endif

	delete[] con;
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
	if (!cur_term)
	{
		int result;
		setupterm(NULL, STDOUT_FILENO, &result);
		if (result <= 0) return;
	}

	putp(tigetstr("clear"));
#endif
}
