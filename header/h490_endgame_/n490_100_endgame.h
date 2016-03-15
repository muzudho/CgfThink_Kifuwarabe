#pragma once


extern "C" {

	#include "../h190_board___/n190_100_board.h"



	//--------------------------------------------------------------------------------
	// 関数宣言まとめ
	//--------------------------------------------------------------------------------

	// 終局処理（サンプルでは簡単な判断で死石と地の判定をしています）
	int EndgameStatus	(
		int arr_endgameBoard[]	,	// 終局図の盤上の石の色
		Board* pBoard
	);

	// 図形を描く
	int EndgameDrawFigure(
		int arr_endgameBoard[],
		Board* pBoard
	);

	// 数値を書く(0は表示されない)
	int EndgameDrawNumber(
		int arr_endgameBoard[],
		Board* pBoard
	);
}

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
