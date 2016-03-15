//
// /header/h300_move/h300_move.h
//
#pragma once

extern "C" {

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

	// 1��i�߂�B
	int	MoveOne(
		int node,	// ���W
		int color,		// �΂̐F
		int board[]
		);

}

