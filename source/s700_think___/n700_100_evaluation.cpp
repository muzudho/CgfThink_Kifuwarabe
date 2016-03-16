#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h300_move____/n300_100_move.h"
#include "../../header/h670_noHit___/n670_050_noHitSuicide.h"
#include "../../header/h670_noHit___/n670_100_noHitOwnEye.h"
#include "../../header/h670_noHit___/n670_110_noHitMouth.h"
#include "../../header/h670_noHit___/n670_120_noHitHasinohoBocchi.h"
#include "../../header/h675_hit_____/n675_050_hitRandom.h"
#include "../../header/h675_hit_____/n675_100_hitTuke.h"
#include "../../header/h700_think___/n700_100_evaluation.h"


int Evaluation::Evaluate(
	HANDLE	hConsoleWindow,
	int&	flgAbort	,
	int		color		,
	int		node        ,
	Board*  pBoard
)
{
	int invColor = INVCLR(color);	//白黒反転
	NoHitSuicide			noHitSuicide;	// 自殺手を打たないようにする仕組み。
	NoHitOwnEye				noHitOwnEye;	// 自分の眼に打たない仕組み。
	NoHitMouth				noHitMouth;		// 相手の口に打たない仕組み。
	NoHitHasinohoBocchi		noHitHasinoho;	// 端の方には、ぼっち石　を、あまり打たないようにする仕組み。
	HitRandom				hitRandom;		// 手をばらけさせる仕組み。
	HitTuke					hitTuke;		// 相手の石に積極的にツケるようにする仕組み。
	int score = 0;					// 読んでいる手の評価値

	if (pBoard->table[node]) {
		// 石があるか、枠なら
		//PRT(_T("石があるか、枠。 \n"));
		flgAbort	= 1;
		goto gt_EndMethod;
	}

	if (node == pBoard->kouNode) {
		// コウになる位置なら
		//PRT(_T("コウ。 \n"));
		flgAbort	= 1;
		goto gt_EndMethod;
	}

	score = hitRandom.Evaluate_AdjNode(); // 0 ～ 99 のランダムな評価値を与える。

	noHitMouth.Research(color, node, pBoard);		// 相手の口に石を打ち込む状況でないか調査。


	Liberty liberties[4];// 上隣 → 右隣 → 下隣 → 左隣
	pBoard->ForeachArroundDirAndNodes(node, [&pBoard, &liberties](int iDir, int adjNode, bool& isBreak) {
		int adjColor = pBoard->table[adjNode];		// 上下左右隣(adjacent)の石の色

		liberties[iDir].Count(adjNode, pBoard);						// 隣の石（または連）の呼吸点　の数を数えます。
	});
	/*
	int iDir;
	int adjNode;	// 上下左右隣(adjacent)の交点
	int adjColor;	// 上下左右隣(adjacent)の石の色
	for (iDir = 0; iDir < 4; iDir++) {		// 上隣 → 右隣 → 下隣 → 左隣
		adjNode = node + pBoard->dir4[iDir];	// 隣接(adjacent)する交点と、
		adjColor = pBoard->table[adjNode];		// その色

		liberties[iDir].Count(adjNode, pBoard);						// 隣の石（または連）の呼吸点　の数を数えます。
	}
	*/

	// 評価値の計算（４方向分）
	score += hitTuke.Evaluate(invColor, node, liberties, pBoard);

	if (
		noHitOwnEye.IsThis(color, node, liberties, pBoard)		||		// 自分の眼に打ち込む状況か調査
		noHitSuicide.IsThis(hConsoleWindow, color, node, liberties, pBoard)			// 自殺手になる状況でないか調査。
	) {
		flgAbort = 1;
		goto gt_EndMethod;
	}

	// 2016-03-12 16:45 Add
	score += noHitMouth.Evaluate(noHitSuicide.flgCapture);

	// 2016-03-15 00:57 Add
	noHitHasinoho.Research(node, pBoard);
	score += noHitHasinoho.Evaluate();

	Core::PRT(hConsoleWindow, _T("ノード=%x "), node);
	Core::PRT(hConsoleWindow, _T("スコア=%d "), score);
	//PRT(_T("noHitMouth.adjOppo=%d "), noHitMouth.adjOppo);
	//PRT(_T("noHitMouth.Evaluate=%d "), noHitMouth.Evaluate(noHitSuicide.flgCapture));
	Core::PRT(hConsoleWindow, _T("\n"));

gt_EndMethod:
	return score;
}


