#pragma once

#include "../h190_board___/n190_100_board.h"

extern "C" {

	// �w��ǖʂ̕]���l�����߂܂��B
	extern int Evaluate(
		int&	flgAbort	,	// ���Ȃ��Ȃ� 0 �ȊO�B
		int		color		,	// ��Ԃ̐F
		int		node		,	// �΂�u���ʒu
		Board*  pBoard
	);

}
