//
// /header/h190_board/h190_board.h
//
#pragma once

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
	// .cpp ファイルで定義されている グローバル変数
	//--------------------------------------------------------------------------------

	extern int g_board[BOARD_MAX];
	extern int g_checkedBoard[BOARD_MAX];
	extern int g_dir4[4];
	extern int g_boardSize;
	extern int g_renIshi;
	extern int g_liberty;
	extern int g_kouNode;
	extern int g_hama[2];


	//--------------------------------------------------------------------------------
	// 関数
	//--------------------------------------------------------------------------------

	// 位置 tNode におけるリバティ（石の呼吸点）の数と石の数を計算。結果はグローバル変数に格納。
	void	CountLiberty(
		int node
	);

	// リバティ（石の呼吸点）と石の数える再帰関数
	// 4方向を調べて、空白だったら+1、自分の石なら再帰で。相手の石、壁ならそのまま。
	void	CountLibertyElement(
		int tNode,
		int color
		);

	// (x,y)を1つの座標に変換
	int	ConvertNode(
		int x,
		int y
		);

	// 1手進める。
	int	MoveOne(
		int node,	// 座標
		int color		// 石の色
		);

	// 連になっている石を消す。１個の石でも消す。
	void	DeleteRenStones(
		int tNode,
		int color
		);

}