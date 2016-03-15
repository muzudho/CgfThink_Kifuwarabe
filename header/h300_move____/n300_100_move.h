#pragma once


#include <windows.h> // コンソールへの出力等


//--------------------------------------------------------------------------------
// enum
//--------------------------------------------------------------------------------

// move()関数で手を進めた時の結果
enum MoveResult {
	// 操作を受け入れる１種類。
	MOVE_SUCCESS,			// 成功

							// 操作を弾く４種類。
	MOVE_SUICIDE,			// 自殺手
	MOVE_KOU,				// コウ
	MOVE_EXIST,				// 既に石が存在
	MOVE_FATAL				// エラーなど。
};


class Move {
public:
	// 1手進める。
	static int	MoveOne(
		HANDLE hConsoleWindow,
		int node,	// 座標
		int color,		// 石の色
		Board* pBoard
		);
};

