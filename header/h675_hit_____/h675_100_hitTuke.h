//
// /header/h675_hit_____/h675_100_hitTuke.h
//

#pragma once

// _____
// _ooo_
// __o__
// __1__
// _____
//
// ��} o �𑊎�̐΂Ƃ��A
// 1 �Ɏ����̐΂�u�����Ƃ��A
// �� �̐��������A�� �̌ċz�_�����Ȃ����̂ق�
// 1 �̕]���������Ȃ�d�g�݁B
//
// �c�P�悤�Ƃ��܂��B
class HitTuke {
public:
	int safe = 0;

public:
	HitTuke();

	// �]���l���o���܂��B
	int Evaluate_AdjNode(
		int invColor,
		int adjColor
		);

};