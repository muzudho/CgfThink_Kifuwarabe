//
// /source/s670_noHitEye/s670_noHitEye.cpp
//

extern "C" {
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h670_noHit___/h670_100_noHitEye.h"
}




NoHitEye::NoHitEye()
{
};




void NoHitEye::Research(
	int color,
	int adjColor
)
{
	// �ׂɁA�ċz�_�� 2 �ȏ゠��i���΂�u���Ă��ċz�_�� 1 �ȏ�c��A
	// ���E��ɂ͂Ȃ�Ȃ��j�����ɂȂ���܂��B
	if (adjColor == color && 2 <= g_liberty) {
		this->safe++;
	}
}




bool NoHitEye::DontHit(
	)
{
	bool result = false;

	if (this->safe == 4) { // �l�����@�����̐΂�A�ǂɁ@�͂܂�Ă���ꏊ�i��j�ɂȂ�Ȃ�
							  //PRT(_T("��ɂ͑ł��Ȃ��B \n"));
							  // ��ɂ͑ł��Ȃ��B
		result = 1;
		goto gt_EndMethod;
	}

gt_EndMethod:
	return result;
}
