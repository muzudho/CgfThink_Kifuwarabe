	#include <tchar.h>		// Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB

	#include "../../header/h090_core____/n090_100_core.h"
	#include "../../header/h480_view____/n480_100_boardView.h"

	void BoardView::PrintBoard(Board* pBoard)
	{
		int x;
		int y;
		int node;
		_TCHAR* str[4] = {
			_T("�E"),	// ��
			_T("��"),	// ����
			_T("��"),	// ����
			_T("�{")		// �g
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

