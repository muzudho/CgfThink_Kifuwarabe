﻿#pragma once


#include "../n670_noHit___/n670_000_noHit.h"


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
class NoHitMouth : NoHit {
public:
	int adjOppo;	// 上下左右に隣接(adjacent)する相手(opponent)の石の数。

public:
	NoHitMouth();

	// 相手の口に石を打ち込む状況でないか調査。
	void Research(
		int		color	,
		int		node	,
		Board*	pBoard
	);

	// 評価値を出します。
	int Evaluate(
		int flgCapture		// suicide.flgCapture
	);
};