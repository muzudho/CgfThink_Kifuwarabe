//
// /header/h670_noHit___/h670_100_noHitEye.h
//

#pragma once


#include "../../header/h200_research/h200_100_Liberty.h"
#include "../../header/h670_noHit___/h670_000_noHit.h"


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
class NoHitOwnEye : NoHit {
public:
	// �S�����ɗאڂ���A�ċz�_��������@�Ȃ����閡���̐΂��@�������B0�`4�B
	int safe;

public:
	NoHitOwnEye();

	// �����̊�ɑł����ޏ󋵂�����
	void Research(
		int color,
		int	node,
		Liberty liberties[4]
	);

	// ���̌�_�ɂ͑ł����܂Ȃ��Ȃ�^�B
	bool DontHit(
	);
};