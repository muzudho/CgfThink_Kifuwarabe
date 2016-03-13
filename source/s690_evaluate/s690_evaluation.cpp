//
// /source/s690_evaluate/s690_evaluation.cpp
//

extern "C" {

	//#include <windows.h>								// rand() �����g�p���邽�߂ɁB
	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h300_move____/h300_move.h"
	#include "../../header/h670_noHit___/h670_050_noHitSuicide.h"
	#include "../../header/h670_noHit___/h670_100_noHitOwnEye.h"
	#include "../../header/h670_noHit___/h670_110_noHitMouth.h"
	#include "../../header/h675_hit_____/h675_050_hitRandom.h"
	#include "../../header/h675_hit_____/h675_100_hitTuke.h"


	int Evaluate(
		int&	flgAbort	,
		int		color		,
		int		node
	)
	{
		int invColor = INVCLR(color);	//�������]
		NoHitOwnEye		noHitOwnEye;	// �����̊�ɑł��Ȃ��d�g�݁B
		NoHitMouth		noHitMouth;		// ����̌��ɑł��Ȃ��d�g�݁B
		NoHitSuicide	noHitSuicide;	// ���E���ł��Ȃ��悤�ɂ���d�g�݁B
		HitRandom		hitRandom;		// ����΂炯������d�g�݁B
		HitTuke			hitTuke;		// ����̐΂ɐϋɓI�Ƀc�P��悤�ɂ���d�g�݁B
		int score = 0;					// �ǂ�ł����̕]���l
		int iDir;
		int adjNode;	// �㉺���E��(adjacent)�̌�_
		int adjColor;	// �㉺���E��(adjacent)�̐΂̐F

		if (g_board[node]) {
			// �΂����邩�A�g�Ȃ�
			//PRT(_T("�΂����邩�A�g�B \n"));
			flgAbort	= 1;
			goto gt_EndMethod;
		}

		if (node == g_kouNode) {
			// �R�E�ɂȂ�ʒu�Ȃ�
			//PRT(_T("�R�E�B \n"));
			flgAbort	= 1;
			goto gt_EndMethod;
		}

		score = hitRandom.Evaluate_AdjNode(); // 0 �` 99 �̃����_���ȕ]���l��^����B

		noHitOwnEye.Research(color, node);		// �����̊�ɑł����ޏ󋵂�����
		noHitMouth.Research(color, node);// ����̌��ɐ΂�ł����ޏ󋵂łȂ��������B
		noHitSuicide.Research(invColor, node);	// ���E��ɂȂ�󋵂łȂ��������B

		for (iDir = 0; iDir < 4; iDir++) {		// ��� �� �E�� �� ���� �� ����
			adjNode		= node + g_dir4[iDir];	// �א�(adjacent)�����_�ƁA
			adjColor	= g_board[adjNode];		// ���̐F

			//----------------------------------------

			// �]���l�̌v�Z�i�S�������J��Ԃ��j
			score += hitTuke.Evaluate_AdjNode(invColor, adjColor);
		}

		if (
			noHitOwnEye.DontHit() ||
			noHitSuicide.DontHit(color, node)
		) {
			flgAbort = 1;
			goto gt_EndMethod;
		}

		// 2016-03-12 16:45 Add
		score += noHitMouth.Evaluate(noHitSuicide.flgCapture);

		PRT(_T("�m�[�h=%x "), node);
		PRT(_T("�X�R�A=%d "), score);
		//PRT(_T("noHitMouth.adjOppo=%d "), noHitMouth.adjOppo);
		//PRT(_T("noHitMouth.Evaluate=%d "), noHitMouth.Evaluate(noHitSuicide.flgCapture));
		PRT(_T("\n"));

	gt_EndMethod:
		return score;
	}

}
