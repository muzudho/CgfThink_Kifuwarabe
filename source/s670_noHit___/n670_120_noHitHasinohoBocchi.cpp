extern "C" {
	#include "../../header/h090_core____/n090_100_core.h"
	#include "../../header/h190_board___/n190_100_board.h"
	#include "../../header/h670_noHit___/n670_120_noHitHasinohoBocchi.h"
}




NoHitHasinohoBocchi::NoHitHasinohoBocchi()
{
	this->isBocchi		= false;
	this->isSoto		= false;
	this->isEdge		= false;
	this->isCorner		= false;
}





void NoHitHasinohoBocchi::Research(
	int		node,
	Board*	pBoard
	) {

	this->isSoto		= false;
	this->isEdge		= false;
	this->isCorner		= false;



	this->isBocchi = true;
	for (int iDir = 0; iDir < 4; iDir++) {		// ��� �� �E�� �� ���� �� ����
		int adjNode = node + pBoard->dir4[iDir];		// �א�(adjacent)�����_�ƁA
		int adjColor = pBoard->table[adjNode];		// ���̐F

		if (adjColor==BLACK || adjColor==WHITE)
		{
			// �ڂ����ł͂Ȃ��B
			this->isBocchi = false;
			break;
		}
	}



	int x, y;
	Board::ConvertToXy(x, y, node);

	if (x < 1 || pBoard->size < x ||
		y < 1 || pBoard->size < y
		) {
		// �ՊO
		//PRT(_T("(%d,%d) ban=%d ; Soto \n"), x, y, boardSize);
		this->isSoto = true;
		goto gt_EndMethod;
	}

	if (x == 1 || pBoard->size == x ||
		y == 1 || pBoard->size == y
	) {
		// ��
		//PRT(_T("(%d,%d) ban=%d ; EDGE \n"), x, y, boardSize);
		this->isEdge = true;
	}
	else
	{
		//PRT(_T("(%d,%d) ban=%d ; ------ \n"), x, y, boardSize);
		goto gt_EndMethod;
	}

	if ( (x == 1 || pBoard->size == x) &&
		(y == 1 || pBoard->size == y)
	) {
		// �p
		//PRT(_T("(%d,%d) ban=%d ; CORNER \n"), x, y, boardSize);
		this->isCorner = true;
	}

gt_EndMethod:
	;
}




// �]���l���o���܂��B
int NoHitHasinohoBocchi::Evaluate(
	) {
	int score = 100;

	if (!this->isBocchi)
	{
		// �ڂ����΂łȂ���΁A�C�ɂ����@�[�ł��p�ł��u���܂��B
		goto gt_EndMethod;
	}

	if (this->isCorner)
	{
		// �p�Ɂ@�ڂ����΁@��u�������Ȃ��B
		score -= 50;
		goto gt_EndMethod;
	}

	if (this->isEdge)
	{
		// �ӂɁ@�ڂ����΁@��u�������Ȃ��B
		score -= 33;
		goto gt_EndMethod;
	}

gt_EndMethod:
	return score;
}
