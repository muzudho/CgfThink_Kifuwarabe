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
	// �ׂɁA�ċz�_�� 2 �ȏ゠��i���΂�u���Ă��ċz�_�� 1 �ȏ�c��A
	// ���E��ɂ͂Ȃ�Ȃ��j�����ɂȂ���܂��B
	if (adjColor == color && 2 <= g_liberty) {
		this->safe++;
	}
}

void NoHitEye::Judge(
	int& flgAbort
	)
{
	if (this->safe == 4) { // �l�����@�����̐΂�A�ǂɁ@�͂܂�Ă���ꏊ�i��j�ɂȂ�Ȃ�
							  //PRT(_T("��ɂ͑ł��Ȃ��B \n"));
							  // ��ɂ͑ł��Ȃ��B
		flgAbort = 1;
		goto gt_EndMethod;
	}

gt_EndMethod:
	return;
}
