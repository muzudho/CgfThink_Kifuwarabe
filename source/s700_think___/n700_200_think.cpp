// muzudho: ログ出力のために。
#include <iostream>
#include <fstream>
using namespace std;

#include <time.h>		// clock() を使用するために。
#include <tchar.h>		// Unicode対応の _T() 関数を使用するために。
#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_200_libertyOfNodes.h"
#include "../../header/h300_move____/n300_100_move.h"
#include "../../header/h390_explain_/n390_100_explain.h"
#include "../../header/h490_endgame_/n490_100_endgame.h"
#include "../../header/h700_think___/n700_100_evaluation.h"
#include "../../header/h700_think___/n700_200_think.h"


int Think::Bestmove(
	HANDLE			hConsoleWindow	,
	int				color			,
	Board*			pBoard			,
	LibertyOfNodes*	pLibertyOfNodes
)
{
	Core::PRT(hConsoleWindow, _T("Bestmove開始☆！ \n"));

	int maxScore;	// 今まで読んだ手で一番高かった評価値
	int bestmoveNode;

	// 実行するたびに違う値が得られるように現在の時刻で乱数を初期化
	srand((unsigned)clock());

	//----------------------------------------
	// 石を置く位置１つずつ、その手の評価値を算出します。
	//----------------------------------------
	maxScore = -1;
	bestmoveNode = 0; // 0 ならパス。

	Core::PRT(hConsoleWindow, _T("aa \n"));

	pBoard->ForeachAllNodesWithoutWaku([color,&maxScore,&bestmoveNode,&pBoard,&pLibertyOfNodes, &hConsoleWindow](int node, bool& isBreak) {
		//PRT(_T("node=%d \n"));

		Core::PRT(hConsoleWindow, _T("bb \n"));

		// この局面で、石を置いたときの評価値
		int flgAbort = 0;
		int score;		// 読んでいる手の評価値
		score = Evaluation::Evaluate(hConsoleWindow, flgAbort, color, node, pBoard, pLibertyOfNodes);
		if (flgAbort)
		{
			goto gt_Next;
		}

		// ベストムーブを更新します。
		// PRT("x,y=(%d,%d)=%d\n",x,y,score);
		if (maxScore < score) {
			maxScore = score;
			bestmoveNode = node;
		}
	gt_Next:
		;
	});

	Core::PRT(hConsoleWindow, _T("cc \n"));

	return bestmoveNode;
}
