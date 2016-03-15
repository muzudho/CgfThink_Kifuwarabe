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
	this->kouNode = 0;		// �R�E�ɂȂ�ʒu�B
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
	int adjNode;	// �㉺���E�ɗאڂ����_
	int i;

	// �w�肵���ʒu�̐΂��폜�B
	this->table[tNode] = 0;

	// �S�����̐΂ɂ������������s���܂��B
	for (i = 0; i < 4; i++) {
		adjNode = tNode + this->dir4[i];
		if (this->table[adjNode] == color) {
			this->DeleteRenStones(adjNode, color);
		}
	}
}
