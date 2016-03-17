#pragma once


#include "../h090_core____/n090_100_core.h"
#include "../h190_board___/n190_100_board.h"
#include "../h190_board___/n190_200_libertyOfNodes.h"
#include "../h675_hit_____/n675_000_hit.h"

// 助けられそうな石を　ノビ　させようとします。
//
// _____
// __o__
// __1__
// _xox_
// __x__
// _____
//
// 上図 o を相手の石、 x を自分の石とし、
// 地点 1 に o を置くことで、
// 呼吸点 1～4 の自分の石の　呼吸点　を増やすことで助けられるよう、
// 評価が高くなる仕組み。
//
// 呼吸点 2以上の石は　あまり救出しようとしない。
class HitNobiSaver : Hit {
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