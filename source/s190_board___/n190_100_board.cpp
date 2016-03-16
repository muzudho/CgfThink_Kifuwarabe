#include <vector>
#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h190_board___/n190_100_board.h"




void Board::Initialize(int initBoard[])
{
	// 現在局面を棋譜と初期盤面から作る
	for (int iNode = 0; iNode < BOARD_MAX; iNode++) {
		this->table[iNode] = initBoard[iNode];	// 初期盤面をコピー
	}
}



Board::Board()
{
	this->kouNode = 0;		// コウになる位置。
	this->hama[0] = 0;	
	this->hama[BLACK] = 0;	// 取った石の数
	this->hama[WHITE] = 0;
}

Board::~Board()
{
}

int Board::NorthOf(int node)
{
	return this->table[node + this->dir4[0]];
}

int Board::EastOf(int node)
{
	return this->table[node + this->dir4[1]];
}

int Board::SouthOf(int node)
{
	return this->table[node + this->dir4[2]];
}

int Board::WestOf(int node)
{
	return this->table[node + this->dir4[3]];
}

std::vector<int> Board::GetOpenNodesOfStone(int node, int size123)
{
	std::vector<int> openNodes;

	// 上側 → 右側 → 下側 → 左側
	this->ForeachArroundNodes(node,[this,size123,&openNodes](int adjNode, bool& isBreak) {
		if (adjNode == EMPTY && adjNode != this->kouNode)
		{
			// 空きスペースで、コウにならない位置なら。
			openNodes.push_back(adjNode);

			if (openNodes.size() == size123) {
				// 計算を打ち切り。
				isBreak = true;
				goto gt_Next;
			}
		}

	gt_Next:
		;
	});

gt_EndMethod:
	return openNodes;
}




void Board::DeleteRenStones(
	int tNode,
	int color
	)
{
	// 指定した位置の石を削除。
	this->table[tNode] = 0;

	// ４方向の石にも同じ処理を行います。
	this->ForeachArroundNodes(tNode, [this,color]( int adjNode,	bool& isBreak) {
		if (this->table[adjNode] == color) {
			this->DeleteRenStones(adjNode, color);
		}
	});
}

