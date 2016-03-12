//
// /header/h675_hit_____/h675_100_hitTuke.h
//

#pragma once

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
class HitTuke {
public:
	int safe = 0;

public:
	HitTuke();

	// 評価値を出します。
	int Evaluate_AdjNode(
		int invColor,
		int adjColor
		);

};