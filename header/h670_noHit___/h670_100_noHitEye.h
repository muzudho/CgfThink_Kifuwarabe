//
// /header/h670_noHit___/h670_100_noHitEye.h
//

#pragma once

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
	void Research(
		int color,
		int adjColor
	);

	// ���̌�_�ɂ͑ł����܂Ȃ��Ȃ�^�B
	bool DontHit(
	);
};