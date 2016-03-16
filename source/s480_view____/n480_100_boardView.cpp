	#include <tchar.h>		// Unicode‘Î‰ž‚Ì _T() ŠÖ”‚ðŽg—p‚·‚é‚½‚ß‚ÉB

	#include "../../header/h090_core____/n090_100_core.h"
	#include "../../header/h480_view____/n480_100_boardView.h"

	void BoardView::PrintBoard(HANDLE hConsoleWindow, Board* pBoard)
	{
		int x;
		int y;
		int node;
		_TCHAR* str[4] = {
			_T("E"),	// ‹ó‚«
			_T("œ"),	// •Î
			_T("›"),	// ”’Î
			_T("{")		// ˜g
		};

		pBoard->ForeachAllXyWithWaku([&pBoard,&hConsoleWindow,&str](int x,int y) {
			int node = Board::ConvertToNode(x, y);
			Core::PRT(hConsoleWindow, _T("%s"), str[pBoard->table[node]]);
			if (x == pBoard->size + 1) {
				Core::PRT(hConsoleWindow, _T("\n"));
			}
		});
		/*
		for (y = 0; y < pBoard->size + 2; y++) {
			for (x = 0; x < pBoard->size + 2; x++) {
				node = (y + 0) * 256 + (x + 0);
				Core::PRT(hConsoleWindow, _T("%s"), str[pBoard->table[node]]);
				if (x == pBoard->size + 1) {
					Core::PRT(hConsoleWindow, _T("\n"));
				}
			}
		}
		*/
	}

