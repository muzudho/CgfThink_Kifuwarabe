//
// /source/s600_research/s600_100_adjLiberty.cpp
//

#include "../../header/h190_board___/n190_150_liberty.h"




Liberty::Liberty()
{
	this->liberty = 0;
	this->renIshi = 0;
};




void Liberty::Count(int node, Board* pBoard)
{
	int thisColor;	// 上下左右隣(adjacent)の石の色

	thisColor = pBoard->table[node];		// その色

									// 眼に打ち込まないか、口の中に打ち込まないか、の処理のあとに
	if (thisColor == 0 || thisColor == WAKU) {
		// 空っぽか、枠なら。
		//PRT(_T("空っぽか、枠。 \n"));
		goto gt_EndMethod;
	}

	int i;

	this->liberty = 0;
	this->renIshi = 0;
	for (i = 0; i < BOARD_MAX; i++) {
		this->checkedBoard[i] = 0;
	}

	this->CountElement(node, pBoard->table[node], pBoard);

gt_EndMethod:
	return;
}




void Liberty::CountElement(int tNode, int color, Board* pBoard)
{
	int adjNode;
	int iDir;

	this->checkedBoard[tNode] = 1;					// この石は検索済み	
	this->renIshi++;								// 呼吸点を数えている（１個または連の）
													// 石の数
	for (iDir = 0; iDir < 4; iDir++) {				// 隣接する四方向
		adjNode = tNode + pBoard->dir4[iDir];
		if (this->checkedBoard[adjNode]) {
			continue;
		}
		if (pBoard->table[adjNode] == 0) {				// 空点
			this->checkedBoard[adjNode] = 1;			// この空点は検索済みとする
			this->liberty++;							// リバティの数
		}
		if (pBoard->table[adjNode] == color) {
			this->CountElement(adjNode, color, pBoard);	// 未探索の自分の石
		}
	}
}
