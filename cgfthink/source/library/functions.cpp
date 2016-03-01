
extern "C" {

	#include "../../header/cgfthink.h"

	//--------------------------------------------------------------------------------
	// グローバル変数
	//--------------------------------------------------------------------------------

	extern int board[BOARD_MAX];

	// 左右、上下に移動する場合の動く量
	extern int dir4[4];


	// 石を消す
	void del_stone(int tz, int col)
	{
		int z, i;

		board[tz] = 0;
		for (i = 0; i < 4; i++) {
			z = tz + dir4[i];
			if (board[z] == col) del_stone(z, col);
		}
	}
}