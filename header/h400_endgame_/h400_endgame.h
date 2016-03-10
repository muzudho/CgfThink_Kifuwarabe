//
// /header/h400_endgame/h400_endgame.h
//
#pragma once

extern "C" {

	#include "../h190_board___/h190_board.h"


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

	extern int g_board[BOARD_MAX];
	extern int g_dir4[4];
	//extern int g_boardSize;
	extern int g_liberty;


	//--------------------------------------------------------------------------------
	// 関数宣言まとめ
	//--------------------------------------------------------------------------------

	int ConvertNode		(
		int x,
		int y
	);

	// 終局処理（サンプルでは簡単な判断で死石と地の判定をしています）
	int EndgameStatus	(
		int arr_endgameBoard[]	// 終局図の盤上の石の色
	);

	// 図形を描く
	int EndgameDrawFigure(
		int arr_endgameBoard[]
	);

	// 数値を書く(0は表示されない)
	int EndgameDrawNumber(
		int arr_endgameBoard[]
	);
}
