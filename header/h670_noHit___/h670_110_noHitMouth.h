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
// ��} x ���@���@�Ɍ����ĂāA
// 1 �����̒��Ɍ����āA
//
// ����̐΂̌��̒��ɑł��Ȃ��d�g�݁B
class NoHitMouth {
public:
	int adjOppo = 0;	// �㉺���E�ɗא�(adjacent)���鑊��(opponent)�̐΂̐��B

public:
	NoHitMouth();

	// ����̌��ɐ΂�ł����ޏ󋵂łȂ��������B
	void IsThis(
		int adjColor,
		int invClr		// ������Ԃ𔽓]����������
	);
};