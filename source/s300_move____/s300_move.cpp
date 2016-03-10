//
// /source/s300_move/s300_move.cpp
//
extern "C" {

	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h300_move____/h300_move.h"

	int MoveOne(
		int node,
		int color
		)
	{
		int i;
		int adjNode;			// 四方に隣接する交点
		int sum;
		int delNode = 0;
		int all_ishi = 0;	// 取った石の合計
		int un_col = INVCLR(color);

		if (node == 0) {		// PASSの場合
			g_kouNode = 0;
			return MOVE_SUCCESS;
		}
		if (node == g_kouNode) {
			PRT(_T("move() Err: コウ！z=%04x\n"), node);
			return MOVE_KOU;
		}
		if (g_board[node] != 0) {
			PRT(_T("move() Err: 空点ではない！z=%04x\n"), node);
			return MOVE_EXIST;
		}
		g_board[node] = color;	// とりあえず置いてみる

		for (i = 0; i < 4; i++) {
			adjNode = node + g_dir4[i];
			if (g_board[adjNode] != un_col) {
				continue;
			}
			// 敵の石が取れるか？
			CountLiberty(adjNode);
			if (g_liberty == 0) {
				g_hama[color - 1] += g_kakondaIshi;
				all_ishi += g_kakondaIshi;
				delNode = adjNode;	// 取られた石の座標。コウの判定で使う。
				DeleteStone(adjNode, un_col);
			}
		}
		// 自殺手を判定
		CountLiberty(node);
		if (g_liberty == 0) {
			PRT(_T("move() Err: 自殺手! z=%04x\n"), node);
			g_board[node] = 0;
			return MOVE_SUICIDE;
		}

		// 次にコウになる位置を判定。石を1つだけ取った場合。
		g_kouNode = 0;	// コウではない
		if (all_ishi == 1) {
			// 取られた石の4方向に自分のダメ1の連が1つだけある場合、その位置はコウ。
			g_kouNode = delNode;	// 取り合えず取られた石の場所をコウの位置とする
			sum = 0;
			for (i = 0; i < 4; i++) {
				adjNode = delNode + g_dir4[i];
				if (g_board[adjNode] != color) {
					continue;
				}
				CountLiberty(adjNode);
				if (g_liberty == 1 && g_kakondaIshi == 1) {
					sum++;
				}
			}
			if (sum >= 2) {
				PRT(_T("１つ取られて、コウの位置へ打つと、１つの石を2つ以上取れる？node=%04x\n"), node);
				return MOVE_FATAL;
			}
			if (sum == 0) {
				g_kouNode = 0;	// コウにはならない。
			}
		}
		return MOVE_SUCCESS;
	}


}