#pragma once

#include "../h190_board___/n190_100_board.h"

class Evaluation {
public:
	// 指定局面の評価値を求めます。
	static int Evaluate(
		int&	flgAbort,	// 解なしなら 0 以外。
		int		color,	// 手番の色
		int		node,	// 石を置く位置
		Board*  pBoard
		);
};
