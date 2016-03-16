#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_150_liberty.h"
#include "../../header/h670_noHit___/n670_100_noHitOwnEye.h"




NoHitOwnEye::NoHitOwnEye()
{
	this->safe = 0;
};




bool NoHitOwnEye::IsThis(
	int		color			,
	int		node			,
	Liberty liberties[4]	,
	Board*	pBoard
)
{
	bool result = false;

	pBoard->ForeachArroundDirAndNodes(node, [this,&pBoard,&liberties, color](int iDir, int adjNode, bool& isBreak) {
		int adjColor = pBoard->table[adjNode];		// �㉺���E��(adjacent)�̐΂̐F

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
	});
	/*
	int iDir;
	int adjNode;	// �㉺���E��(adjacent)�̌�_
	int adjColor;	// �㉺���E��(adjacent)�̐΂̐F
	for (iDir = 0; iDir < 4; iDir++) {		// ��� �� �E�� �� ���� �� ����
		adjNode = node + pBoard->dir4[iDir];	// �א�(adjacent)�����_�ƁA
		adjColor = pBoard->table[adjNode];		// ���̐F

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
	*/


	if (this->safe == 4) { // �l�����@�����̐΂�A�ǂɁ@�͂܂�Ă���ꏊ�i��j�ɂȂ�Ȃ�
						   //PRT(_T("��ɂ͑ł��Ȃ��B \n"));
						   // ��ɂ͑ł��Ȃ��B
		result = 1;
		goto gt_EndMethod;
	}

gt_EndMethod:
	return result;
}
