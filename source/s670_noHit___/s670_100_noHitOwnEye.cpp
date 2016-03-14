//
// /source/s670_noHitEye/s670_noHitEye.cpp
//

extern "C" {
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h200_research/h200_100_Liberty.h"
	#include "../../header/h670_noHit___/h670_100_noHitOwnEye.h"
}




NoHitOwnEye::NoHitOwnEye()
{
	this->safe = 0;
};




bool NoHitOwnEye::IsThis(
	int color,
	int	node,
	Liberty liberties[4]
)
{
	bool result = false;

	int iDir;
	int adjNode;	// �㉺���E��(adjacent)�̌�_
	int adjColor;	// �㉺���E��(adjacent)�̐΂̐F

	for (iDir = 0; iDir < 4; iDir++) {		// ��� �� �E�� �� ���� �� ����
		adjNode = node + g_dir4[iDir];	// �א�(adjacent)�����_�ƁA
		adjColor = g_board[adjNode];		// ���̐F

		// ���̂Q�́@���S�ȂȂ�����ł��B
		// (�P)�g�ɂȂ���B
		// (�Q)�ċz�_�� 2 �ȏ゠��i���΂�u���Ă��ċz�_�� 1 �ȏ�c��A
		//     ���E��ɂ͂Ȃ�Ȃ��j�����ɂȂ���B
		if (
			adjColor == WAKU
			||
			(adjColor == color && 2 <= liberties[iDir].liberty)
			) {
			//PRT(_T("���S�ȗאځB \n"));
			this->safe++;
		}
	}


	if (this->safe == 4) { // �l�����@�����̐΂�A�ǂɁ@�͂܂�Ă���ꏊ�i��j�ɂȂ�Ȃ�
						   //PRT(_T("��ɂ͑ł��Ȃ��B \n"));
						   // ��ɂ͑ł��Ȃ��B
		result = 1;
		goto gt_EndMethod;
	}

gt_EndMethod:
	return result;
}
