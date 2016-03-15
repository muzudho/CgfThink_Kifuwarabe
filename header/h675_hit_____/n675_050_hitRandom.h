//
// /header/h675_hit_____/h675_050_random.h
//

#pragma once


#include "../../header/h675_hit_____/n675_000_hit.h"




// 0～99 の数字をランダムに返します。
class HitRandom : Hit {
public:
	HitRandom();

	// 評価値を出します。
	int Evaluate_AdjNode(
		);
};