//
// core/board.h
//
#pragma once


//--------------------------------------------------------------------------------
// �֐��錾�܂Ƃ�
//--------------------------------------------------------------------------------

// �_���Ɛ΂̐��𒲂ׂ�
void	count_dame		(int tz);

// �_���Ɛ΂̐��𒲂ׂ�ċA�֐�
void	count_dame_sub	(int tz, int col);

// (x,y)��1�̍��W�ɕϊ�
int		ConvertNode		(int x, int y);		

// 1��i�߂�Bz ... ���W�Acol ... �΂̐F
int		move_one		(int z, int col);

// �΂�����
void	del_stone		(int tz, int col);
