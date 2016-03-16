#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_150_liberty.h"
#include "../../header/h300_move____/n300_100_move.h"


int Move::MoveOne(
	HANDLE hConsoleWindow,
	int node,
	int color,
	Board* pBoard
	)
{
	int sum;
	int delNode		= 0;
	int tottaIshi	= 0;				// 取った石の合計
	int invClr		= INVCLR(color);	// 相手の石の色

	//----------------------------------------
	// パスの場合
	//----------------------------------------
	if (node == 0) {
		// 操作を受け付けます。
		pBoard->kouNode = 0;
		return MOVE_SUCCESS;
	}

	//----------------------------------------
	// コウに置こうとした場合
	//----------------------------------------
	if (node == pBoard->kouNode) {
		Core::PRT(hConsoleWindow, _T("move() Err: コウ！z=%04x\n"), node);
		// 操作を弾きます。
		return MOVE_KOU;
	}

	//----------------------------------------
	// 空点でないところに置こうとした場合
	//----------------------------------------
	if (pBoard->table[node] != 0) {
		Core::PRT(hConsoleWindow, _T("move() Err: 空点ではない！z=%04x\n"), node);
		// 操作を弾きます。
		return MOVE_EXIST;
	}

	pBoard->table[node] = color;	// とりあえず置いてみる

	// ここから下は、石を置いたあとの盤面です。

	// 自殺手になるかどうか判定するために、
	// また、新しい　コウ　を作るかどうか判定するために、
	// 相手の石を取るところまで進めます。
	pBoard->ForeachArroundNodes(node, [&pBoard, &tottaIshi, &delNode, color, invClr](int adjNode, bool& isBreak) {
		Liberty liberty1;

		if (pBoard->table[adjNode] != invClr) {
			// 隣接する石が　相手の石　でないなら無視。
			goto gt_Next1;
		}

		//----------------------------------------
		// 相手の石が取れるか判定します。
		//----------------------------------------

		// 隣接する石（連）の呼吸点を数えます。
		liberty1.Count(adjNode, pBoard);

		if (liberty1.liberty == 0) {
			// 呼吸点がないようなら、石（連）は取れます。

			// 囲んだ石の数を　ハマに加点。
			pBoard->hama[color - 1] += liberty1.renIshi;
			tottaIshi += liberty1.renIshi;
			delNode = adjNode;	// 取られた石の座標。コウの判定で使う。

								// 処理が被らないように、囲まれている相手の石（計算済み）を消します。
			pBoard->DeleteRenStones(adjNode, invClr);
		}

	gt_Next1:
		;
	});
	/*
	int iDir;
	int adjNode;						// 四方に隣接する交点
	for (iDir = 0; iDir < 4; iDir++) {
		adjNode = node + pBoard->dir4[iDir];

		if (pBoard->table[adjNode] != invClr) {
			// 隣接する石が　相手の石　でないなら無視。
			continue;
		}

		//----------------------------------------
		// 相手の石が取れるか判定します。
		//----------------------------------------

		// 隣接する石（連）の呼吸点を数えます。
		Liberty liberty;
		liberty.Count(adjNode, pBoard);

		if (liberty.liberty == 0) {
			// 呼吸点がないようなら、石（連）は取れます。

			// 囲んだ石の数を　ハマに加点。
			pBoard->hama[color - 1] += liberty.renIshi;
			tottaIshi += liberty.renIshi;
			delNode = adjNode;	// 取られた石の座標。コウの判定で使う。

			// 処理が被らないように、囲まれている相手の石（計算済み）を消します。
			pBoard->DeleteRenStones(adjNode, invClr);
		}
	}
	*/

	//----------------------------------------
	// 自殺手になるかを判定
	//----------------------------------------
	Liberty liberty;
	liberty.Count(node, pBoard);

	if (liberty.liberty == 0) {
		// 置いた石に呼吸点がない場合。

		// 操作を弾きます。
		Core::PRT(hConsoleWindow, _T("move() Err: 自殺手! z=%04x\n"), node);
		pBoard->table[node] = 0;
		return MOVE_SUICIDE;
	}

	//----------------------------------------
	// 次にコウになる位置を判定しておく。
	//----------------------------------------

	pBoard->kouNode = 0;	// コウではない

	// コウになるのは、石を1つだけ取った場合です。
	if (tottaIshi == 1) {
		// 取られた石の4方向に、自分の呼吸点が1個の連が1つだけある場合、その位置はコウ。
		pBoard->kouNode = delNode;	// 取り合えず取られた石の場所をコウの位置とする
		sum = 0;
		pBoard->ForeachArroundNodes(delNode, [&pBoard, &sum, color](int adjNode, bool& isBreak) {
			Liberty liberty2;

			if (pBoard->table[adjNode] != color) {
				goto gt_Next2;
			}

			liberty2.Count(adjNode, pBoard);
			if (liberty2.liberty == 1 && liberty2.renIshi == 1) {
				sum++;
			}

		gt_Next2:
			;
		});
		/*
		for (iDir = 0; iDir < 4; iDir++) {
			adjNode = delNode + pBoard->dir4[iDir];
			if (pBoard->table[adjNode] != color) {
				continue;
			}
			Liberty liberty;
			liberty.Count(adjNode, pBoard);
			if (liberty.liberty == 1 && liberty.renIshi == 1) {
				sum++;
			}
		}
		*/
		if (sum >= 2) {
			Core::PRT(hConsoleWindow, _T("１つ取られて、コウの位置へ打つと、１つの石を2つ以上取れる？node=%04x\n"), node);
			// これはエラー。

			// 操作を弾きます。
			return MOVE_FATAL;
		}
		if (sum == 0) {
			pBoard->kouNode = 0;	// コウにはならない。
		}
	}

	//----------------------------------------
	// 置くことに成功☆（＾▽＾）！
	//----------------------------------------

	// 操作を受け入れます。
	return MOVE_SUCCESS;
}
