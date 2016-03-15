extern "C" {

	#include "../../header/h090_core____/n090_100_core.h"
	#include "../../header/h190_board___/n190_100_board.h"
}




int Board::ConvertToNode(int x, int y)
{
	return (y + 1) * 256 + (x + 1);
}




void Board::ConvertToXy(int& x, int& y, int node)
{
	y = (node - 256) / 256;
	x = (node - 1) % 256;
}




Board::Board()
{
	this->kouNode = 0;		// コウになる位置。
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
	int adjNode;	// 上下左右に隣接する交点
	int i;

	// 指定した位置の石を削除。
	this->table[tNode] = 0;

	// ４方向の石にも同じ処理を行います。
	for (i = 0; i < 4; i++) {
		adjNode = tNode + this->dir4[i];
		if (this->table[adjNode] == color) {
			this->DeleteRenStones(adjNode, color);
		}
	}
}
