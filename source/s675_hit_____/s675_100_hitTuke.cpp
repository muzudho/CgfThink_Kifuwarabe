//
// /source/s675_hit_____/s675_100_hitTuke.cpp
//

extern "C" {

	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h675_hit_____/h675_100_hitTuke.h"

}




HitTuke::HitTuke()
{
	this->unuse = 0;
};




int HitTuke::Evaluate_AdjNode(
	int invColor,
	int adjColor
	)
{
	int score = 0;

	// 評価値の計算（４方向分繰り返す）
	score +=
		(adjColor == invColor)		// 隣の石
									//		相手の石: 1
									//		それ以外: 0
									//   ×
		* g_renIshi					// 連の石の数
									//   ×
		* (10 / (g_liberty + 1));	// 連の呼吸点の個数
									//		0個: 10点
									//		1個:  5点
									//		2個:  3.3333...点
									//		3個:  2.5点
									//		4個:  2点
									//		...

	return score;
}