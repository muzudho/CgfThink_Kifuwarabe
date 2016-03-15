extern "C" {

	#include "../../header/h190_board___/n190_100_board.h"
	#include "../../header/h190_board___/n190_150_Liberty.h"
	#include "../../header/h675_hit_____/n675_100_hitTuke.h"

}




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
	int iDir;
	int adjNode;
	int adjColor;

	for (iDir = 0; iDir < 4; iDir++) {		// 上隣 → 右隣 → 下隣 → 左隣
		adjNode	 = node + pBoard->dir4[iDir];	// 隣接(adjacent)する交点と、
		adjColor = pBoard->table[adjNode];		// その色

		// 評価値の計算（４方向分繰り返す）
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
	}


	return score;
}