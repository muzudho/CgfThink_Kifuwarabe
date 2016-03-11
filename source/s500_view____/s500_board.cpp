//
// /source/s500_board/s500_board.cpp
//

extern "C" {

	#include <tchar.h>		// Unicode対応の _T() 関数を使用するために。
	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h500_view____/h500_board.h"

	void PrintBoard(void)
	{
		int x;
		int y;
		int node;
		_TCHAR* str[4] = {
			_T("・"),	// 空き
			_T("●"),	// 黒石
			_T("○"),	// 白石
			_T("＋")		// 枠
		};

		for (y = 0; y < g_boardSize + 2; y++) {
			for (x = 0; x < g_boardSize + 2; x++) {
				node = (y + 0) * 256 + (x + 0);
				PRT(_T("%s"), str[g_board[node]]);
				if (x == g_boardSize + 1) {
					PRT(_T("\n"));
				}
			}
		}
	}
}
