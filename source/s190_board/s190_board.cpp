//
// /source/core/board.cpp
//
extern "C" {

	#include "../../header/h190_board/h190_board.h"
	#include "../../header/h400_core/h400_board.h"
	#include "../../header/h400_core/h400_ui.h"
	#include "../../header/think.h"


	//--------------------------------------------------------------------------------
	// グローバル変数
	//--------------------------------------------------------------------------------

	int g_board[BOARD_MAX];
	int g_boardSize;


	//--------------------------------------------------------------------------------
	// 関数
	//--------------------------------------------------------------------------------

	void CountLiberty(int tNode)
	{
		int i;

		g_liberty = g_kakondaIshi = 0;
		for (i = 0; i < BOARD_MAX; i++) {
			g_checkedBoard[i] = 0; 
		}
		CountLibertyElement(tNode, g_board[tNode]);
	}

	void CountLibertyElement(int tNode, int color)
	{
		int adjNode;
		int i;

		g_checkedBoard[tNode] = 1;				// この石は検索済み	
		g_kakondaIshi++;								// 取れる相手の石の数
		for (i = 0; i < 4; i++) {
			adjNode = tNode + g_dir4[i];
			if (g_checkedBoard[adjNode]) {
				continue;
			}
			if (g_board[adjNode] == 0) {				// 空点
				g_checkedBoard[adjNode] = 1;			// この空点は検索済みとする
				g_liberty++;							// リバティの数
			}
			if (g_board[adjNode] == color) {
				CountLibertyElement(adjNode, color);	// 未探索の自分の石
			}
		}
	}

	int ConvertNode(int x, int y)
	{
		return (y + 1) * 256 + (x + 1);
	}

	int MoveOne(
		int node	,
		int color
	)
	{
		int i;
		int adjNode;			// 四方に隣接する交点
		int sum;
		int delNode		= 0;
		int all_ishi	= 0;	// 取った石の合計
		int un_col		= INVCLR(color);

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

	void DeleteStone(
		int tNode,
		int color
	)
	{
		int adjNode;	// 上下左右に隣接する交点
		int i;

		g_board[tNode] = 0;
		for (i = 0; i < 4; i++) {
			adjNode = tNode + g_dir4[i];
			if (g_board[adjNode] == color) {
				DeleteStone(adjNode, color);
			}
		}
	}

}
