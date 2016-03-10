//
// /source/view/board.cpp
//

extern "C" {

	#include <tchar.h>		// Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB
	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h500_view____/h500_board.h"

	// ���݂̔Ֆʂ�\��
	void PrintBoard(void)
	{
		int x;
		int y;
		int node;
		_TCHAR* str[4] = { _T("�E"), _T("��"), _T("��"), _T("�{") };

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
