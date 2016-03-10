//
// /source/think.cpp
//

// muzudho: ログ出力のために。
#include <iostream>
#include <fstream>
using namespace std;

extern "C" {

	#include <time.h>		// clock() を使用するために。
	#include <windows.h>	// rand() 等を使用するために。
	#include <tchar.h>		// Unicode対応の _T() 関数を使用するために。
	#include "../../header/h090_core/h090_core.h"
	#include "../../header/h300_move/h300_move.h"
	#include "../../header/h400_core/h400_board.h"
	#include "../../header/h400_core/h400_explain.h"
	#include "../../header/h400_core/h400_endgame.h"
	#include "../../header/h700_think/h700_think.h"


	//--------------------------------------------------------------------------------
	// グローバル変数
	//--------------------------------------------------------------------------------

	// 右、左、下、上に移動するのに使う加減値
	int g_dir4[4] = {
		+0x001,	// 右
		-0x001,	// 左
		+0x100,	// 下
		-0x100	// 上
	};

	// 既にこの石を検索した場合は1
	int g_checkedBoard[BOARD_MAX];

	// 囲んで取れる相手の石の数(再帰関数で使う)
	int g_kakondaIshi = 0;

	// 連のリバティ（石の呼吸点）の数(再帰関数で使う)
	int g_liberty = 0;

	// 次にコウになる位置。無ければ 0。
	int g_kouNode = 0;

	// ハマ。取った石の数のこと。[0]... 黒が取った石の数, [1]...白が取った石の数
	int g_hama[2];

	// コンソールに出力するためのハンドル
	HANDLE g_hConsoleWindow = INVALID_HANDLE_VALUE; //static HANDLE hOutput;

	//--------------------------------------------------------------------------------
	// 関数
	//--------------------------------------------------------------------------------

	// 説明はヘッダーファイルを見てください。
	int Bestmove(
		int color
	)
	{
		PRT(_T("Bestmove開始☆！ \n"));

		int bestmoveNode;
		int x;
		int y;
		int node;
		int i;
		int iDir;
		int maxScore;	// 今まで読んだ手で一番高かった評価値
		int score;		// 読んでいる手の評価値
		int flgCapture;	// 敵石を取ったフラグ
		int flgMove;	// 移動結果の種類
		int safe;
		int adjNode;	// 上下左右隣(adjacent)の交点
		int adjColor;	// 上下左右隣(adjacent)の石の色
		int invClr = INVCLR(color);//白黒反転

		PRT(_T("color=%d invClr=%d \n", color, invClr));


		// ログ： 動いていることの確認。
		ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
		outputfile << _T("Bestmove: (^q^)") << endl;


		// 実行するたびに違う値が得られるように現在の時刻で乱数を初期化
		srand((unsigned)clock());

		maxScore = -1;
		bestmoveNode = 0; // 0 ならパス。

		PRT(_T("g_boardSize=%d \n", g_boardSize));

		for (y = 0; y < g_boardSize; y++) {
			for (x = 0; x < g_boardSize; x++) {
				node = ConvertNode(x, y);
				PRT(_T("node=%d \n"));

				if (g_board[node]) {
					// 石があるか、枠なら
					PRT(_T("石があるか、枠。 \n"));
					continue;
				}
				if (node == g_kouNode) {
					// コウになる位置なら
					PRT(_T("コウ。 \n"));
					continue;
				}

				score		= rand() % 100; // 0 ～ 99 のランダムな評価値を与える。
				flgCapture	= 0;
				safe		= 0;
				for (iDir = 0; iDir < 4; iDir++) {
					adjNode		= node + g_dir4[iDir];
					adjColor	= g_board[adjNode];
					if (adjColor == WAKU) {
						// 枠なら
						PRT(_T("枠。 \n"));
						safe++;
					}
					if (adjColor == 0 || adjColor == WAKU) {
						// 空っぽか、枠なら。
						PRT(_T("空っぽか、枠。 \n"));
						continue;
					}

					// 隣の石のリバティ（呼吸点）の数を数えます。
					CountLiberty(adjNode);

					// 敵石で、呼吸点の数が 1 なら、ここに石を置くと取ることができます。
					if (adjColor == invClr && g_liberty == 1) {
						PRT(_T("敵石を取った。 \n"));
						flgCapture = 1; 	// 敵石を、取ったフラグ。
					}

					// ここに石を置いても、呼吸点が 1 以上残る（＝安全な）味方につながります。
					if (adjColor == color && 2 <= g_liberty ) {
						safe++;
					}

					// 評価値の計算
					score +=
						(adjColor == invClr)		// 隣の石は、相手の石なら 1。
						* g_kakondaIshi				// 囲んで取れる石の数。
						* (10 / (g_liberty + 1));	// 連の呼吸点の個数が
													//		0 なら 10点
													//		1 なら 5点
													//		2 なら 3.3333...点
													//		3 なら 2.5点
													//		4 なら 2点
													//		...
					PRT(_T("スコア=%d \n", score));
				}
				if (safe == 4) { // 四方が　自分の石や、壁に　囲まれている場所（眼）になるなら
					PRT(_T("眼には打たない。 \n"));
					continue;	// 眼には打たない。
				}
				if (flgCapture == 0) {		// 石が取れない場合
					// 実際に置いてみて　自殺手かどうか判定
					int temp_kouNode	= g_kouNode;				// コウの位置を退避
					flgMove				= MoveOne(node, color);		// コウの位置が変わるかも。
					g_board[node]		= 0;
					g_kouNode			= temp_kouNode;
					if (flgMove == MOVE_SUICIDE) {	// 自殺手なら
						PRT(_T("自殺手は打たない。 \n"));
						continue;	// ベストムーブにはなりえない
					}
				}

				// ベストムーブを更新します。
				// PRT("x,y=(%d,%d)=%d\n",x,y,value);
				if (maxScore < score) {
					maxScore = score;
					bestmoveNode = node;
				}
			}
		}


		return bestmoveNode;
		//return 1*256 + 1;
	}
}