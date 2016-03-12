//
// /header/h670_noHit___/h670_100_noHitEye.h
//

#pragma once

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
class NoHitEye {
public:
	int safe;

public:
	NoHitEye();

	// 自分の眼に打ち込む状況か調査
	void Research(
		int color,
		int adjColor
	);

	// この交点には打ち込まないなら真。
	bool DontHit(
	);
};