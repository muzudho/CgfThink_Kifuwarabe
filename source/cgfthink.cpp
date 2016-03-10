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
	#include "../header/h090_core/h090_core.h"
	#include "../header/h300_move/h300_move.h"
	#include "../header/h400_core/h400_board.h"
	#include "../header/h400_core/h400_explain.h"
	#include "../header/h400_core/h400_endgame.h"
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

}//extern "C"

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

	PRT(_T("cgfgui_thinking 開始☆！ boardSize=%d \n", boardSize));

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
	g_hama[BLACK]			= 0;	// 取った石の数
	g_hama[WHITE]			= 0;
	g_thoughtTime[BLACK]	= 0;	// 累計思考時間
	g_thoughtTime[WHITE]	= 0;	
	g_kouNode				= 0;	// コウになる位置。

	// 棋譜を進めていくぜ☆
	for (iTesuu =0; iTesuu<curTesuu; iTesuu++) {
		node	= kifu[iTesuu][0];	// 座標、y*256 + x の形で入っている
		color	= kifu[iTesuu][1];	// 石の色
		time	= kifu[iTesuu][2];	// 消費時間
		g_thoughtTime[iTesuu & 1] += time; // 手数の下1桁を見て [0]先手、[1]後手。
		if (MoveOne(node, color) != MOVE_SUCCESS) {
			// 動かせなければそこで止める。
			break;
		}
	}
	 
#if 0	// 中断処理を入れる場合のサンプル。0を1にすればコンパイルされます。
	for (i=0;i<300;i++) {				// 300*10ms = 3000ms = 3秒待ちます。
		YieldWindowsSystem();			// 一時的にWindowsに制御を渡します。
		if ( *g_pThinkStop != 0 ) break;	// 中断ボタンが押された場合。
		Sleep(10);						// 10ms(0.01秒)停止。
	}
#endif

	// モード別対応
	switch (endgameType)
	{
	// 「終局処理」なら
	case GAME_END_STATUS:
							return EndgameStatus		(endgameBoard);
	// 「図形を描く」なら
	case GAME_DRAW_FIGURE:
							return EndgameDrawFigure	(endgameBoard);
	// 「数値を書く」なら
	case GAME_DRAW_NUMBER:	
							return EndgameDrawNumber	(endgameBoard);
	// 通常の指し手
	default:				
							break;
	}

	//--------------------------------------------------------------------------------
	// 思考ルーチン
	//--------------------------------------------------------------------------------

	if (blackTurn) {
		color = BLACK;
	} else {
		color = WHITE;
	}
	// １手指します。
	bestmoveNode = Bestmove(color);

	PRT(_T("思考時間：先手=%d秒、後手=%d秒\n"), g_thoughtTime[0], g_thoughtTime[1]);
	PRT(_T("着手=(%2d,%2d)(%04x), 手数=%d,手番=%d,盤size=%d,komi=%.1f\n"),(bestmoveNode&0xff),(bestmoveNode>>8),bestmoveNode, curTesuu,blackTurn,boardSize,komi);
//	PrintBoard();
	return bestmoveNode;
}

DLL_EXPORT void cgfgui_thinking_close(
	void
)
{
	// ログ： 動いていることの確認。
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking_close") << endl;

	FreeConsole();
	// この下に、メモリの解放など必要な場合のコードを記述してください。
}