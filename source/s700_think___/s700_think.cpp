//
// /source/s700_think___/s700_think.cpp
//

// muzudho: ログ出力のために。
#include <iostream>
#include <fstream>
using namespace std;

extern "C" {

	#include <time.h>		// clock() を使用するために。
	#include <tchar.h>		// Unicode対応の _T() 関数を使用するために。
	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h300_move____/h300_move.h"
	#include "../../header/h390_explain_/h390_explain.h"
	#include "../../header/h400_endgame_/h400_endgame.h"
	#include "../../header/h690_evaluate/h690_evaluation.h"
	#include "../../header/h700_think___/h700_think.h"


	//--------------------------------------------------------------------------------
	// 関数
	//--------------------------------------------------------------------------------

	int Bestmove(
		int color
	)
	{
		PRT(_T("Bestmove開始☆！ \n"));
		//PRT(_T("color=%d invClr=%d \n", color, invClr));

		int x;
		int y;
		int node;
		int score;		// 読んでいる手の評価値
		int maxScore;	// 今まで読んだ手で一番高かった評価値
		int bestmoveNode;

		// 実行するたびに違う値が得られるように現在の時刻で乱数を初期化
		srand((unsigned)clock());

		//----------------------------------------
		// 石を置く位置１つずつ、その手の評価値を算出します。
		//----------------------------------------
		maxScore = -1;
		bestmoveNode = 0; // 0 ならパス。

		for (y = 0; y < g_boardSize; y++) {
			for (x = 0; x < g_boardSize; x++) {
				node = ConvertNode(x, y);
				//PRT(_T("node=%d \n"));

				// この局面で、石を置いたときの評価値
				int flgAbort = 0;
				score = Evaluate( flgAbort, color, node);
				if (flgAbort)
				{
					continue;
				}

				// ベストムーブを更新します。
				// PRT("x,y=(%d,%d)=%d\n",x,y,score);
				if (maxScore < score) {
					maxScore = score;
					bestmoveNode = node;
				}
			}
		}

		return bestmoveNode;
	}
}