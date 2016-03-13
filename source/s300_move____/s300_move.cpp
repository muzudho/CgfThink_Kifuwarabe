//
// /source/s300_move/s300_move.cpp
//
extern "C" {

	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h200_research/h200_100_Liberty.h"
	#include "../../header/h300_move____/h300_move.h"

	int MoveOne(
		int node,
		int color
		)
	{
		int i;
		int adjNode;						// 四方に隣接する交点
		int sum;
		int delNode		= 0;
		int tottaIshi	= 0;				// 取った石の合計
		int invClr		= INVCLR(color);	// 相手の石の色

		//----------------------------------------
		// パスの場合
		//----------------------------------------
		if (node == 0) {
			// 操作を受け付けます。
			g_kouNode = 0;
			return MOVE_SUCCESS;
		}

		//----------------------------------------
		// コウに置こうとした場合
		//----------------------------------------
		if (node == g_kouNode) {
			PRT(_T("move() Err: コウ！z=%04x\n"), node);
			// 操作を弾きます。
			return MOVE_KOU;
		}

		//----------------------------------------
		// 空点でないところに置こうとした場合
		//----------------------------------------
		if (g_board[node] != 0) {
			PRT(_T("move() Err: 空点ではない！z=%04x\n"), node);
			// 操作を弾きます。
			return MOVE_EXIST;
		}

		g_board[node] = color;	// とりあえず置いてみる

		// ここから下は、石を置いたあとの盤面です。

		// 自殺手になるかどうか判定するために、
		// また、新しい　コウ　を作るかどうか判定するために、
		// 相手の石を取るところまで進めます。
		for (i = 0; i < 4; i++) {
			adjNode = node + g_dir4[i];

			if (g_board[adjNode] != invClr) {
				// 隣接する石が　相手の石　でないなら無視。
				continue;
			}

			//----------------------------------------
			// 相手の石が取れるか判定します。
			//----------------------------------------

			// 隣接する石（連）の呼吸点を数えます。
			Liberty liberty;
			liberty.Count(adjNode);

			if (liberty.liberty == 0) {
				// 呼吸点がないようなら、石（連）は取れます。

				// 囲んだ石の数を　ハマに加点。
				g_hama[color - 1] += liberty.renIshi;
				tottaIshi += liberty.renIshi;
				delNode = adjNode;	// 取られた石の座標。コウの判定で使う。

				// 処理が被らないように、囲まれている相手の石（計算済み）を消します。
				DeleteRenStones(adjNode, invClr);
			}
		}

		//----------------------------------------
		// 自殺手になるかを判定
		//----------------------------------------
		Liberty liberty;
		liberty.Count(node);

		if (liberty.liberty == 0) {
			// 置いた石に呼吸点がない場合。

			// 操作を弾きます。
			PRT(_T("move() Err: 自殺手! z=%04x\n"), node);
			g_board[node] = 0;
			return MOVE_SUICIDE;
		}

		//----------------------------------------
		// 次にコウになる位置を判定しておく。
		//----------------------------------------

		g_kouNode = 0;	// コウではない

		// コウになるのは、石を1つだけ取った場合です。
		if (tottaIshi == 1) {
			// 取られた石の4方向に、自分の呼吸点が1個の連が1つだけある場合、その位置はコウ。
			g_kouNode = delNode;	// 取り合えず取られた石の場所をコウの位置とする
			sum = 0;
			for (i = 0; i < 4; i++) {
				adjNode = delNode + g_dir4[i];
				if (g_board[adjNode] != color) {
					continue;
				}
				Liberty liberty;
				liberty.Count(adjNode);
				if (liberty.liberty == 1 && liberty.renIshi == 1) {
					sum++;
				}
			}
			if (sum >= 2) {
				PRT(_T("１つ取られて、コウの位置へ打つと、１つの石を2つ以上取れる？node=%04x\n"), node);
				// これはエラー。

				// 操作を弾きます。
				return MOVE_FATAL;
			}
			if (sum == 0) {
				g_kouNode = 0;	// コウにはならない。
			}
		}

		//----------------------------------------
		// 置くことに成功☆（＾▽＾）！
		//----------------------------------------

		// 操作を受け入れます。
		return MOVE_SUCCESS;
	}


}