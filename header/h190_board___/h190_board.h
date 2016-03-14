//
// /header/h190_board/h190_board.h
//
#pragma once

extern "C" {

	//--------------------------------------------------------------------------------
	// #define
	//--------------------------------------------------------------------------------

	// 碁石
	#define EMPTY 0		// 空きスペース
	#define BLACK 1
	#define WHITE 2
	#define WAKU  3		// 盤外

	#define BOARD_MAX ((19+2)*256)	// 19路盤を最大サイズとする
	#define INVCLR(x) (3-(x))		// 石の色を反転させる


	//--------------------------------------------------------------------------------
	// .cpp ファイルで定義されている グローバル変数
	//--------------------------------------------------------------------------------

	extern int g_board[BOARD_MAX];
	extern int g_dir4[4];
	extern int g_boardSize;
	extern int g_kouNode;
	extern int g_hama[2];


	//--------------------------------------------------------------------------------
	// 関数
	//--------------------------------------------------------------------------------

	// (x,y)を1つの座標に変換
	int	ConvertToNode(
		int x,
		int y
		);

	// (node)を(x,y)座標に変換
	void ConvertToXy(
		int& x,
		int& y,
		int node
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