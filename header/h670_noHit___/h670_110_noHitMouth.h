//
// /header/h670_noHit___/h670_110_noHitMouth.h
//

#pragma once


#include "../../header/h670_noHit___/h670_000_noHit.h"


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
class NoHitMouth : NoHit {
public:
	int adjOppo;	// �㉺���E�ɗא�(adjacent)���鑊��(opponent)�̐΂̐��B

public:
	NoHitMouth();

	// ����̌��ɐ΂�ł����ޏ󋵂łȂ��������B
	void Research(
		int color,
		int node
	);

	// �]���l���o���܂��B
	int Evaluate(
		int flgCapture		// suicide.flgCapture
	);
};