//
// core/board.h
//
#pragma once


//--------------------------------------------------------------------------------
// 関数宣言まとめ
//--------------------------------------------------------------------------------

// ダメと石の数を調べる
void	count_dame		(int tz);

// ダメと石の数を調べる再帰関数
void	count_dame_sub	(int tz, int col);

// (x,y)を1つの座標に変換
int		ConvertNode		(int x, int y);		

// 1手進める。z ... 座標、col ... 石の色
int		move_one		(int z, int col);

// 石を消す
void	del_stone		(int tz, int col);
