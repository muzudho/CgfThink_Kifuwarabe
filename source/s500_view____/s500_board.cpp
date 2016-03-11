//
// /source/s500_board/s500_board.cpp
//

extern "C" {

	#include <tchar.h>		// UnicodeΞΜ _T() Φπgp·ι½ίΙB
	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h500_view____/h500_board.h"

	void PrintBoard(void)
	{
		int x;
		int y;
		int node;
		_TCHAR* str[4] = {
			_T("E"),	// σ«
			_T(""),	// Ξ
			_T(""),	// Ξ
			_T("{")		// g
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
