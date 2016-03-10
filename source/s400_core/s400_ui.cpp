extern "C" {

	#include <windows.h>	// rand() �����g�p���邽�߂ɁB
	#include <tchar.h>		// Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB
	#include "../../header/h400_core/h400_ui.h"


	// printf()�̑�p�֐��B
	void PRT(const _TCHAR* format, ...)
	{
		va_list			argList;
		int				len;
		static _TCHAR	text[PRT_LEN_MAX];
		DWORD			nw;

		if (g_hConsoleWindow == INVALID_HANDLE_VALUE) {
			return;
		}
		va_start(argList, format);
		len = _vsnwprintf(text, PRT_LEN_MAX - 1, format, argList);
		va_end(argList);

		if (len < 0 || len >= PRT_LEN_MAX) {
			return;
		}
		WriteConsole(g_hConsoleWindow, text, (DWORD)wcslen(text), &nw, NULL);
	}

	void YieldWindowsSystem(void)
	{
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}