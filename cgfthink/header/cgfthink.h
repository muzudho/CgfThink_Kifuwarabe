//
// cgfthink.h
//

// #pragma once とは、
// いくつものソースファイル（〜.c、〜.cpp）から　このヘッダ・ファイルを
// 何回 #include しようとしても　２回目以降無視する呪文だぜ☆
#pragma once

#include <tchar.h> // Unicode対応の _T() 関数を使用するために。

//--------------------------------------------------------------------------------
// #define まとめ
//--------------------------------------------------------------------------------

// DLL_EXPORT は、あなたが今読んでいるこの .dll ファイルのソースコードとは別の、
// 実行中の .exe プログラムから、呼び出させたい関数の頭に付けるおまじないだぜ☆
#define DLL_EXPORT	__declspec( dllexport )

// 碁石
#define BLACK 1
#define WHITE 2
#define WAKU  3		// 盤外

#define BOARD_MAX ((19+2)*256)	// 19路盤を最大サイズとする
#define UNCOL(x) (3-(x))		// 石の色を反転させる

//--------------------------------------------------------------------------------
// enum まとめ
//--------------------------------------------------------------------------------

// 現在局面で何をするか、を指定
enum GameType {
	GAME_MOVE,				// 通常の手
	GAME_END_STATUS,		// 終局処理
	GAME_DRAW_FIGURE,		// 図形を描く
	GAME_DRAW_NUMBER 		// 数値を書く
};

// コンピューター囲碁ソフト『彩』のスクリーンショットでも見たことのある人もいるかもしれない、
// 盤面、石の上に描く記号
// (形 | 色) で指定する。黒で四角を描く場合は (FIGURE_SQUARE | FIGURE_BLACK)
enum FigureType {
	FIGURE_NONE,			// 何も描かない
	FIGURE_TRIANGLE,		// 三角形
	FIGURE_SQUARE,			// 四角
	FIGURE_CIRCLE,			// 円
	FIGURE_CROSS,			// ×
	FIGURE_QUESTION,		// "？"の記号	
	FIGURE_HORIZON,			// 横線
	FIGURE_VERTICAL,		// 縦線
	FIGURE_LINE_LEFTUP,		// 斜め、左上から右下
	FIGURE_LINE_RIGHTUP,	// 斜め、左下から右上
	FIGURE_BLACK = 0x1000,	// 黒で描く（色指定)
	FIGURE_WHITE = 0x2000,	// 白で描く	(色指定）
};

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

// move()関数で手を進めた時の結果
enum MoveResult {
	MOVE_SUCCESS,			// 成功
	MOVE_SUICIDE,			// 自殺手
	MOVE_KOU,				// コウ
	MOVE_EXIST,				// 既に石が存在
	MOVE_FATAL				// それ以外
};


//--------------------------------------------------------------------------------
// 関数宣言まとめ（１）　別のアプリケーションから呼び出されるもの
//--------------------------------------------------------------------------------

// 思考ルーチン。本体から現在の手数とそれまでの棋譜が入った状態で呼ばれる。
// 手の座標を返す。PASSの場合0を。
// また、終局処理の場合は、終局判断の結果を返す。
DLL_EXPORT int cgfgui_thinking(
	int init_board[],	// 初期盤面（置碁の場合は、ここに置石が入る）
	int kifu[][3],		// 棋譜  [][0]...座標、[][1]...石の色、[][2]...消費時間（秒)
	int tesuu,			// 手数
	int black_turn,		// 手番(黒番...1、白番...0)
	int board_size,		// 盤面のサイズ
	double komi,		// コミ
	int endgame_type,	// 0...通常の思考、1...終局処理、2...図形を表示、3...数値を表示。
	int endgame_board[]	// 終局処理の結果を代入する。
);

// 対局開始時に一度だけ呼ばれます。
DLL_EXPORT void cgfgui_thinking_init(int *ptr_stop_thinking);

// 対局終了時に一度だけ呼ばれます。
DLL_EXPORT void cgfgui_thinking_close(void);


//--------------------------------------------------------------------------------
// 関数宣言まとめ（２）
//--------------------------------------------------------------------------------

// サンプルで使用する関数
void PRT(const _TCHAR* fmt, ...);	// printf()の代用関数。コンソールに出力。
void PassWindowsSystem(void);	// 一時的にWindowsに制御を渡します。

								// 関数のプロトタイプ宣言
void count_dame(int tz);				// ダメと石の数を調べる
void count_dame_sub(int tz, int col);	// ダメと石の数を調べる再帰関数
int move_one(int z, int col);			// 1手進める。z ... 座標、col ... 石の色
void print_board(void);					// 現在の盤面を表示
int think_sample(int col);
int get_z(int x, int y);					// (x,y)を1つの座標に変換
int endgame_status(int endgame_board[]);		// 終局処理
int endgame_draw_figure(int endgame_board[]);	// 図形を描く
int endgame_draw_number(int endgame_board[]);	// 数値を書く(0は表示されない)


//--------------------------------------------------------------------------------
// 関数宣言まとめ（３）　functions
//--------------------------------------------------------------------------------

// 石を消す
void del_stone(int tz, int col);


