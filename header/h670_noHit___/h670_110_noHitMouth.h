//
// /header/h670_noHit___/h670_110_noHitMouth.h
//

#pragma once

extern "C" {

}

// _____
// __x__
// _x1x_
// _____
// _____
//
// 上図 x を　口　に見立てて、
// 1 を口の中に見立て、
//
// 相手の石の口の中に打たない仕組み。
class NoHitMouth {
public:
	int adjOppo = 0;	// 上下左右に隣接(adjacent)する相手(opponent)の石の数。

public:
	NoHitMouth();

	// 相手の口に石を打ち込む状況でないか調査。
	void IsThis(
		int adjColor,
		int invClr		// 白黒手番を反転させたもの
	);
};