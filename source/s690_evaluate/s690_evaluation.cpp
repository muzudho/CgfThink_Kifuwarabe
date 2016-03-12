//
// /source/s690_evaluate/s690_evaluation.cpp
//

extern "C" {

	#include <windows.h>								// rand() 等を使用するために。
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
		NoHitEye	noHitEye;		// 自分の眼に打たない仕組み。
		NoHitMouth	noHitMouth;	// 相手の口に打たない仕組み。
		Suicide		suicide;
		int score;		// 読んでいる手の評価値
		int iDir;
		int adjNode;	// 上下左右隣(adjacent)の交点
		int adjColor;	// 上下左右隣(adjacent)の石の色
		int invClr = INVCLR(color);//白黒反転

		if (g_board[node]) {
			// 石があるか、枠なら
			//PRT(_T("石があるか、枠。 \n"));
			score		= 0;
			flgAbort	= 1;
			goto gt_EndMethod;
		}

		if (node == g_kouNode) {
			// コウになる位置なら
			//PRT(_T("コウ。 \n"));
			score		= 0;
			flgAbort	= 1;
			goto gt_EndMethod;
		}

		score			= rand() % 100; // 0 ～ 99 のランダムな評価値を与える。
		noHitEye.safe	= 0;
		//flgCapture		= 0;
		for (iDir = 0; iDir < 4; iDir++) {		// 上隣 → 右隣 → 下隣 → 左隣
			adjNode		= node + g_dir4[iDir];	// 隣接(adjacent)する交点と、
			adjColor	= g_board[adjNode];		// その色

			if (adjColor == WAKU) {
				// 枠なら
				//PRT(_T("枠。 \n"));
				noHitEye.safe++;
			}

			if (adjColor == 0 || adjColor == WAKU) {
				// 空っぽか、枠なら。
				//PRT(_T("空っぽか、枠。 \n"));
				continue;
			}

			// 相手の口に石を打ち込む状況でないか調査。
			noHitMouth.IsThis(adjColor, invClr);

			// 隣の石（または連）の呼吸点　の数を数えます。
			CountLiberty(adjNode);

			// 自殺手になる状況でないか調査。
			suicide.IsThis(adjColor, invClr);

			// 自分の眼に打ち込む状況か調査
			noHitEye.IsThis(color, adjColor);

			// 評価値の計算（４方向分繰り返す）
			score +=
				(adjColor == invClr)		// 隣の石
											//		自分の石: 0
											//		相手の石: 1
											//   ×
				* g_kakondaIshi				// 連の石の数
											//   ×
				* (10 / (g_liberty + 1));	// 連の呼吸点の個数
											//		0個: 10点
											//		1個:  5点
											//		2個:  3.3333...点
											//		3個:  2.5点
											//		4個:  2点
											//		...
											//PRT(_T("スコア=%d \n", score));
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
			// 3方向が相手の石のところで
			// 駒も取れないところには、打ち込みたくない。
		}
		else
		{
			// それ以外の点を、大幅に加点。
			score += 50;
		}

		// 2016-03-12 16:45 Add
		if (score < 0)
		{
			// スコアは 0点以下（パス）にはしません。
			score = 0;
		}

	gt_EndMethod:
		return score;
	}

}
