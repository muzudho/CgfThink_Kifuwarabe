#include <tchar.h>		// Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB
#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h480_view____/n480_200_libertyOfNodesView.h"




void LibertyOfNodesView::PrintBoard(HANDLE hConsoleWindow, LibertyOfNodes * pLibertyOfNodes)
{
	pLibertyOfNodes->ForeachAllXyWithWaku([&hConsoleWindow, &pLibertyOfNodes](int x, int y, bool& isBreak) {
		int node = Board::ConvertToNode(x, y);

		Core::PRT(hConsoleWindow, _T("%2d"), pLibertyOfNodes->ValueOf(node));

		if (x == pLibertyOfNodes->GetSize() + 1) {
			Core::PRT(hConsoleWindow, _T("\n"));
		}
	});
}