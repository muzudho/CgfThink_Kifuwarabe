//
// /source/s680_suicide/s680_suicide.cpp
//

extern "C" {

	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h300_move____/h300_move.h"

	void JudgeSuicide(
		int&	flgAbort,
		int		flgCapture,
		int		color,
		int		node
	)
	{
		int flgMove;	// �ړ����ʂ̎��

		if (flgCapture == 0) {					// �΂����Ȃ��ꍇ
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

}