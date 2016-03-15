//
// /header/h700_think/h700_think.h
//
#pragma once

#include <tchar.h> // Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB


extern "C" {

	//--------------------------------------------------------------------------------
	// enum
	//--------------------------------------------------------------------------------

	// ���݋ǖʂŉ������邩�A���w��
	enum GameType {
		GAME_MOVE,				// �ʏ�̎�
		GAME_END_STATUS,		// �I�Ǐ���
		GAME_DRAW_FIGURE,		// �}�`��`��
		GAME_DRAW_NUMBER 		// ���l������
	};


	//--------------------------------------------------------------------------------
	// �֐�
	//--------------------------------------------------------------------------------

	// �����ɋ߂��]���֐��B�����΂����ɍs���悤�ɁB
	int	Bestmove(
		int coler,		// �΂̐F
		int board[],
		int boardSize
	);

}