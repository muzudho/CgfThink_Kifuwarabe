//
// /source/s670_noHit___/s670_110_noHitMouth.cpp
//

extern "C" {
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h670_noHit___/h670_110_noHitMouth.h"
}


NoHitMouth::NoHitMouth() {
};

void NoHitMouth::IsThis(
	int adjColor,
	int invClr
)
{
	// 2016-03-12 16:45 Add
	// �ׂ�����̐΁A�܂��͘g�Ȃ�J�E���g�A�b�v�B
	if (adjColor == invClr || adjColor == WAKU) {
		this->adjOppo++;
	}
}
