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
	#include "../header/cgfthink.h"
	#include "../header/library/functions.h"

	//--------------------------------------------------------------------------------
	// グローバル変数
	//--------------------------------------------------------------------------------

	// 思考中断フラグ。0で初期化されています。
	// GUIの「思考中断ボタン」を押された場合に1になります。
	int* g_pThinkStop = NULL;

	// 累計思考時間 [0]先手 [1]後手
	int g_sumThinkTime[2];


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
	extern int board[BOARD_MAX];

	// 左右、上下に移動する場合の動く量
	extern int dir4[4];

	// 既にこの石を検索した場合は1
	extern int check_board[BOARD_MAX];

	// 盤面のサイズ。19路盤では19、9路盤では9
	extern int board_size;

	// 取った石の数(再帰関数で使う)
	extern int ishi;

	// 連のダメの数(再帰関数で使う)
	extern int dame;

	// 次にコウになる位置
	extern int kou_z;

	// [0]... 黒が取った石の数, [1]...白が取った石の数
	extern int hama[2];

	// コンソールに出力するためのハンドル
	//static extern HANDLE hOutput;
	extern HANDLE hOutput;

}//extern "C" {

// 対局開始時に一度だけ呼ばれます。
DLL_EXPORT void cgfgui_thinking_init( int* ptr_stop_thinking)
{
	// muzudho: 
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"));
	outputfile << _T("called: cgfgui_thinking_init") << endl;

	// 中断フラグへのポインタ変数。
	// この値が1になった場合は思考を終了してください。
	g_pThinkStop = ptr_stop_thinking;

	// PRT()情報を表示するためのコンソールを起動する。
	AllocConsole();		// この行をコメントアウトすればコンソールは表示されません。
	SetConsoleTitle(_T("CgfgobanDLL Infomation Window"));
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	PRT(_T("デバッグ用の窓です。PRT()関数で出力できます。\n"));

	// この下に、メモリの確保など必要な場合のコードを記述してください。
}


// 対局終了時に一度だけ呼ばれます。
// メモリの解放などが必要な場合にここに記述してください。
DLL_EXPORT void cgfgui_thinking_close(void)
{
	// muzudho: 
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking_close") << endl;

	FreeConsole();
	// この下に、メモリの解放など必要な場合のコードを記述してください。
}

// 思考ルーチン。次の1手を返す。
// 本体から初期盤面、棋譜、手数、手番、盤のサイズ、コミ、が入った状態で呼ばれる。
DLL_EXPORT int cgfgui_thinking(
	int dll_init_board[],	// 初期盤面
	int dll_kifu[][3],		// 棋譜  [][0]...座標、[][1]...石の色、[][2]...消費時間（秒)
	int dll_tesuu,			// 手数
	int dll_black_turn,		// 手番(黒番...1、白番...0)
	int dll_board_size,		// 盤面のサイズ
	double dll_komi,		// コミ
	int dll_endgame_type,	// 0...通常の思考、1...終局処理、2...図形を表示、3...数値を表示。
	int dll_endgame_board[]	// 終局処理の結果を代入する。
)
{
	// muzudho: 
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking") << endl;

	int z,col,t,i,ret_z;

	// 現在局面を棋譜と初期盤面から作る
	for (i=0;i<BOARD_MAX;i++) board[i] = dll_init_board[i];	// 初期盤面をコピー
	board_size = dll_board_size;
	hama[0] = hama[1] = 0;
	g_sumThinkTime[0] = g_sumThinkTime[1] = 0;	// 累計思考時間を初期化
	kou_z = 0;
		
	for (i=0;i<dll_tesuu;i++) {
		z   = dll_kifu[i][0];	// 座標、y*256 + x の形で入っている
		col = dll_kifu[i][1];	// 石の色
		t   = dll_kifu[i][2];	// 消費時間
		g_sumThinkTime[i&1] += t;
		if ( move_one(z,col) != MOVE_SUCCESS ) break;
	}
	 
#if 0	// 中断処理を入れる場合のサンプル。0を1にすればコンパイルされます。
	for (i=0;i<300;i++) {				// 300*10ms = 3000ms = 3秒待ちます。
		PassWindowsSystem();			// 一時的にWindowsに制御を渡します。
		if ( *g_pThinkStop != 0 ) break;	// 中断ボタンが押された場合。
		Sleep(10);						// 10ms(0.01秒)停止。
	}
#endif

	// 終局処理、図形、数値を表示する場合
	if ( dll_endgame_type == GAME_END_STATUS  ) return endgame_status(dll_endgame_board);
	if ( dll_endgame_type == GAME_DRAW_FIGURE ) return endgame_draw_figure(dll_endgame_board);
	if ( dll_endgame_type == GAME_DRAW_NUMBER ) return endgame_draw_number(dll_endgame_board);

	// サンプルの思考ルーチンを呼ぶ
	if ( dll_black_turn ) col = BLACK;
	else                  col = WHITE;
	ret_z = think_sample(col);

	PRT(_T("思考時間：先手=%d秒、後手=%d秒\n"), g_sumThinkTime[0], g_sumThinkTime[1]);
	PRT(_T("着手=(%2d,%2d)(%04x), 手数=%d,手番=%d,盤size=%d,komi=%.1f\n"),(ret_z&0xff),(ret_z>>8),ret_z, dll_tesuu,dll_black_turn,dll_board_size,dll_komi);
//	print_board();
	return ret_z;
}
