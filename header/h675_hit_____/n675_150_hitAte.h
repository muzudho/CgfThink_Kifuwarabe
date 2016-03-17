#pragma once


#include "../h090_core____/n090_100_core.h"
#include "../h190_board___/n190_100_board.h"
#include "../h190_board___/n190_200_libertyOfNodes.h"
#include "../h675_hit_____/n675_000_hit.h"

// アテようとします。
//
// _____
// __1__
// _xox_
// __x__
// _____
//
// 上図 o を相手の石、 x を自分の石とし、
// 1 に自分の石を置く評価が高くなる仕組み。
//
// ただし、1 の地点がコウになる場合は置きません。
//
// _____
// __o__
// _o1o_
// _xox_
// __x__
// _____
class HitAte : Hit {
public:

	// 評価値を出します。
	int Evaluate(
		Core			core,
		int				color,
		int				node,
		Board*			pBoard,
		LibertyOfNodes*	pLibertyOfNodes
		);
};