//
// /source/s690_evaluate/s690_evaluation.cpp
//

extern "C" {

	//#include <windows.h>								// rand() 等を使用するために。
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
		int invColor = INVCLR(color);	//白黒反転
		NoHitOwnEye		noHitOwnEye;	// 自分の眼に打たない仕組み。
		NoHitMouth		noHitMouth;		// 相手の口に打たない仕組み。
		NoHitSuicide	noHitSuicide;	// 自殺手を打たないようにする仕組み。
		HitRandom		hitRandom;		// 手をばらけさせる仕組み。
		HitTuke			hitTuke;		// 相手の石に積極的にツケるようにする仕組み。
		int score = 0;					// 読んでいる手の評価値
		int iDir;
		int adjNode;	// 上下左右隣(adjacent)の交点
		int adjColor;	// 上下左右隣(adjacent)の石の色

		if (g_board[node]) {
			// 石があるか、枠なら
			//PRT(_T("石があるか、枠。 \n"));
			flgAbort	= 1;
			goto gt_EndMethod;
		}

		if (node == g_kouNode) {
			// コウになる位置なら
			//PRT(_T("コウ。 \n"));
			flgAbort	= 1;
			goto gt_EndMethod;
		}

		score = hitRandom.Evaluate_AdjNode(); // 0 ～ 99 のランダムな評価値を与える。

		noHitOwnEye.Research(color, node);		// 自分の眼に打ち込む状況か調査
		noHitMouth.Research(color, node);// 相手の口に石を打ち込む状況でないか調査。
		noHitSuicide.Research(invColor, node);	// 自殺手になる状況でないか調査。

		for (iDir = 0; iDir < 4; iDir++) {		// 上隣 → 右隣 → 下隣 → 左隣
			adjNode		= node + g_dir4[iDir];	// 隣接(adjacent)する交点と、
			adjColor	= g_board[adjNode];		// その色

			//----------------------------------------

			// 評価値の計算（４方向分繰り返す）
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

		PRT(_T("ノード=%x "), node);
		PRT(_T("スコア=%d "), score);
		//PRT(_T("noHitMouth.adjOppo=%d "), noHitMouth.adjOppo);
		//PRT(_T("noHitMouth.Evaluate=%d "), noHitMouth.Evaluate(noHitSuicide.flgCapture));
		PRT(_T("\n"));

	gt_EndMethod:
		return score;
	}

}
