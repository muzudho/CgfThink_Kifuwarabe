//
// /header/h670_noHit___/h670_100_noHitEye.h
//

#pragma once


#include "../../header/h200_research/h200_100_Liberty.h"
#include "../../header/h670_noHit___/h670_000_noHit.h"


// _____
// __o__
// _o1o_
// __o__
// _____
//
// 上図 o を自分の石とし、
// 1 を　眼　に見立てて、
//
// 自分の眼に打たない仕組み。
class NoHitOwnEye : NoHit {
public:
	// ４方向に隣接する、呼吸点が増える　つなげられる味方の石が　いくつか。0～4。
	int safe;

public:
	NoHitOwnEye();

	// 自分の眼に打ち込む状況か調査
	void Research(
		int color,
		int	node,
		Liberty liberties[4]
	);

	// この交点には打ち込まないなら真。
	bool DontHit(
	);
};