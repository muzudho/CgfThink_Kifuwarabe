//
// /source/s680_suicide/s680_suicide.cpp
//

extern "C" {

	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h300_move____/h300_move.h"
	#include "../../header/h680_suicide_/h680_suicide.h"
}

Suicide::Suicide() {

}

// ���E��ɂȂ�󋵂łȂ��������B
void Suicide::IsThis(
	int adjColor,
	int invClr
)
{
	// �ׂɁA�ċz�_�� 1 �̑���̐΂�����΁A����͎�邱�Ƃ��ł��܂��B
	if (adjColor == invClr && g_liberty == 1) {
		//PRT(_T("�G�΂�������B \n"));
		this->flgCapture = 1; 	// �G�΂��A������t���O�B
	}
}

void Suicide::Judge(
	int&	flgAbort,
	int		color,
	int		node
	)
{
	int flgMove;	// �ړ����ʂ̎��

	if (this->flgCapture == 0) {					// �΂����Ȃ��ꍇ
											// ���ۂɒu���Ă݂ā@���E�肩�ǂ�������
		int temp_kouNode = g_kouNode;		// �R�E�̈ʒu��ޔ�

		flgMove = MoveOne(node, color);		// �΂�u���܂��B�R�E�̈ʒu���ς�邩���B

											// �΂�u���O�̏�Ԃɖ߂��܂��B
		g_board[node] = 0;					// �u�����΂������܂��B
		g_kouNode = temp_kouNode;			// �R�E�̈ʒu�����ɖ߂��܂��B

		if (flgMove == MOVE_SUICIDE) {		// ���E��Ȃ�
											//PRT(_T("���E��͑ł��Ȃ��B \n"));
											// �x�X�g���[�u�ɂ͂Ȃ肦�Ȃ�
			flgAbort = 1;
			goto gt_EndMethod;
		}

	gt_EndMethod:
		;
	}
}
