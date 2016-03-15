
extern "C" {

	#include "../../header/h190_board___/n190_100_board.h"
	#include "../../header/h190_board___/n190_150_liberty.h"
	#include "../../header/h300_move____/n300_100_move.h"
	#include "../../header/h670_noHit___/n670_050_noHitSuicide.h"
}




NoHitSuicide::NoHitSuicide() {
	this->flgCapture = 0;
}




// ���E��ɂȂ�󋵂łȂ��������B
bool NoHitSuicide::IsThis(
	int		color		,
	int		node		,
	Liberty liberties[4],
	Board*	pBoard
)
{
	bool result = false;
	int invColor = INVCLR(color);	//�������]
	int iDir;
	int adjNode;	// �㉺���E��(adjacent)�̌�_
	int adjColor;	// �㉺���E��(adjacent)�̐΂̐F

	for (iDir = 0; iDir < 4; iDir++) {		// ��� �� �E�� �� ���� �� ����
		adjNode = node + pBoard->dir4[iDir];	// �א�(adjacent)�����_�ƁA
		adjColor = pBoard->table[adjNode];		// ���̐F

		// �ׂɁA�ċz�_�� 1 �̑���̐΂�����΁A����͎�邱�Ƃ��ł��܂��B
		if (adjColor == invColor && liberties[iDir].liberty == 1) {
			//PRT(_T("�G�΂�������B \n"));
			this->flgCapture = 1; 	// �G�΂��A������t���O�B
		}
	}

	int flgMove;	// �ړ����ʂ̎��

	if (this->flgCapture == 0) {					// �΂����Ȃ��ꍇ
													// ���ۂɒu���Ă݂ā@���E�肩�ǂ�������
		int temp_kouNode = pBoard->kouNode;		// �R�E�̈ʒu��ޔ�

		flgMove = MoveOne(node, color, pBoard);		// �΂�u���܂��B�R�E�̈ʒu���ς�邩���B

											// �΂�u���O�̏�Ԃɖ߂��܂��B
		pBoard->table[node] = 0;					// �u�����΂������܂��B
		pBoard->kouNode = temp_kouNode;			// �R�E�̈ʒu�����ɖ߂��܂��B

		if (flgMove == MOVE_SUICIDE) {		// ���E��Ȃ�
											//PRT(_T("���E��͑ł��Ȃ��B \n"));
											// �x�X�g���[�u�ɂ͂Ȃ肦�Ȃ�
			result = true;
			goto gt_EndMethod;
		}
	}

gt_EndMethod:
	return result;
}
