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
	int invColor = INVCLR(color);	//白黒反転
	NoHitSuicide			noHitSuicide;	// 自殺手を打たないようにする仕組み。
	NoHitOwnEye				noHitOwnEye;	// 自分の眼に打たない仕組み。
	NoHitMouth				noHitMouth;		// 相手の口に打たない仕組み。
	NoHitHasinohoBocchi		noHitHasinoho;	// 端の方には、ぼっち石　を、あまり打たないようにする仕組み。
	HitRandom				hitRandom;		// 手をばらけさせる仕組み。
	HitTuke					hitTuke;		// 相手の石に積極的にツケるようにする仕組み。
	HitAte					hitAte;			// アタリに積極的にアテるようにする仕組み。
	int score = 0;					// 読んでいる手の評価値


	int x, y;
	AbstractBoard::ConvertToXy(x, y, node);
	int libertyOfRen = pLibertyOfNodes->ValueOf(node);
	core.PRT(_T("(%d,%d) LibRen=%d スコア="), x, y, libertyOfRen);


	if (pBoard->ValueOf(node)) {
		// 石があるか、枠なら
		core.PRT(_T("石があるか、枠。 \n"));
		flgAbort	= 1;
		goto gt_EndMethod;
	}

	if (node == pBoard->kouNode) {
		// コウになる位置なら
		core.PRT(_T("コウ。 \n"));
		flgAbort	= 1;
		goto gt_EndMethod;
	}

	int nHitRandom = hitRandom.Evaluate(); // 0 ～ 99 のランダムな評価値を与える。

	noHitMouth.Research(color, node, pBoard);		// 相手の口に石を打ち込む状況でないか調査。


	Liberty liberties[4];// 上隣 → 右隣 → 下隣 → 左隣
	pBoard->ForeachArroundDirAndNodes(node, [&pBoard, &liberties](int iDir, int adjNode, bool& isBreak) {
		int adjColor = pBoard->ValueOf(adjNode);		// 上下左右隣(adjacent)の石の色

		liberties[iDir].Count(adjNode, pBoard);						// 隣の石（または連）の呼吸点　の数を数えます。
	});

	// ツケるかどうかを評価
	int nTuke = hitTuke.Evaluate(invColor, node, liberties, pBoard);

	// アテるかどうかを評価
	int nAte = hitAte.Evaluate(core, color, node, pBoard, pLibertyOfNodes);

	if (
		noHitOwnEye.IsThis(color, node, liberties, pBoard)		||		// 自分の眼に打ち込む状況か調査
		noHitSuicide.IsThis(core, color, node, liberties, pBoard)			// 自殺手になる状況でないか調査。
	) {
		core.PRT(_T("自分の眼、または自殺手を回避。 \n"));
		flgAbort = 1;
		goto gt_EndMethod;
	}

	// 2016-03-12 16:45 Add
	int nNoHitMouth = noHitMouth.Evaluate(noHitSuicide.flgCapture);

	// 2016-03-15 00:57 Add
	noHitHasinoho.Research(node, pBoard);
	int nNoHitHasinoho = noHitHasinoho.Evaluate();

	//----------------------------------------
	// 集計
	//----------------------------------------

	// ばらしたい
	core.PRT(_T("%d,"), nHitRandom);
	score += nHitRandom;

	// マウスに打ちたくない
	core.PRT(_T("%d,"), nNoHitMouth);
	score += nNoHitMouth;

	// ツケたい
	core.PRT(_T("%d,"), nTuke);
	score += nTuke;

	// アテたい
	core.PRT(_T("%d,"), nAte);
	score += nAte;

	// 端の方に打ちたくない
	core.PRT(_T("%d,"), nNoHitHasinoho);
	score += nNoHitHasinoho;

	core.PRT(_T("[%d] \n"), score);

gt_EndMethod:
	return score;
}


