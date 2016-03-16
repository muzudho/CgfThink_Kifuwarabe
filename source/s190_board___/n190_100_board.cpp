#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h190_board___/n190_100_board.h"




void Board::Initialize(int initBoard[])
{
	// 現在局面を棋譜と初期盤面から作る
	for (int iNode = 0; iNode < BOARD_MAX; iNode++) {
		this->table[iNode] = initBoard[iNode];	// 初期盤面をコピー
	}
}


int Board::ConvertToNode(int x, int y)
{
	return y * 256 + x;
}




void Board::ConvertToXy(int& x, int& y, int node)
{
	y = node / 256;
	x = node % 256;
}




Board::Board()
{
	this->size = 0;
	this->kouNode = 0;		// コウになる位置。
	this->hama[0] = 0;	
	this->hama[BLACK] = 0;	// 取った石の数
	this->hama[WHITE] = 0;
}

Board::~Board()
{
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

