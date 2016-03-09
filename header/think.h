//
// think.h
//
#pragma once

#include <tchar.h> // Unicode対応の _T() 関数を使用するために。


extern "C" {

	//--------------------------------------------------------------------------------
	// #define
	//--------------------------------------------------------------------------------

	// 碁石
	#define BLACK 1
	#define WHITE 2
	#define WAKU  3		// 盤外

	#define BOARD_MAX ((19+2)*256)	// 19路盤を最大サイズとする
	#define INVCLR(x) (3-(x))		// 石の色を反転させる


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

	// move()関数で手を進めた時の結果
	enum MoveResult {
		MOVE_SUCCESS,			// 成功
		MOVE_SUICIDE,			// 自殺手
		MOVE_KOU,				// コウ
		MOVE_EXIST,				// 既に石が存在
		MOVE_FATAL				// それ以外
	};


	//--------------------------------------------------------------------------------
	// グローバル変数
	//--------------------------------------------------------------------------------

	// 盤面のサイズ。19路盤では19、9路盤では9
	extern int g_boardSize;


	//--------------------------------------------------------------------------------
	// 関数
	//--------------------------------------------------------------------------------

	// 乱数に近い評価関数。少し石を取りに行くように。
	int	Bestmove(
		int coler		// 石の色
	);

}