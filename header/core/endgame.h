//
// endgame.h
//
#pragma once


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
// �֐��錾�܂Ƃ�
//--------------------------------------------------------------------------------

int endgame_status(int endgame_board[]);		// �I�Ǐ���
int endgame_draw_figure(int endgame_board[]);	// �}�`��`��
int endgame_draw_number(int endgame_board[]);	// ���l������(0�͕\������Ȃ�)
