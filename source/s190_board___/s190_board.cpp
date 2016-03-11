//
// /source/s190_board/s190_board.cpp
//
extern "C" {

	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h190_board___/h190_board.h"


	//--------------------------------------------------------------------------------
	// グローバル変数
	//--------------------------------------------------------------------------------

	// 盤上の石の色。
	int g_board[BOARD_MAX];

	// 既にこの石を検索した場合は1
	int g_checkedBoard[BOARD_MAX];

	// 上、右、下、左　に移動するのに使う加減値
	int g_dir4[4] = {
		-0x100,	// 上
		+0x001,	// 右
		+0x100,	// 下
		-0x001	// 左
	};// オリジナルのcgfthinkでは右、左、下、上の順だった。

	// 盤面のサイズ。19路盤では19、9路盤では9
	int g_boardSize;

	// 囲んで取れる相手の石の数(再帰関数で使う)
	int g_kakondaIshi = 0;

	// 連のリバティ（石の呼吸点）の数(再帰関数で使う)
	int g_liberty = 0;

	// 次にコウになる位置。無ければ 0。
	int g_kouNode = 0;

	// ハマ。取った石の数のこと。[0]... 黒が取った石の数, [1]...白が取った石の数
	int g_hama[2];


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
		int iDir;

		g_checkedBoard[tNode] = 1;						// この石は検索済み	
		g_kakondaIshi++;								// 呼吸点を数えている（もしかすると連の）
														// 石の数
		for (iDir = 0; iDir < 4; iDir++) {				// 隣接する四方向
			adjNode = tNode + g_dir4[iDir];
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

	void DeleteRenStones(
		int tNode,
		int color
	)
	{
		int adjNode;	// 上下左右に隣接する交点
		int i;

		// 指定した位置の石を削除。
		g_board[tNode] = 0;

		// ４方向の石にも同じ処理を行います。
		for (i = 0; i < 4; i++) {
			adjNode = tNode + g_dir4[i];
			if (g_board[adjNode] == color) {
				DeleteRenStones(adjNode, color);
			}
		}
	}

}
