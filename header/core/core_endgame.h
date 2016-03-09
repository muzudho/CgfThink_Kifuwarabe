//
// /header/core/core_endgame.h
//
#pragma once

extern "C" {

	#include "../../header/think.h"

	//--------------------------------------------------------------------------------
	// enum まとめ
	//--------------------------------------------------------------------------------

	// 死活情報でセットする値
	// その位置の石が「活」か「死」か。不明な場合は「活」で。
	// その位置の点が「黒地」「白地」「ダメ」か。
	enum GtpStatusType {
		GTP_ALIVE,				// 活
		GTP_DEAD,				// 死
		GTP_ALIVE_IN_SEKI,		// セキで活（未使用、「活」で代用して下さい）
		GTP_WHITE_TERRITORY,	// 白地
		GTP_BLACK_TERRITORY,	// 黒地
		GTP_DAME				// ダメ
	};


	//--------------------------------------------------------------------------------
	// 変数宣言まとめ
	//--------------------------------------------------------------------------------

	// 説明は /source/think.cpp を見てください。
	extern int g_board[BOARD_MAX];
	extern int g_dir4[4];
	extern int g_boardSize;
	extern int g_dame;


	//--------------------------------------------------------------------------------
	// 関数宣言まとめ
	//--------------------------------------------------------------------------------

	extern int ConvertNode(int x, int y);

	// 終局処理
	extern int endgame_status(int endgame_board[]);

	// 図形を描く
	extern int endgame_draw_figure(int endgame_board[]);

	// 数値を書く(0は表示されない)
	extern int endgame_draw_number(int endgame_board[]);
}
