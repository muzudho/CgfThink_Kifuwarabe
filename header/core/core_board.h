//
// core/board.h
//
#pragma once

extern "C" {

	#include "../../header/think.h"

	//--------------------------------------------------------------------------------
	// グローバル変数
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
	// 関数
	//--------------------------------------------------------------------------------

	// ダメと石の数を調べる
	void	count_dame(int tz);

	// ダメと石の数を調べる再帰関数
	void	count_dame_sub(int tz, int col);

	// (x,y)を1つの座標に変換
	extern int		ConvertNode(int x, int y);

	// 1手進める。z ... 座標、col ... 石の色
	int		move_one(int z, int col);

	// 石を消す
	void	del_stone(int tz, int col);

}