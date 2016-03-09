
extern "C" {

	#include <time.h>		// clock() を使用するために。
	#include <windows.h>	// rand() 等を使用するために。
	#include <tchar.h> // Unicode対応の _T() 関数を使用するために。
	#include "../../header/cgfthink.h"
	#include "../../header/core/board.h"
	#include "../../header/core/explain.h"
	#include "../../header/core/endgame.h"
	#include "../../header/library/think.h"

	//--------------------------------------------------------------------------------
	// 定義
	//--------------------------------------------------------------------------------

	#define PRT_LEN_MAX 256			// 最大256文字まで出力可

	//--------------------------------------------------------------------------------
	// グローバル変数
	//--------------------------------------------------------------------------------

	int g_board[BOARD_MAX];

	// 右、左、下、上に移動するのに使う加減値
	int g_dir4[4] = {
		+0x001,	// 右
		-0x001,	// 左
		+0x100,	// 下
		-0x100	// 上
	};

	// 既にこの石を検索した場合は1
	int g_checkedBoard[BOARD_MAX];

	// 盤面のサイズ。19路盤では19、9路盤では9
	int g_boardSize;

	// 取った石の数(再帰関数で使う)
	int g_ishi = 0;

	// 連のダメの数(再帰関数で使う)
	int g_dame = 0;

	// 次にコウになる位置。無ければ 0。
	int g_kouNode = 0;

	// ハマ。取った石の数のこと。[0]... 黒が取った石の数, [1]...白が取った石の数
	int g_hama[2];

	// コンソールに出力するためのハンドル
	HANDLE g_hConsoleWindow = INVALID_HANDLE_VALUE; //static HANDLE hOutput;

	//--------------------------------------------------------------------------------
	// 関数定義
	//--------------------------------------------------------------------------------

	// 一時的にWindowsに制御を渡します。
	// 思考中にこの関数を呼ぶと思考中断ボタンが有効になります。
	// 毎秒30回以上呼ばれるようにするとスムーズに中断できます。
	void PassWindowsSystem(void)
	{
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	// printf()の代用関数。
	void PRT(const _TCHAR* format, ...)
	{
		va_list argList;
		int len;
		static _TCHAR text[PRT_LEN_MAX];
		DWORD nw;

		if (g_hConsoleWindow == INVALID_HANDLE_VALUE) return;
		va_start(argList, format);
		len = _vsnwprintf(text, PRT_LEN_MAX - 1, format, argList);
		va_end(argList);

		if (len < 0 || len >= PRT_LEN_MAX) return;
		WriteConsole(g_hConsoleWindow, text, (DWORD)wcslen(text), &nw, NULL);
	}

	// 乱数に近い評価関数。少し石を取りに行くように。
	int think_sample(int col)
	{
		int max;
		int bestmoveNode;
		int x;
		int y;
		int node;
		int i;
		int value;
		int capture;
		int z1;
		int flag;
		int safe;
		int k;
		int un_col = UNCOL(col);

		// 実行するたびに違う値が得られるように現在の時刻で乱数を初期化
		srand((unsigned)clock());

		max = -1;
		bestmoveNode = 0;
		for (y = 0; y<g_boardSize; y++) for (x = 0; x<g_boardSize; x++) {
			node = ConvertNode(x, y);
			if (g_board[node]) continue;
			if (node == g_kouNode) continue;	// コウ

			value = rand() % 100;
			capture = safe = 0;
			for (i = 0; i<4; i++) {
				z1 = node + g_dir4[i];
				k = g_board[z1];
				if (k == WAKU) safe++;
				if (k == 0 || k == WAKU) continue;
				count_dame(z1);
				if (k == un_col && g_dame == 1) capture = 1;	// 敵石が取れる
				if (k == col && g_dame >= 2) safe++;			// 安全な味方に繋がる
				value += (k == un_col) * g_ishi * (10 / (g_dame + 1));
			}
			if (safe == 4) continue;	// 眼には打たない。
			if (capture == 0) {		// 石が取れない場合は実際に置いてみて自殺手かどうか判定
				int kz = g_kouNode;			// コウの位置を退避
				flag = move_one(node, col);
				g_board[node] = 0;
				g_kouNode = kz;
				if (flag == MOVE_SUICIDE) continue;	// 自殺手
			}
			//		PRT("x,y=(%d,%d)=%d\n",x,y,value);
			if (value > max) { max = value; bestmoveNode = node; }
		}
		return bestmoveNode;
	}

	// 現在の盤面を表示
	void print_board(void)
	{
		int x;
		int y;
		int node;
		_TCHAR* str[4] = { _T("・"), _T("●"), _T("○"), _T("＋") };

		for (y = 0; y<g_boardSize + 2; y++) for (x = 0; x<g_boardSize + 2; x++) {
			node = (y + 0) * 256 + (x + 0);
			PRT(_T("%s"), str[g_board[node]]);
			if (x == g_boardSize + 1) PRT(_T("\n"));
		}
	}

	// 終局処理（サンプルでは簡単な判断で死石と地の判定をしています）
	int endgame_status(int endgame_board[])
	{
		int x;
		int y;
		int node;
		int sum;
		int i;
		int k;
		int *p;

		for (y = 0; y<g_boardSize; y++) for (x = 0; x<g_boardSize; x++) {
			node = ConvertNode(x, y);
			p = endgame_board + node;
			if (g_board[node] == 0) {
				*p = GTP_DAME;
				sum = 0;
				for (i = 0; i<4; i++) {
					k = g_board[node + g_dir4[i]];
					if (k == WAKU) continue;
					sum |= k;
				}
				if (sum == BLACK) *p = GTP_BLACK_TERRITORY;
				if (sum == WHITE) *p = GTP_WHITE_TERRITORY;
			}
			else {
				*p = GTP_ALIVE;
				count_dame(node);
				//			PRT("(%2d,%2d),ishi=%2d,dame=%2d\n",z&0xff,z>>8,ishi,dame);
				if (g_dame <= 1) *p = GTP_DEAD;
			}
		}
		return 0;
	}

	// 図形を描く
	int endgame_draw_figure(int endgame_board[])
	{
		int x;
		int y;
		int node;
		int *p;

		for (y = 0; y<g_boardSize; y++) for (x = 0; x<g_boardSize; x++) {
			node = ConvertNode(x, y);
			p = endgame_board + node;
			if ((rand() % 2) == 0) *p = FIGURE_NONE;
			else {
				if (rand() % 2) *p = FIGURE_BLACK;
				else              *p = FIGURE_WHITE;
				*p |= (rand() % 9) + 1;
			}
		}
		return 0;
	}

	// 数値を書く(0は表示されない)
	int endgame_draw_number(int endgame_board[])
	{
		int x;
		int y;
		int node;
		int *p;

		for (y = 0; y<g_boardSize; y++) for (x = 0; x<g_boardSize; x++) {
			node = ConvertNode(x, y);
			p = endgame_board + node;
			*p = (rand() % 110) - 55;
		}
		return 0;
	}

	// (x,y)を1つの座標に変換
	int ConvertNode(int x, int y)
	{
		return (y + 1) * 256 + (x + 1);
	}

	// 位置 tz におけるダメの数と石の数を計算。結果はグローバル変数に。
	void count_dame(int tz)
	{
		int i;

		g_dame = g_ishi = 0;
		for (i = 0; i<BOARD_MAX; i++) g_checkedBoard[i] = 0;
		count_dame_sub(tz, g_board[tz]);
	}

	// ダメと石の数える再帰関数
	// 4方向を調べて、空白だったら+1、自分の石なら再帰で。相手の石、壁ならそのまま。
	void count_dame_sub(int tNode, int col)
	{
		int node;
		int i;

		g_checkedBoard[tNode] = 1;			// この石は検索済み	
		g_ishi++;							// 石の数
		for (i = 0; i<4; i++) {
			node = tNode + g_dir4[i];
			if (g_checkedBoard[node]) continue;
			if (g_board[node] == 0) {
				g_checkedBoard[node] = 1;	// この空点は検索済み
				g_dame++;				// ダメの数
			}
			if (g_board[node] == col) count_dame_sub(node, col);	// 未探索の自分の石
		}
	}

	// 石を消す
	void del_stone(int tNode, int col)
	{
		int node;
		int i;

		g_board[tNode] = 0;
		for (i = 0; i < 4; i++) {
			node = tNode + g_dir4[i];
			if (g_board[node] == col) del_stone(node, col);
		}
	}

	// 手を進める。
	int move_one(
		int node	,	// 座標
		int col			// 石の色
	)
	{
		int i;
		int z1;
		int sum;
		int delNode = 0;
		int all_ishi = 0;	// 取った石の合計
		int un_col = UNCOL(col);

		if (node == 0) {	// PASSの場合
			g_kouNode = 0;
			return MOVE_SUCCESS;
		}
		if (node == g_kouNode) {
			PRT(_T("move() Err: コウ！z=%04x\n"), node);
			return MOVE_KOU;
		}
		if (g_board[node] != 0) {
			PRT(_T("move() Err: 空点ではない！z=%04x\n"), node);
			return MOVE_EXIST;
		}
		g_board[node] = col;	// とりあえず置いてみる

		for (i = 0; i<4; i++) {
			z1 = node + g_dir4[i];
			if (g_board[z1] != un_col) continue;
			// 敵の石が取れるか？
			count_dame(z1);
			if (g_dame == 0) {
				g_hama[col - 1] += g_ishi;
				all_ishi += g_ishi;
				delNode = z1;	// 取られた石の座標。コウの判定で使う。
				del_stone(z1, un_col);
			}
		}
		// 自殺手を判定
		count_dame(node);
		if (g_dame == 0) {
			PRT(_T("move() Err: 自殺手! z=%04x\n"), node);
			g_board[node] = 0;
			return MOVE_SUICIDE;
		}

		// 次にコウになる位置を判定。石を1つだけ取った場合。
		g_kouNode = 0;	// コウではない
		if (all_ishi == 1) {
			// 取られた石の4方向に自分のダメ1の連が1つだけある場合、その位置はコウ。
			g_kouNode = delNode;	// 取り合えず取られた石の場所をコウの位置とする
			sum = 0;
			for (i = 0; i<4; i++) {
				z1 = delNode + g_dir4[i];
				if (g_board[z1] != col) continue;
				count_dame(z1);
				if (g_dame == 1 && g_ishi == 1) sum++;
			}
			if (sum >= 2) {
				PRT(_T("１つ取られて、コウの位置へ打つと、１つの石を2つ以上取れる？z=%04x\n"), node);
				return MOVE_FATAL;
			}
			if (sum == 0) g_kouNode = 0;	// コウにはならない。
		}
		return MOVE_SUCCESS;
	}


}