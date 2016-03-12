//
// /header/h675_hit_____/h675_100_hitTuke.h
//

#pragma once


#include "../../header/h675_hit_____/h675_000_hit.h"


// _____
// _ooo_
// __o__
// __1__
// _____
//
// 上図 o を相手の石とし、
// 1 に自分の石を置いたとき、
// ○ の数が多く、○ の呼吸点が少ないものほど
// 1 の評価が高くなる仕組み。
//
// ツケようとします。
class HitTuke : Hit {
public:
	HitTuke();

	// 評価値を出します。
	int Evaluate_AdjNode(
		int invColor,
		int adjColor
		);

};