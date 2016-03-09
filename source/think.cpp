
extern "C" {

	#include <time.h>		// clock() を使用するために。
	#include <windows.h>	// rand() 等を使用するために。
	#include <tchar.h>		// Unicode対応の _T() 関数を使用するために。
	#include "../header/core/core_board.h"
	#include "../header/core/core_explain.h"
	#include "../header/core/core_endgame.h"
	#include "../header/core/core_ui.h"
	#include "../header/cgfthink.h"
	#include "../header/think.h"


	//--------------------------------------------------------------------------------
	// グローバル変数
	//--------------------------------------------------------------------------------

	int g_board[BOARD_MAX];

	// 右、左、下、上に移動するのに使う加減値
	int g_dir4[4] = {
		+0x001,	// 右
		-0x001,	// 左
		+0x100,	// 下
		-0x100	// 上
	};

	// 既にこの石を検索した場合は1
	int g_checkedBoard[BOARD_MAX];

	// 盤面のサイズ。19路盤では19、9路盤では9
	int g_boardSize;

	// 取った石の数(再帰関数で使う)
	int g_ishi = 0;

	// 連のダメの数(再帰関数で使う)
	int g_dame = 0;

	// 次にコウになる位置。無ければ 0。
	int g_kouNode = 0;

	// ハマ。取った石の数のこと。[0]... 黒が取った石の数, [1]...白が取った石の数
	int g_hama[2];

	// コンソールに出力するためのハンドル
	HANDLE g_hConsoleWindow = INVALID_HANDLE_VALUE; //static HANDLE hOutput;

	//--------------------------------------------------------------------------------
	// 関数定義
	//--------------------------------------------------------------------------------

	// 乱数に近い評価関数。少し石を取りに行くように。
	int think_sample(int col)
	{
		int max;
		int bestmoveNode;
		int x;
		int y;
		int node;
		int i;
		int value;
		int capture;
		int z1;
		int flag;
		int safe;
		int k;
		int un_col = UNCOL(col);

		// 実行するたびに違う値が得られるように現在の時刻で乱数を初期化
		srand((unsigned)clock());

		max = -1;
		bestmoveNode = 0;
		for (y = 0; y<g_boardSize; y++) for (x = 0; x<g_boardSize; x++) {
			node = ConvertNode(x, y);
			if (g_board[node]) continue;
			if (node == g_kouNode) continue;	// コウ

			value = rand() % 100;
			capture = safe = 0;
			for (i = 0; i<4; i++) {
				z1 = node + g_dir4[i];
				k = g_board[z1];
				if (k == WAKU) safe++;
				if (k == 0 || k == WAKU) continue;
				count_dame(z1);
				if (k == un_col && g_dame == 1) capture = 1;	// 敵石が取れる
				if (k == col && g_dame >= 2) safe++;			// 安全な味方に繋がる
				value += (k == un_col) * g_ishi * (10 / (g_dame + 1));
			}
			if (safe == 4) continue;	// 眼には打たない。
			if (capture == 0) {		// 石が取れない場合は実際に置いてみて自殺手かどうか判定
				int kz = g_kouNode;			// コウの位置を退避
				flag = move_one(node, col);
				g_board[node] = 0;
				g_kouNode = kz;
				if (flag == MOVE_SUICIDE) continue;	// 自殺手
			}
			//		PRT("x,y=(%d,%d)=%d\n",x,y,value);
			if (value > max) { max = value; bestmoveNode = node; }
		}
		return bestmoveNode;
	}
}