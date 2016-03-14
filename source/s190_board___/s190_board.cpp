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

	// 上、右、下、左　に移動するのに使う加減値
	int g_dir4[4] = {
		-0x100,	// 上
		+0x001,	// 右
		+0x100,	// 下
		-0x001	// 左
	};// オリジナルのcgfthinkでは右、左、下、上の順だった。

	// 盤面のサイズ。19路盤では19、9路盤では9
	int g_boardSize;

	// 次にコウになる位置。無ければ 0。
	int g_kouNode = 0;

	// ハマ。取った石の数のこと。[0]... 黒が取った石の数, [1]...白が取った石の数
	int g_hama[2];


	//--------------------------------------------------------------------------------
	// 関数
	//--------------------------------------------------------------------------------

	int ConvertToNode(int x, int y)
	{
		return (y + 1) * 256 + (x + 1);
	}

	void ConvertToXy(int& x, int& y, int node)
	{
		y = (node - 256) / 256;
		x = (node - 1) % 256;
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
