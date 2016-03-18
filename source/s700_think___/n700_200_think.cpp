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
	Core			core			,
	int				color			,
	Board*			pBoard			,
	LibertyOfNodes*	pLibertyOfNodes
)
{
#ifdef CHECK_LOG
	core.PRT( _T("Bestmove開始☆！ \n"));
#endif

	int maxScore;	// 今まで読んだ手で一番高かった評価値
	int bestmoveNode;

	// 実行するたびに違う値が得られるように現在の時刻で乱数を初期化
	srand((unsigned)clock());

	//----------------------------------------
	// 石を置く位置１つずつ、その手の評価値を算出します。
	//----------------------------------------
	maxScore = -1;
	bestmoveNode = 0; // 0 ならパス。

	pBoard->ForeachAllNodesWithoutWaku([color,&maxScore,&bestmoveNode,&pBoard,&pLibertyOfNodes, &core](int node, bool& isBreak) {
		//{
			//int x, y;
			//AbstractBoard::ConvertToXy(x, y, node);
			//core.PRT(_T("#(%d,%d) "), x, y);
		//}

		// この局面で、石を置いたときの評価値
		int flgAbort = 0;
		int score;		// 読んでいる手の評価値
		score = Evaluation::EvaluateAtNode(core, flgAbort, color, node, pBoard, pLibertyOfNodes);
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

	return bestmoveNode;
}
