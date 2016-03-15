#pragma once


#include "../h190_board___/n190_100_board.h"
#include "../h670_noHit___/n670_000_noHit.h"


// 盤上の端の方に　ぼっちの石（隣接する石がない石）　を打たない仕組み。
// 呼吸点が塞いでしまって　得をしにくい。
class NoHitHasinohoBocchi : NoHit {
private:
	// ぼっち石なら真。
	bool isBocchi;
	// 盤外なら真。
	bool isSoto;
	// 枠に隣接しているなら真。
	bool isEdge;
	// 角なら真。
	bool isCorner;
public:
	NoHitHasinohoBocchi();

	// どのような状況か調査。
	void Research(
		int		node,
		Board*	pBoard
	);

	// 評価値を出します。
	int Evaluate(
	);
};