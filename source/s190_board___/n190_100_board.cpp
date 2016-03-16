#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h190_board___/n190_100_board.h"




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
	this->kouNode = 0;		// �R�E�ɂȂ�ʒu�B
	this->hama[0] = 0;	
	this->hama[BLACK] = 0;	// ������΂̐�
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
	// �w�肵���ʒu�̐΂��폜�B
	this->table[tNode] = 0;

	// �S�����̐΂ɂ������������s���܂��B
	this->ForeachArroundNodes(tNode, [this,color]( int adjNode,	bool& isBreak) {
		if (this->table[adjNode] == color) {
			this->DeleteRenStones(adjNode, color);
		}
	});
}

