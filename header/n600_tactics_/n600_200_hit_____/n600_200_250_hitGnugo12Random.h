#pragma once


#include "../../n190_board___/n190_100_board.h"
#include "../../n600_tactics_/n600_200_hit_____/n600_200_000_hit.h"




// Gnugo1.2 を参考にしたランダムな評価を返します。
class HitGnugo12Random : Hit {
public:
	// 評価値を出します。
	int Evaluate(
		int		color,
		int		node,
		Board*	pBoard
		);
};