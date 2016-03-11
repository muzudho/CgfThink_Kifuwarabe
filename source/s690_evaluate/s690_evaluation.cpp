//
// /source/s690_evaluate/s690_evaluation.cpp
//

extern "C" {

	#include <windows.h>								// rand() �����g�p���邽�߂ɁB
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h300_move____/h300_move.h"


	int Evaluate(
		int&	flgAbort	,
		int		color		,
		int		node
	)
	{
		int score;		// �ǂ�ł����̕]���l
		int flgCapture;	// �G�΂�������t���O
		int safe;
		int iDir;
		int adjNode;	// �㉺���E��(adjacent)�̌�_
		int adjColor;	// �㉺���E��(adjacent)�̐΂̐F
		int invClr = INVCLR(color);//�������]
		int flgMove;	// �ړ����ʂ̎��

		if (g_board[node]) {
			// �΂����邩�A�g�Ȃ�
			//PRT(_T("�΂����邩�A�g�B \n"));
			score		= 0;
			flgAbort	= 1;
			goto gt_EndMethod;
		}

		if (node == g_kouNode) {
			// �R�E�ɂȂ�ʒu�Ȃ�
			//PRT(_T("�R�E�B \n"));
			score		= 0;
			flgAbort	= 1;
			goto gt_EndMethod;
		}

		score		= rand() % 100; // 0 �` 99 �̃����_���ȕ]���l��^����B
		flgCapture	= 0;
		safe		= 0;
		for (iDir = 0; iDir < 4; iDir++) {
			adjNode		= node + g_dir4[iDir];
			adjColor	= g_board[adjNode];
			if (adjColor == WAKU) {
				// �g�Ȃ�
				//PRT(_T("�g�B \n"));
				safe++;
			}
			if (adjColor == 0 || adjColor == WAKU) {
				// ����ۂ��A�g�Ȃ�B
				//PRT(_T("����ۂ��A�g�B \n"));
				continue;
			}

			// �ׂ̐΂̃��o�e�B�i�ċz�_�j�̐��𐔂��܂��B
			CountLiberty(adjNode);

			// �G�΂ŁA�ċz�_�̐��� 1 �Ȃ�A�����ɐ΂�u���Ǝ�邱�Ƃ��ł��܂��B
			if (adjColor == invClr && g_liberty == 1) {
				//PRT(_T("�G�΂�������B \n"));
				flgCapture = 1; 	// �G�΂��A������t���O�B
			}

			// �����ɐ΂�u���Ă��A�ċz�_�� 1 �ȏ�c��i�����S�ȁj�����ɂȂ���܂��B
			if (adjColor == color && 2 <= g_liberty) {
				safe++;
			}

			// �]���l�̌v�Z�i�S�������J��Ԃ��j
			score +=
				(adjColor == invClr)		// �ׂ̐�
											//		�����̐�: 0
											//		����̐�: 1
											//   �~
				* g_kakondaIshi				// �͂�Ŏ���΂̐�
											//   �~
				* (10 / (g_liberty + 1));	// �A�̌ċz�_�̌�
											//		0��: 10�_
											//		1��:  5�_
											//		2��:  3.3333...�_
											//		3��:  2.5�_
											//		4��:  2�_
											//		...
											//PRT(_T("�X�R�A=%d \n", score));
		}

		if (safe == 4) { // �l�����@�����̐΂�A�ǂɁ@�͂܂�Ă���ꏊ�i��j�ɂȂ�Ȃ�
			//PRT(_T("��ɂ͑ł��Ȃ��B \n"));
			// ��ɂ͑ł��Ȃ��B
			flgAbort = 1;
			goto gt_EndMethod;
		}

		if (flgCapture == 0) {		// �΂����Ȃ��ꍇ
									// ���ۂɒu���Ă݂ā@���E�肩�ǂ�������
			int temp_kouNode = g_kouNode;				// �R�E�̈ʒu��ޔ�
			flgMove = MoveOne(node, color);		// �R�E�̈ʒu���ς�邩���B
			g_board[node] = 0;
			g_kouNode = temp_kouNode;
			if (flgMove == MOVE_SUICIDE) {	// ���E��Ȃ�
				//PRT(_T("���E��͑ł��Ȃ��B \n"));
				// �x�X�g���[�u�ɂ͂Ȃ肦�Ȃ�
				flgAbort = 1;
				goto gt_EndMethod;
			}
		}

	gt_EndMethod:
		return score;
	}

}
