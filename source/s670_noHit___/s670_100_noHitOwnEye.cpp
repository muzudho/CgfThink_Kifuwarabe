//
// /source/s670_noHitEye/s670_noHitEye.cpp
//

extern "C" {
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h200_research/h200_100_Liberty.h"
	#include "../../header/h670_noHit___/h670_100_noHitOwnEye.h"
}




NoHitOwnEye::NoHitOwnEye()
{
	this->safe = 0;
};




bool NoHitOwnEye::IsThis(
	int color,
	int	node,
	Liberty liberties[4]
)
{
	bool result = false;

	int iDir;
	int adjNode;	// 上下左右隣(adjacent)の交点
	int adjColor;	// 上下左右隣(adjacent)の石の色

	for (iDir = 0; iDir < 4; iDir++) {		// 上隣 → 右隣 → 下隣 → 左隣
		adjNode = node + g_dir4[iDir];	// 隣接(adjacent)する交点と、
		adjColor = g_board[adjNode];		// その色

		// 次の２つは　安全なつながり方です。
		// (１)枠につなげる。
		// (２)呼吸点が 2 以上ある（＝石を置いても呼吸点が 1 以上残る、
		//     自殺手にはならない）味方につながる。
		if (
			adjColor == WAKU
			||
			(adjColor == color && 2 <= liberties[iDir].liberty)
			) {
			//PRT(_T("安全な隣接。 \n"));
			this->safe++;
		}
	}


	if (this->safe == 4) { // 四方が　自分の石や、壁に　囲まれている場所（眼）になるなら
						   //PRT(_T("眼には打たない。 \n"));
						   // 眼には打たない。
		result = 1;
		goto gt_EndMethod;
	}

gt_EndMethod:
	return result;
}
