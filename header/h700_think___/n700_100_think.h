//
// /header/h700_think/h700_think.h
//
#pragma once

#include <tchar.h> // Unicode対応の _T() 関数を使用するために。


extern "C" {

	//--------------------------------------------------------------------------------
	// enum
	//--------------------------------------------------------------------------------

	// 現在局面で何をするか、を指定
	enum GameType {
		GAME_MOVE,				// 通常の手
		GAME_END_STATUS,		// 終局処理
		GAME_DRAW_FIGURE,		// 図形を描く
		GAME_DRAW_NUMBER 		// 数値を書く
	};


	//--------------------------------------------------------------------------------
	// 関数
	//--------------------------------------------------------------------------------

	// 乱数に近い評価関数。少し石を取りに行くように。
	int	Bestmove(
		int coler,		// 石の色
		int board[],
		int boardSize
	);

}