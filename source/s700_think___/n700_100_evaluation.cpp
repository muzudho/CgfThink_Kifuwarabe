#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_200_libertyOfNodes.h"
#include "../../header/h300_move____/n300_100_move.h"
#include "../../header/h670_noHit___/n670_050_noHitSuicide.h"
#include "../../header/h670_noHit___/n670_100_noHitOwnEye.h"
#include "../../header/h670_noHit___/n670_110_noHitMouth.h"
#include "../../header/h670_noHit___/n670_120_noHitHasinohoBocchi.h"
#include "../../header/h675_hit_____/n675_050_hitRandom.h"
#include "../../header/h675_hit_____/n675_100_hitTuke.h"
#include "../../header/h675_hit_____/n675_150_hitAte.h"
#include "../../header/h700_think___/n700_100_evaluation.h"


int Evaluation::EvaluateAtNode(
	Core			core			,
	int&			flgAbort		,
	int				color			,
	int				node			,
	Board*			pBoard			,
	LibertyOfNodes* pLibertyOfNodes
)
{
	int invColor = INVCLR(color);	//�������]
	NoHitSuicide			noHitSuicide;	// ���E���ł��Ȃ��悤�ɂ���d�g�݁B
	NoHitOwnEye				noHitOwnEye;	// �����̊�ɑł��Ȃ��d�g�݁B
	NoHitMouth				noHitMouth;		// ����̌��ɑł��Ȃ��d�g�݁B
	NoHitHasinohoBocchi		noHitHasinoho;	// �[�̕��ɂ́A�ڂ����΁@���A���܂�ł��Ȃ��悤�ɂ���d�g�݁B
	HitRandom				hitRandom;		// ����΂炯������d�g�݁B
	HitTuke					hitTuke;		// ����̐΂ɐϋɓI�Ƀc�P��悤�ɂ���d�g�݁B
	HitAte					hitAte;			// �A�^���ɐϋɓI�ɃA�e��悤�ɂ���d�g�݁B
	int score = 0;					// �ǂ�ł����̕]���l


	int x, y;
	AbstractBoard::ConvertToXy(x, y, node);
	int libertyOfRen = pLibertyOfNodes->ValueOf(node);
	core.PRT(_T("(%d,%d) LibRen=%d �X�R�A="), x, y, libertyOfRen);


	if (pBoard->ValueOf(node)) {
		// �΂����邩�A�g�Ȃ�
		core.PRT(_T("�΂����邩�A�g�B \n"));
		flgAbort	= 1;
		goto gt_EndMethod;
	}

	if (node == pBoard->kouNode) {
		// �R�E�ɂȂ�ʒu�Ȃ�
		core.PRT(_T("�R�E�B \n"));
		flgAbort	= 1;
		goto gt_EndMethod;
	}

	int nHitRandom = hitRandom.Evaluate(); // 0 �` 99 �̃����_���ȕ]���l��^����B

	noHitMouth.Research(color, node, pBoard);		// ����̌��ɐ΂�ł����ޏ󋵂łȂ��������B


	Liberty liberties[4];// ��� �� �E�� �� ���� �� ����
	pBoard->ForeachArroundDirAndNodes(node, [&pBoard, &liberties](int iDir, int adjNode, bool& isBreak) {
		int adjColor = pBoard->ValueOf(adjNode);		// �㉺���E��(adjacent)�̐΂̐F

		liberties[iDir].Count(adjNode, pBoard);						// �ׂ̐΁i�܂��͘A�j�̌ċz�_�@�̐��𐔂��܂��B
	});

	// �c�P�邩�ǂ�����]��
	int nTuke = hitTuke.Evaluate(invColor, node, liberties, pBoard);

	// �A�e�邩�ǂ�����]��
	int nAte = hitAte.Evaluate(core, color, node, pBoard, pLibertyOfNodes);

	if (
		noHitOwnEye.IsThis(color, node, liberties, pBoard)		||		// �����̊�ɑł����ޏ󋵂�����
		noHitSuicide.IsThis(core, color, node, liberties, pBoard)			// ���E��ɂȂ�󋵂łȂ��������B
	) {
		core.PRT(_T("�����̊�A�܂��͎��E�������B \n"));
		flgAbort = 1;
		goto gt_EndMethod;
	}

	// 2016-03-12 16:45 Add
	int nNoHitMouth = noHitMouth.Evaluate(noHitSuicide.flgCapture);

	// 2016-03-15 00:57 Add
	noHitHasinoho.Research(node, pBoard);
	int nNoHitHasinoho = noHitHasinoho.Evaluate();

	//----------------------------------------
	// �W�v
	//----------------------------------------

	// �΂炵����
	core.PRT(_T("%d,"), nHitRandom);
	score += nHitRandom;

	// �}�E�X�ɑł������Ȃ�
	core.PRT(_T("%d,"), nNoHitMouth);
	score += nNoHitMouth;

	// �c�P����
	core.PRT(_T("%d,"), nTuke);
	score += nTuke;

	// �A�e����
	core.PRT(_T("%d,"), nAte);
	score += nAte;

	// �[�̕��ɑł������Ȃ�
	core.PRT(_T("%d,"), nNoHitHasinoho);
	score += nNoHitHasinoho;

	core.PRT(_T("[%d] \n"), score);

gt_EndMethod:
	return score;
}


