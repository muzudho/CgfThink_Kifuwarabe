//
// /source/s690_evaluate/s690_evaluation.cpp
//

extern "C" {

	#include <windows.h>								// rand() 等を使用するために。
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h300_move____/h300_move.h"


	int Evaluate(
		int&	flgAbort	,
		int		color		,
		int		node
	)
	{
		int score;		// 読んでいる手の評価値
		int flgCapture;	// 敵石を取ったフラグ
		int safe;
		int iDir;
		int adjNode;	// 上下左右隣(adjacent)の交点
		int adjColor;	// 上下左右隣(adjacent)の石の色
		int invClr = INVCLR(color);//白黒反転
		int flgMove;	// 移動結果の種類

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

		score		= rand() % 100; // 0 ～ 99 のランダムな評価値を与える。
		safe		= 0;
		flgCapture	= 0;
		for (iDir = 0; iDir < 4; iDir++) {		// 上隣 → 右隣 → 下隣 → 左隣
			adjNode		= node + g_dir4[iDir];	// 隣接(adjacent)する交点と、
			adjColor	= g_board[adjNode];		// その色

			if (adjColor == WAKU) {
				// 枠なら
				//PRT(_T("枠。 \n"));
				safe++;
			}

			if (adjColor == 0 || adjColor == WAKU) {
				// 空っぽか、枠なら。
				//PRT(_T("空っぽか、枠。 \n"));
				continue;
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
				safe++;
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

		if (safe == 4) { // 四方が　自分の石や、壁に　囲まれている場所（眼）になるなら
			//PRT(_T("眼には打たない。 \n"));
			// 眼には打たない。
			flgAbort = 1;
			goto gt_EndMethod;
		}

		if (flgCapture == 0) {					// 石が取れない場合
												// 実際に置いてみて　自殺手かどうか判定
			int temp_kouNode = g_kouNode;		// コウの位置を退避

			flgMove = MoveOne(node, color);		// 石を置きます。コウの位置が変わるかも。

			// 石を置く前の状態に戻します。
			g_board[node] = 0;					// 置いた石を消します。
			g_kouNode = temp_kouNode;			// コウの位置を元に戻します。

			if (flgMove == MOVE_SUICIDE) {		// 自殺手なら
				//PRT(_T("自殺手は打たない。 \n"));
				// ベストムーブにはなりえない
				flgAbort = 1;
				goto gt_EndMethod;
			}
		}

	gt_EndMethod:
		return score;
	}

}
