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


#include <windows.h> // コンソールへの出力等
#include <tchar.h> // Unicode対応の _T() 関数を使用するために。

#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h300_move____/n300_100_move.h"
#include "../../header/h390_explain_/n390_100_explain.h"
#include "../../header/h480_view____/n480_100_boardView.h"
#include "../../header/h490_endgame_/n490_100_endgame.h"
#include "../../header/h700_think___/n700_200_think.h"
//#include "../../header/h800_cgfthink/n800_100_cppBoard.h"
#include "../../header/h800_cgfthink/n800_200_cgfthink.h"



//--------------------------------------------------------------------------------
// グローバル変数
//--------------------------------------------------------------------------------

// 思考中断フラグ。0で初期化されています。
// GUIの「思考中断ボタン」を押された場合に1になります。
Cgfthink g_cgfthink;


//--------------------------------------------------------------------------------
// 関数宣言まとめ　別のアプリケーションから呼び出されるもの
//--------------------------------------------------------------------------------

// 対局開始時に一度だけ呼ばれます。
//extern "C"
DLL_EXPORT void cgfgui_thinking_init(
	int* pThinkStoped	// 普段は0。中止ボタンが押されたときに 1 になります。この値が1になった場合は思考を終了してください。
)
{
	// ログ： 動いていることの確認。
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"));
	outputfile << _T("called: cgfgui_thinking_init") << endl;

	//--------------------
	// ポインタの受け渡し
	//--------------------
	g_cgfthink.pThinkStoped = pThinkStoped;

	// PRT()情報を表示するためのコンソールを起動する。
	AllocConsole();		// この行をコメントアウトすればコンソールは表示されません。
	SetConsoleTitle(_T("CgfgobanDLL Infomation Window"));

	// コンソールに出力するためのハンドル
	//HANDLE hConsoleWindow = INVALID_HANDLE_VALUE; //static HANDLE hOutput;
	HANDLE hConsoleWindow = GetStdHandle(STD_OUTPUT_HANDLE);
	Core::PRT(hConsoleWindow, _T("デバッグ用の窓だぜ☆（＾ｑ＾）　PRT()関数で出力できるんだぜ☆\n"));

	// この下に、メモリの確保など必要な場合のコードを記述してください。
}

// 思考ルーチン。次の1手の座標を返す。PASSの場合0。
// GUIから現在の局面の情報が渡される。
// また、終局処理の場合は、終局判断の結果を返す。
//extern "C"
DLL_EXPORT int cgfgui_thinking(
	int		initBoard[]		,	// 初期盤面（置碁の場合は、ここに置石が入る）
	int		kifu[][3]		,	// 棋譜	[手数][0]...座標
								//		[手数][1]...石の色
								//		[手数][2]...消費時間（秒)
								// 手数は 0 から始まり、curTesuu の1つ手前まである。
	int		curTesuu		,	// 現在の手数
	int		flgBlackTurn	,	// 黒手番フラグ(黒番...1、白番...0)。ここだけ定数と違ってややこしい。
	int		boardSize		,	// 盤面のサイズ
	double	komi			,	// コミ
	int		endgameType		,	// 0...通常の思考、1...終局処理、2...図形を表示、3...数値を表示。
	int		endgameBoard[]		// 終局処理の結果を代入する。
)
{
	// デバッグ用の窓です。PRT()関数で出力できます。
	HANDLE hConsoleWindow = GetStdHandle(STD_OUTPUT_HANDLE);

	// ログ： 動いていることの確認。
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking") << endl;

	Core::PRT(hConsoleWindow, _T("cgfgui_thinking 開始☆！ boardSize=%d \n"), boardSize);

	//--------------------
	// 何路盤
	//--------------------
	Board* pBoard = new Board();
	pBoard->size = boardSize;

	// 現在局面を棋譜と初期盤面から作る
	for (int iNode = 0; iNode < BOARD_MAX; iNode++) {
		pBoard->table[iNode] = initBoard[iNode];	// 初期盤面をコピー
	}

	/*
	// [&hConsoleWindow]を付けておくと、ブロックの外側の hConsoleWindow 変数を参照できるぜ☆（＾ｑ＾）
	pBoard->ForeachAllNodesWithWaku( [&hConsoleWindow](int node) {
		int x, y;
		Board::ConvertToXy(x, y, node);
		Core::PRT(hConsoleWindow, _T("(^q^) %d =(%d %d) \n"), node, x, y);
	});
	// */


	//--------------------
	// 初期化
	//--------------------
	int node;			// 囲碁盤上の交点（将棋盤でいうマス目）
	int bestmoveNode;	// コンピューターが打つ交点。
	int color;			// 石の色
	int time;			// 消費時間
	int iTesuu;

	// 累計思考時間 [0]先手 [1]後手。　黒白とは限らない？
	int thoughtTime[2] = { 0, 0 };

	// 棋譜を進めていくぜ☆
	for (iTesuu =0; iTesuu<curTesuu; iTesuu++) {
		node	= kifu[iTesuu][0];	// 座標、y*256 + x の形で入っている
		color	= kifu[iTesuu][1];	// 石の色
		time	= kifu[iTesuu][2];	// 消費時間
		thoughtTime[iTesuu & 1] += time; // 手数の下1桁を見て [0]先手、[1]後手。
		if (Move::MoveOne(hConsoleWindow, node, color, pBoard) != MOVE_SUCCESS) {
			// 動かせなければそこで止める。（エラーがあった？？）
			Core::PRT(hConsoleWindow, _T("棋譜を進められなかったので止めた☆ \n"));
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
							return Endgame::EndgameStatus		(endgameBoard, pBoard);
	// 「図形を描く」なら
	case GAME_DRAW_FIGURE:
							return Endgame::EndgameDrawFigure	(endgameBoard, pBoard);
	// 「数値を書く」なら
	case GAME_DRAW_NUMBER:	
							return Endgame::EndgameDrawNumber	(endgameBoard, pBoard);
	// 通常の指し手
	default:				
							break;
	}

	//--------------------------------------------------------------------------------
	// 思考ルーチン
	//--------------------------------------------------------------------------------

	if (flgBlackTurn) {
		color = BLACK;
	} else {
		color = WHITE;
	}
	// １手指します。
	bestmoveNode = Think::Bestmove(hConsoleWindow, color, pBoard);

	Core::PRT(hConsoleWindow, _T("思考時間：先手=%d秒、後手=%d秒\n"), thoughtTime[0], thoughtTime[1]);
	Core::PRT(hConsoleWindow, _T("着手=(%2d,%2d)(%04x), 手数=%d,手番=%d,盤size=%d,komi=%.1f\n"),(bestmoveNode&0xff),(bestmoveNode>>8),bestmoveNode, curTesuu,flgBlackTurn,boardSize,komi);
	//PrintBoard();

	//PRT(_T("a"));

	// FIXME: メモリリークしているはず☆（＾ｑ＾）
	//delete pBoard;

	//PRT(_T("b"));

	//int* pointer = new int;

	//PRT(_T("c"));

	//delete pointer;

	//PRT(_T("d"));

	return bestmoveNode;
}

// 対局終了時に一度だけ呼ばれます。
// メモリの解放などが必要な場合にここに記述してください。
//extern "C"
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