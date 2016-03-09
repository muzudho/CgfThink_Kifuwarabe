//
// /header/core/core_endgame.h
//
#pragma once

extern "C" {

	#include "../../header/think.h"

	//--------------------------------------------------------------------------------
	// enum �܂Ƃ�
	//--------------------------------------------------------------------------------

	// �������ŃZ�b�g����l
	// ���̈ʒu�̐΂��u���v���u���v���B�s���ȏꍇ�́u���v�ŁB
	// ���̈ʒu�̓_���u���n�v�u���n�v�u�_���v���B
	enum GtpStatusType {
		GTP_ALIVE,				// ��
		GTP_DEAD,				// ��
		GTP_ALIVE_IN_SEKI,		// �Z�L�Ŋ��i���g�p�A�u���v�ő�p���ĉ������j
		GTP_WHITE_TERRITORY,	// ���n
		GTP_BLACK_TERRITORY,	// ���n
		GTP_DAME				// �_��
	};


	//--------------------------------------------------------------------------------
	// �ϐ��錾�܂Ƃ�
	//--------------------------------------------------------------------------------

	// ������ /source/think.cpp �����Ă��������B
	extern int g_board[BOARD_MAX];
	extern int g_dir4[4];
	extern int g_boardSize;
	extern int g_dame;


	//--------------------------------------------------------------------------------
	// �֐��錾�܂Ƃ�
	//--------------------------------------------------------------------------------

	extern int ConvertNode(int x, int y);

	// �I�Ǐ���
	extern int endgame_status(int endgame_board[]);

	// �}�`��`��
	extern int endgame_draw_figure(int endgame_board[]);

	// ���l������(0�͕\������Ȃ�)
	extern int endgame_draw_number(int endgame_board[]);
}
