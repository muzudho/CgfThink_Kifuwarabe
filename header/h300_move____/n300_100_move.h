#pragma once


#include <windows.h> // �R���\�[���ւ̏o�͓�


//--------------------------------------------------------------------------------
// enum
//--------------------------------------------------------------------------------

// move()�֐��Ŏ��i�߂����̌���
enum MoveResult {
	// ������󂯓����P��ށB
	MOVE_SUCCESS,			// ����

							// �����e���S��ށB
	MOVE_SUICIDE,			// ���E��
	MOVE_KOU,				// �R�E
	MOVE_EXIST,				// ���ɐ΂�����
	MOVE_FATAL				// �G���[�ȂǁB
};


class Move {
public:
	// 1��i�߂�B
	static int	MoveOne(
		HANDLE hConsoleWindow,
		int node,	// ���W
		int color,		// �΂̐F
		Board* pBoard
		);
};

