//
// /source/s690_evaluate/s690_evaluation.cpp
//

extern "C" {

	#include <windows.h>								// rand() �����g�p���邽�߂ɁB
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h300_move____/h300_move.h"
	#include "../../header/h670_noHit___/h670_100_noHitEye.h"
	#include "../../header/h670_noHit___/h670_110_noHitMouth.h"
	#include "../../header/h680_suicide_/h680_suicide.h"


	int Evaluate(
		int&	flgAbort	,
		int		color		,
		int		node
	)
	{
		NoHitEye	noHitEye;		// �����̊�ɑł��Ȃ��d�g�݁B
		NoHitMouth	noHitMouth;	// ����̌��ɑł��Ȃ��d�g�݁B
		Suicide		suicide;
		int score;		// �ǂ�ł����̕]���l
		int iDir;
		int adjNode;	// �㉺���E��(adjacent)�̌�_
		int adjColor;	// �㉺���E��(adjacent)�̐΂̐F
		int invClr = INVCLR(color);//�������]

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

		score			= rand() % 100; // 0 �` 99 �̃����_���ȕ]���l��^����B
		noHitEye.safe	= 0;
		//flgCapture		= 0;
		for (iDir = 0; iDir < 4; iDir++) {		// ��� �� �E�� �� ���� �� ����
			adjNode		= node + g_dir4[iDir];	// �א�(adjacent)�����_�ƁA
			adjColor	= g_board[adjNode];		// ���̐F

			if (adjColor == WAKU) {
				// �g�Ȃ�
				//PRT(_T("�g�B \n"));
				noHitEye.safe++;
			}

			if (adjColor == 0 || adjColor == WAKU) {
				// ����ۂ��A�g�Ȃ�B
				//PRT(_T("����ۂ��A�g�B \n"));
				continue;
			}

			// ����̌��ɐ΂�ł����ޏ󋵂łȂ��������B
			noHitMouth.IsThis(adjColor, invClr);

			// �ׂ̐΁i�܂��͘A�j�̌ċz�_�@�̐��𐔂��܂��B
			CountLiberty(adjNode);

			// ���E��ɂȂ�󋵂łȂ��������B
			suicide.IsThis(adjColor, invClr);

			// �����̊�ɑł����ޏ󋵂�����
			noHitEye.IsThis(color, adjColor);

			// �]���l�̌v�Z�i�S�������J��Ԃ��j
			score +=
				(adjColor == invClr)		// �ׂ̐�
											//		�����̐�: 0
											//		����̐�: 1
											//   �~
				* g_kakondaIshi				// �A�̐΂̐�
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

		noHitEye.Judge(flgAbort);
		if (flgAbort) {
			goto gt_EndMethod;
		}

		suicide.Judge(flgAbort, color, node);
		if (flgAbort) {
			goto gt_EndMethod;
		}

		// 2016-03-12 16:45 Add
		if (noHitMouth.adjOppo == 3 && suicide.flgCapture)
		{
			// 3����������̐΂̂Ƃ����
			// ������Ȃ��Ƃ���ɂ́A�ł����݂����Ȃ��B
		}
		else
		{
			// ����ȊO�̓_���A�啝�ɉ��_�B
			score += 50;
		}

		// 2016-03-12 16:45 Add
		if (score < 0)
		{
			// �X�R�A�� 0�_�ȉ��i�p�X�j�ɂ͂��܂���B
			score = 0;
		}

	gt_EndMethod:
		return score;
	}

}
