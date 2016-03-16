#include <tchar.h>		// Unicode‘Î‰ž‚Ì _T() ŠÖ”‚ðŽg—p‚·‚é‚½‚ß‚ÉB
#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h480_view____/n480_200_libertyOfNodesView.h"




void LibertyOfNodesView::PrintBoard(Core core, LibertyOfNodes * pLibertyOfNodes)
{
	pLibertyOfNodes->ForeachAllXyWithWaku([&core, &pLibertyOfNodes](int x, int y, bool& isBreak) {
		int node = Board::ConvertToNode(x, y);

		core.PRT(_T("%2d"), pLibertyOfNodes->ValueOf(node));

		if (x == pLibertyOfNodes->GetSize() + 1) {
			core.PRT(_T("\n"));
		}
	});
}
