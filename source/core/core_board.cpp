//
// /source/core/board.cpp
//
extern "C" {

	#include "../../header/core/core_board.h"
	#include "../../header/think.h"

	// 位置 tz におけるダメの数と石の数を計算。結果はグローバル変数に。
	void count_dame(int tz)
	{
		int i;

		g_dame = g_ishi = 0;
		for (i = 0; i < BOARD_MAX; i++) g_checkedBoard[i] = 0;
		count_dame_sub(tz, g_board[tz]);
	}

	// ダメと石の数える再帰関数
	// 4方向を調べて、空白だったら+1、自分の石なら再帰で。相手の石、壁ならそのまま。
	void count_dame_sub(int tNode, int col)
	{
		int node;
		int i;

		g_checkedBoard[tNode] = 1;			// この石は検索済み	
		g_ishi++;							// 石の数
		for (i = 0; i < 4; i++) {
			node = tNode + g_dir4[i];
			if (g_checkedBoard[node]) continue;
			if (g_board[node] == 0) {
				g_checkedBoard[node] = 1;	// この空点は検索済み
				g_dame++;				// ダメの数
			}
			if (g_board[node] == col) count_dame_sub(node, col);	// 未探索の自分の石
		}
	}

	// (x,y)を1つの座標に変換
	int ConvertNode(int x, int y)
	{
		return (y + 1) * 256 + (x + 1);
	}

	// 手を進める。
	int move_one(
		int node,	// 座標
		int col			// 石の色
		)
	{
		int i;
		int z1;
		int sum;
		int delNode = 0;
		int all_ishi = 0;	// 取った石の合計
		int un_col = UNCOL(col);

		if (node == 0) {	// PASSの場合
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
		g_board[node] = col;	// とりあえず置いてみる

		for (i = 0; i < 4; i++) {
			z1 = node + g_dir4[i];
			if (g_board[z1] != un_col) continue;
			// 敵の石が取れるか？
			count_dame(z1);
			if (g_dame == 0) {
				g_hama[col - 1] += g_ishi;
				all_ishi += g_ishi;
				delNode = z1;	// 取られた石の座標。コウの判定で使う。
				del_stone(z1, un_col);
			}
		}
		// 自殺手を判定
		count_dame(node);
		if (g_dame == 0) {
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
				z1 = delNode + g_dir4[i];
				if (g_board[z1] != col) continue;
				count_dame(z1);
				if (g_dame == 1 && g_ishi == 1) sum++;
			}
			if (sum >= 2) {
				PRT(_T("１つ取られて、コウの位置へ打つと、１つの石を2つ以上取れる？z=%04x\n"), node);
				return MOVE_FATAL;
			}
			if (sum == 0) g_kouNode = 0;	// コウにはならない。
		}
		return MOVE_SUCCESS;
	}

	// 石を消す
	void del_stone(int tNode, int col)
	{
		int node;
		int i;

		g_board[tNode] = 0;
		for (i = 0; i < 4; i++) {
			node = tNode + g_dir4[i];
			if (g_board[node] == col) del_stone(node, col);
		}
	}

}
