//
// /source/s670_noHit___/s670_110_noHitMouth.cpp
//

extern "C" {
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h670_noHit___/h670_110_noHitMouth.h"
}




NoHitMouth::NoHitMouth() {
};




void NoHitMouth::Research(
	int invColor,
	int adjColor
)
{
	// 2016-03-12 16:45 Add
	// �ׂ�����̐΁A�܂��͘g�Ȃ�J�E���g�A�b�v�B
	if (adjColor == invColor || adjColor == WAKU) {
		this->adjOppo++;
	}
}




int NoHitMouth::Evaluate(int flgCapture)
{
	int score = 0;

	// 2016-03-12 16:45 Add
	if (this->adjOppo == 3 && ! flgCapture)
	{
		// 3����������̐΂̂Ƃ����
		// ������Ȃ��Ƃ���ɂ́A�ł����݂����Ȃ��B
	}
	else
	{
		// ����ȊO�̓_���A�啝�ɉ��_�B
		score += 50;
	}

	return score;
}

