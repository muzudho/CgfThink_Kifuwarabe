#pragma once

#include "../h190_board___/n190_100_board.h"

class Evaluation {
public:
	// �w��ǖʂ̕]���l�����߂܂��B
	static int Evaluate(
		int&	flgAbort,	// ���Ȃ��Ȃ� 0 �ȊO�B
		int		color,	// ��Ԃ̐F
		int		node,	// �΂�u���ʒu
		Board*  pBoard
		);
};
