//
// cgfthink.h
//

// #pragma once とは、
// いくつものソースファイル（〜.c、〜.cpp）から　このヘッダ・ファイルを
// 何回 #include しようとしても　２回目以降無視する呪文だぜ☆
#pragma once

extern "C" {

	//--------------------------------------------------------------------------------
	// #define まとめ
	//--------------------------------------------------------------------------------

	// DLL_EXPORT は、あなたが今読んでいるこの .dll ファイルのソースコードとは別の、
	// 実行中の .exe プログラムから、呼び出させたい関数の頭に付けるおまじないだぜ☆
	#define DLL_EXPORT	__declspec( dllexport )


	//--------------------------------------------------------------------------------
	// 外部グローバル変数
	//--------------------------------------------------------------------------------

	//
	// extern とは、変数や関数を　別のファイルで既に 定義してあるときに、
	// もう他のファイルで定義してあるのでそれを使います、という意味で定義の頭に付ける、
	// おまじないだぜ☆！
	// コンパイラが読みに行くファイルのどこかに 1つ extern のついていない定義が
	// あればOKなんだぜ☆！ もう定義してあります、というコンパイル・エラーを防げるぜ☆！
	//

	extern int g_board[BOARD_MAX];
	extern int g_dir4[4];
	extern int g_checkedBoard[BOARD_MAX];
	extern int g_boardSize;
	extern int g_kakondaIshi;
	extern int g_liberty;
	extern int g_kouNode;
	extern int g_hama[2];
	extern HANDLE g_hConsoleWindow;

}

//--------------------------------------------------------------------------------
// 関数宣言まとめ　別のアプリケーションから呼び出されるもの
//--------------------------------------------------------------------------------

// 対局開始時に一度だけ呼ばれます。
DLL_EXPORT void cgfgui_thinking_init(
	int *pThinkStoped	// 普段は0。中止ボタンが押されたときに 1 になります。この値が1になった場合は思考を終了してください。
	);

// 思考ルーチン。次の1手の座標を返す。PASSの場合0。
// GUIから現在の局面の情報が渡される。
// また、終局処理の場合は、終局判断の結果を返す。
DLL_EXPORT int cgfgui_thinking(
	int		initBoard[]		,	// 初期盤面（置碁の場合は、ここに置石が入る）
	int		kifu[][3]		,	// 棋譜	[手数][0]...座標
								//		[手数][1]...石の色
								//		[手数][2]...消費時間（秒)
								// 手数は 0 から始まり、curTesuu の1つ手前まである。
	int		tesuu			,	// 現在の手数
	int		flgBlackTurn	,	// 黒手番フラグ(黒番...1、白番...0)。ここだけ定数と違ってややこしい。
	int		boardSize		,	// 盤面のサイズ
	double	komi			,	// コミ
	int		endgameType		,	// 0...通常の思考、1...終局処理、2...図形を表示、3...数値を表示。
	int		endgameBoard[]		// 終局処理の結果を代入する。
);

// 対局終了時に一度だけ呼ばれます。
// メモリの解放などが必要な場合にここに記述してください。
DLL_EXPORT void cgfgui_thinking_close(void);


