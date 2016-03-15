	#include <tchar.h>		// Unicode対応の _T() 関数を使用するために。

	#include "../../header/h090_core____/n090_100_core.h"
	#include "../../header/h480_view____/n480_100_boardView.h"

	void BoardView::PrintBoard(Board* pBoard)
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

		for (y = 0; y < pBoard->size + 2; y++) {
			for (x = 0; x < pBoard->size + 2; x++) {
				node = (y + 0) * 256 + (x + 0);
				PRT(_T("%s"), str[pBoard->table[node]]);
				if (x == pBoard->size + 1) {
					PRT(_T("\n"));
				}
			}
		}
	}

