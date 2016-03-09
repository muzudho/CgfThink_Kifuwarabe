//
// コンピューター囲碁ソフト『きふわらべ』の思考エンジン
//
// CgfGoban.exe用の思考ルーチンのサンプル
//
// 『2005/06/04 - 2005/07/15 山下 宏』版を元に改造。
// 乱数で手を返すだけです。

// muzudho: ログ出力のために。
#include <iostream>
#include <fstream>
using namespace std;

extern "C" {

	#include <windows.h> // コンソールへの出力等
	#include <tchar.h> // Unicode対応の _T() 関数を使用するために。
	#include "../header/core/core_ui.h"
	#include "../header/core/core_board.h"
	#include "../header/core/core_explain.h"
	#include "../header/core/core_endgame.h"
	#include "../header/cgfthink.h"
	#include "../header/think.h"

	//--------------------------------------------------------------------------------
	// グローバル変数
	//--------------------------------------------------------------------------------

	// 思考中断フラグ。0で初期化されています。
	// GUIの「思考中断ボタン」を押された場合に1になります。
	int* g_pThinkStoped = NULL;

	// 累計思考時間 [0]先手 [1]後手。　黒白とは限らない？
	int g_thoughtTime[2];


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

	// 説明は header/functions.cpp を参照してください。
	extern int g_board[BOARD_MAX];
	extern int g_dir4[4];
	extern int g_checkedBoard[BOARD_MAX];
	extern int g_boardSize;
	extern int g_ishi;
	extern int g_dame;
	extern int g_kouNode;
	extern int g_hama[2];
	extern HANDLE g_hConsoleWindow;

}//extern "C"

// 説明は cgfthink.h の関数プロトタイプ宣言を参照してください。
DLL_EXPORT void cgfgui_thinking_init(
	int* pThinkStoped
)
{
	// ログ： 動いていることの確認。
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"));
	outputfile << _T("called: cgfgui_thinking_init") << endl;

	//--------------------
	// ポインタの受け渡し
	//--------------------
	g_pThinkStoped = pThinkStoped;

	// PRT()情報を表示するためのコンソールを起動する。
	AllocConsole();		// この行をコメントアウトすればコンソールは表示されません。
	SetConsoleTitle(_T("CgfgobanDLL Infomation Window"));
	g_hConsoleWindow = GetStdHandle(STD_OUTPUT_HANDLE);
	PRT(_T("デバッグ用の窓です。PRT()関数で出力できます。\n"));

	// この下に、メモリの確保など必要な場合のコードを記述してください。
}

// 説明は cgfthink.h の関数プロトタイプ宣言を参照してください。
DLL_EXPORT int cgfgui_thinking(
	int		initBoard[]		,
	int		kifu[][3]		,
	int		curTesuu		,
	int		blackTurn		,
	int		boardSize		,
	double	komi			,
	int		endgameType		,
	int		endgameBoard[]
)
{
	// ログ： 動いていることの確認。
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking") << endl;

	int node;			// 囲碁盤上の交点（将棋盤でいうマス目）
	int bestmoveNode;	// コンピューターが打つ交点。
	int color;			// 石の色
	int time;			// 消費時間
	int i;
	int iNode;
	int iTesuu;

	// 現在局面を棋譜と初期盤面から作る
	for (iNode = 0; iNode < BOARD_MAX; iNode++) {
		g_board[iNode] = initBoard[iNode];	// 初期盤面をコピー
	}

	//--------------------
	// 何路盤
	//--------------------
	g_boardSize				= boardSize;

	//--------------------
	// 初期化
	//--------------------
	g_hama	     [BLACK]	= g_hama	   [WHITE]	= 0;
	g_thoughtTime[BLACK]	= g_thoughtTime[WHITE]	= 0;	// 累計思考時間を初期化
	g_kouNode				= 0;

	// 棋譜を進めていくぜ☆
	for (iTesuu =0; iTesuu<curTesuu; iTesuu++) {
		node   = kifu[iTesuu][0];	// 座標、y*256 + x の形で入っている
		color = kifu[iTesuu][1];	// 石の色
		time   = kifu[iTesuu][2];	// 消費時間
		g_thoughtTime[iTesuu & 1] += time;
		if (move_one(node, color) != MOVE_SUCCESS) {
			break;
		}
	}
	 
#if 0	// 中断処理を入れる場合のサンプル。0を1にすればコンパイルされます。
	for (i=0;i<300;i++) {				// 300*10ms = 3000ms = 3秒待ちます。
		PassWindowsSystem();			// 一時的にWindowsに制御を渡します。
		if ( *g_pThinkStop != 0 ) break;	// 中断ボタンが押された場合。
		Sleep(10);						// 10ms(0.01秒)停止。
	}
#endif

	// 終局処理、図形、数値を表示する場合
	switch (endgameType)
	{
	case GAME_END_STATUS:
		return endgame_status(endgameBoard);
	case GAME_DRAW_FIGURE:
		return endgame_draw_figure(endgameBoard);
	case GAME_DRAW_NUMBER:
		return endgame_draw_number(endgameBoard);
	default:
		break;
	}

	//--------------------------------------------------------------------------------
	// 思考ルーチン
	//--------------------------------------------------------------------------------

	// ランダムに１手指します。
	if (blackTurn) {
		color = BLACK;
	} else {
		color = WHITE;
	}
	bestmoveNode = think_sample(color);

	PRT(_T("思考時間：先手=%d秒、後手=%d秒\n"), g_thoughtTime[0], g_thoughtTime[1]);
	PRT(_T("着手=(%2d,%2d)(%04x), 手数=%d,手番=%d,盤size=%d,komi=%.1f\n"),(bestmoveNode&0xff),(bestmoveNode>>8),bestmoveNode, curTesuu,blackTurn,boardSize,komi);
//	print_board();
	return bestmoveNode;
}

// 対局終了時に一度だけ呼ばれます。
// メモリの解放などが必要な場合にここに記述してください。
DLL_EXPORT void cgfgui_thinking_close(void)
{
	// ログ： 動いていることの確認。
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking_close") << endl;

	FreeConsole();
	// この下に、メモリの解放など必要な場合のコードを記述してください。
}