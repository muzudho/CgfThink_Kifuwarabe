//
// /source/s190_board/s190_board.cpp
//
extern "C" {

	#include "../../header/h090_core/h090_core.h"
	#include "../../header/h190_board/h190_board.h"


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
