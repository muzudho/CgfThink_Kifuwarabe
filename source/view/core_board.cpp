//
// /source/view/board.cpp
//

extern "C" {

	#include <tchar.h>		// Unicode対応の _T() 関数を使用するために。
	#include "../../header/core/core_ui.h"
	#include "../../header/think.h"

	extern int g_board[BOARD_MAX];
	extern int g_boardSize;

	// 現在の盤面を表示
	void print_board(void)
	{
		int x;
		int y;
		int node;
		_TCHAR* str[4] = { _T("・"), _T("●"), _T("○"), _T("＋") };

		for (y = 0; y < g_boardSize + 2; y++) for (x = 0; x < g_boardSize + 2; x++) {
			node = (y + 0) * 256 + (x + 0);
			PRT(_T("%s"), str[g_board[node]]);
			if (x == g_boardSize + 1) PRT(_T("\n"));
		}
	}
}
