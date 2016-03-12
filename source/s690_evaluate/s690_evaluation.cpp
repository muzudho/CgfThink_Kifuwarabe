//
// /source/s690_evaluate/s690_evaluation.cpp
//

extern "C" {

	#include <windows.h>								// rand() 等を使用するために。
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h300_move____/h300_move.h"
	#include "../../header/h670_noHitEye/h670_noHitEye.h"
	#include "../../header/h680_suicide_/h680_suicide.h"


	int Evaluate(
		int&	flgAbort	,
		int		color		,
		int		node
	)
	{
		NoHitEye noHitEye;	// 自分の眼に打たない仕組み。
		Suicide suicide;
		int score;		// 読んでいる手の評価値
		int flgCapture;	// 敵石を取ったフラグ
		int iDir;
		int adjNode;	// 上下左右隣(adjacent)の交点
		int adjColor;	// 上下左右隣(adjacent)の石の色
		int invClr = INVCLR(color);//白黒反転
		int adjOppo = 0;	// 上下左右に隣接(adjacent)する相手(opponent)の石の数。

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
		flgCapture		= 0;
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

			// 2016-03-12 16:45 Add
			// 隣が相手の石、または枠ならカウントアップ。
			if (adjColor == invClr || adjColor == WAKU) {
				adjOppo++;
			}

			// 隣の石（または連）の呼吸点　の数を数えます。
			CountLiberty(adjNode);

			// 隣に、呼吸点が 1 個の相手の石があれば、それは取ることができます。
			if (adjColor == invClr && g_liberty == 1) {
				//PRT(_T("敵石を取った。 \n"));
				flgCapture = 1; 	// 敵石を、取ったフラグ。
			}

			// 隣に、呼吸点が 2 以上ある（＝石を置いても呼吸点が 1 以上残る、
			// 自殺手にはならない）味方につながります。
			if (adjColor == color && 2 <= g_liberty) {
				noHitEye.safe++;
			}

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

		if (noHitEye.safe == 4) { // 四方が　自分の石や、壁に　囲まれている場所（眼）になるなら
			//PRT(_T("眼には打たない。 \n"));
			// 眼には打たない。
			flgAbort = 1;
			goto gt_EndMethod;
		}

		JudgeSuicide(
			flgAbort,
			flgCapture,
			color,
			node
			);
		if (flgAbort) {
			goto gt_EndMethod;
		}

		// 2016-03-12 16:45 Add
		if (adjOppo == 3 && flgCapture)
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
