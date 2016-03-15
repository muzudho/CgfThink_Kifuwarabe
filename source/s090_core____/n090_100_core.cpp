	#include <windows.h>	// rand() 等を使用するために。
	#include <tchar.h>		// Unicode対応の _T() 関数を使用するために。

extern "C" {

	#include "../../header/h090_core____/n090_100_core.h"


	//--------------------------------------------------------------------------------
	// 関数
	//--------------------------------------------------------------------------------

	// printf()の代用関数。
	void PRT(HANDLE hConsoleWindow, const _TCHAR* format, ...)
	{
		va_list			argList;
		int				len;
		static _TCHAR	text[PRT_LEN_MAX];
		DWORD			nw;

		if (hConsoleWindow == INVALID_HANDLE_VALUE) {
			return;
		}
		va_start(argList, format);
		len = _vsnwprintf(text, PRT_LEN_MAX - 1, format, argList);
		va_end(argList);

		if (len < 0 || len >= PRT_LEN_MAX) {
			return;
		}
		WriteConsole(hConsoleWindow, text, (DWORD)wcslen(text), &nw, NULL);
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

//--------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------

// コンソールに出力するためのハンドル
HANDLE g_hConsoleWindow = INVALID_HANDLE_VALUE; //static HANDLE hOutput;
