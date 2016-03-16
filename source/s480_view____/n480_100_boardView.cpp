	#include <tchar.h>		// Unicode‘Î‰ž‚Ì _T() ŠÖ”‚ðŽg—p‚·‚é‚½‚ß‚ÉB

	#include "../../header/h090_core____/n090_100_core.h"
	#include "../../header/h480_view____/n480_100_boardView.h"

	void BoardView::PrintBoard(HANDLE hConsoleWindow, Board* pBoard)
	{
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
	}

