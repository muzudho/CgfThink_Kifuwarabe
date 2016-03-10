extern "C" {

	#include <windows.h>	// rand() 等を使用するために。
	#include <tchar.h>		// Unicode対応の _T() 関数を使用するために。
	#include "../../header/h400_core/h400_ui.h"


	// printf()の代用関数。
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