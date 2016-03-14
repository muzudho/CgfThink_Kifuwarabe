extern "C" {
	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h670_noHit___/h670_120_noHitHasinohoBocchi.h"
}




NoHitHasinohoBocchi::NoHitHasinohoBocchi()
{
	this->isBocchi		= false;
	this->isSoto		= false;
	this->isEdge		= false;
	this->isCorner		= false;
}





void NoHitHasinohoBocchi::Research(
	int node
	) {

	this->isSoto		= false;
	this->isEdge		= false;
	this->isCorner		= false;



	this->isBocchi = true;
	for (int iDir = 0; iDir < 4; iDir++) {		// ��� �� �E�� �� ���� �� ����
		int adjNode = node + g_dir4[iDir];		// �א�(adjacent)�����_�ƁA
		int adjColor = g_board[adjNode];		// ���̐F

		if (adjColor==BLACK || adjColor==WHITE)
		{
			// �ڂ����ł͂Ȃ��B
			this->isBocchi = false;
			break;
		}
	}



	int x, y;
	ConvertToXy(x, y, node);

	if (x < 0 || (g_boardSize - 1) < x ||
		y < 0 || (g_boardSize - 1) < y
		) {
		// �ՊO
		//PRT(_T("(%d,%d) ban=%d ; Soto \n"), x, y, g_boardSize);
		this->isSoto = true;
		goto gt_EndMethod;
	}

	if (x == 0 || (g_boardSize-1) == x ||
		y == 0 || (g_boardSize-1) == y
	) {
		// ��
		//PRT(_T("(%d,%d) ban=%d ; EDGE \n"), x, y, g_boardSize);
		this->isEdge = true;
	}
	else
	{
		//PRT(_T("(%d,%d) ban=%d ; ------ \n"), x, y, g_boardSize);
		goto gt_EndMethod;
	}

	if ( (x == 0 || (g_boardSize - 1) == x) &&
		(y == 0 || (g_boardSize - 1) == y)
	) {
		// �p
		//PRT(_T("(%d,%d) ban=%d ; CORNER \n"), x, y, g_boardSize);
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
