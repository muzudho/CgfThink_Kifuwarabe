#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_150_Liberty.h"
#include "../../header/h675_hit_____/n675_100_hitTuke.h"




HitTuke::HitTuke()
{
};




int HitTuke::Evaluate(
	int		invColor,
	int		node,
	Liberty liberties[4],
	Board*	pBoard
	)
{
	int score = 0;

#ifndef RANDOM_MOVE_ONLY
#ifdef ENABLE_MOVE_ATTACK
	// 評価値の計算（４方向分繰り返す）
	pBoard->ForeachArroundDirAndNodes(node, [&pBoard,&liberties,&score, invColor](int iDir,int adjNode, bool& isBreak) {
		int adjColor = pBoard->ValueOf(adjNode);		// その色

		score +=
			(adjColor == invColor)		// 隣の石
										//		相手の石: 1
										//		それ以外: 0
										//   ×
			* liberties[iDir].renIshi	// 連の石の数
										//   ×
			* (10 / (liberties[iDir].liberty + 1));	// 連の呼吸点の個数
													//		0個: 10点
													//		1個:  5点
													//		2個:  3.3333...点
													//		3個:  2.5点
													//		4個:  2点
													//		...
	});
#endif
#endif

	return score;
}