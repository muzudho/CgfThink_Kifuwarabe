//
// /source/s670_noHitEye/s670_noHitEye.cpp
//

extern "C" {
	#include "../../header/h670_noHit___/h670_100_noHitEye.h"
	#include "../../header/h190_board___/h190_board.h"
}


NoHitEye::NoHitEye()
{
};

void NoHitEye::IsThis(
	int color,
	int adjColor
)
{
	// 隣に、呼吸点が 2 以上ある（＝石を置いても呼吸点が 1 以上残る、
	// 自殺手にはならない）味方につながります。
	if (adjColor == color && 2 <= g_liberty) {
		this->safe++;
	}
}

void NoHitEye::Judge(
	int& flgAbort
	)
{
	if (this->safe == 4) { // 四方が　自分の石や、壁に　囲まれている場所（眼）になるなら
							  //PRT(_T("眼には打たない。 \n"));
							  // 眼には打たない。
		flgAbort = 1;
		goto gt_EndMethod;
	}

gt_EndMethod:
	return;
}
