//
// /source/s670_noHit___/s670_110_noHitMouth.cpp
//

extern "C" {
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h670_noHit___/h670_110_noHitMouth.h"
}




NoHitMouth::NoHitMouth() {
	this->adjOppo = 0;
};




void NoHitMouth::Research(
	int color,
	int node
)
{
	int invColor = INVCLR(color);	//白黒反転
	int iDir;
	int adjNode;	// 上下左右隣(adjacent)の交点
	int adjColor;	// 上下左右隣(adjacent)の石の色

	for (iDir = 0; iDir < 4; iDir++) {		// 上隣 → 右隣 → 下隣 → 左隣
		adjNode = node + g_dir4[iDir];		// 隣接(adjacent)する交点と、
		adjColor = g_board[adjNode];		// その色

		// 2016-03-12 16:45 Add
		// 隣が相手の石、または枠ならカウントアップ。
		if (adjColor == invColor || adjColor == WAKU) {
			this->adjOppo++;
		}
	}

}




int NoHitMouth::Evaluate(int flgCapture)
{
	int score = 0;

	// 2016-03-12 16:45 Add
	if (this->adjOppo == 3 && ! flgCapture)
	{
		// 3方向が相手の石のところで
		// 駒も取れないところには、打ち込みたくない。
	}
	else
	{
		// それ以外の点を、大幅に加点。
		//score += 50;
		score += 100;
	}

	return score;
}

