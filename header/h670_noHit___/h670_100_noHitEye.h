//
// /header/h670_noHit___/h670_100_noHitEye.h
//

#pragma once

extern "C" {

}

// _____
// __o__
// _o1o_
// __o__
// _____
//
// ��} o �������̐΂Ƃ��A
// 1 ���@��@�Ɍ����ĂāA
//
// �����̊�ɑł��Ȃ��d�g�݁B
class NoHitEye {
public:
	int safe;

public:
	NoHitEye();

	// �����̊�ɑł����ޏ󋵂�����
	void IsThis(
		int color,
		int adjColor
	);

	void Judge(
		int& flgAbort
	);
};