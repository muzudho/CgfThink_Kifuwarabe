//
// /header/h490_endgame/h490_endgame.h
//
#pragma once

extern "C" {

	#include "../h190_board___/n190_100_board.h"


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
	// .cpp �t�@�C���Œ�`����Ă��� �O���[�o���ϐ�
	//--------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------
	// �֐��錾�܂Ƃ�
	//--------------------------------------------------------------------------------

	int ConvertToNode		(
		int x,
		int y
	);

	// �I�Ǐ����i�T���v���ł͊ȒP�Ȕ��f�Ŏ��΂ƒn�̔�������Ă��܂��j
	int EndgameStatus	(
		int arr_endgameBoard[]	,	// �I�ǐ}�̔Տ�̐΂̐F
		int board[]				,
		int baordSize
	);

	// �}�`��`��
	int EndgameDrawFigure(
		int arr_endgameBoard[],
		int boardSize
	);

	// ���l������(0�͕\������Ȃ�)
	int EndgameDrawNumber(
		int arr_endgameBoard[],
		int boardSize
	);
}