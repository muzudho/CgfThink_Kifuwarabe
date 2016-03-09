//
// core/board.h
//
#pragma once

extern "C" {

	#include "../../header/think.h"

	//--------------------------------------------------------------------------------
	// �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	extern int g_board[BOARD_MAX];
	extern int g_dir4[4];
	extern int g_checkedBoard[BOARD_MAX];
	extern int g_ishi;
	extern int g_dame;
	extern int g_kouNode;
	extern int g_hama[2];
	extern void PRT(const _TCHAR* format, ...);


	//--------------------------------------------------------------------------------
	// �֐�
	//--------------------------------------------------------------------------------

	// �_���Ɛ΂̐��𒲂ׂ�
	void	count_dame(int tz);

	// �_���Ɛ΂̐��𒲂ׂ�ċA�֐�
	void	count_dame_sub(int tz, int col);

	// (x,y)��1�̍��W�ɕϊ�
	extern int		ConvertNode(int x, int y);

	// 1��i�߂�Bz ... ���W�Acol ... �΂̐F
	int		move_one(int z, int col);

	// �΂�����
	void	del_stone(int tz, int col);

}