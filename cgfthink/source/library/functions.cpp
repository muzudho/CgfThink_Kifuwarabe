
extern "C" {

	#include <time.h>		// clock() を使用するために。
	#include <windows.h>	// rand() 等を使用するために。
	#include <tchar.h> // Unicode対応の _T() 関数を使用するために。
	#include "../../header/cgfthink.h"

	//--------------------------------------------------------------------------------
	// グローバル変数
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
			TranslateMessage(&msg);						// keyboard input.
			DispatchMessage(&msg);
		}
	}

	#define PRT_LEN_MAX 256			// 最大256文字まで出力可

	// コンソールに出力するためのハンドル
	//static extern HANDLE hOutput;
	extern HANDLE hOutput;


	// printf()の代用関数。
	void PRT(const _TCHAR* fmt, ...)//const char *fmt
	{
		va_list ap;
		int len;
		//static char text[PRT_LEN_MAX];
		static _TCHAR text[PRT_LEN_MAX];
		DWORD nw;

		if (hOutput == INVALID_HANDLE_VALUE) return;
		va_start(ap, fmt);
		//len = _vsnprintf(text, PRT_LEN_MAX - 1, fmt, ap);
		len = _vsnwprintf(text, PRT_LEN_MAX - 1, fmt, ap);
		va_end(ap);

		if (len < 0 || len >= PRT_LEN_MAX) return;
		//WriteConsole(hOutput, text, (DWORD)strlen(text), &nw, NULL);
		WriteConsole(hOutput, text, (DWORD)wcslen(text), &nw, NULL);
	}

	// 乱数に近い評価関数。少し石を取りに行くように。
	int think_sample(int col)
	{
		int max, ret_z;
		int x, y, z, i, value, capture, z1, flag, safe, k;
		int un_col = UNCOL(col);

		// 実行するたびに違う値が得られるように現在の時刻で乱数を初期化
		srand((unsigned)clock());

		max = -1;
		ret_z = 0;
		for (y = 0; y<board_size; y++) for (x = 0; x<board_size; x++) {
			z = get_z(x, y);
			if (board[z]) continue;
			if (z == kou_z) continue;	// コウ

			value = rand() % 100;
			capture = safe = 0;
			for (i = 0; i<4; i++) {
				z1 = z + dir4[i];
				k = board[z1];
				if (k == WAKU) safe++;
				if (k == 0 || k == WAKU) continue;
				count_dame(z1);
				if (k == un_col && dame == 1) capture = 1;	// 敵石が取れる
				if (k == col && dame >= 2) safe++;			// 安全な味方に繋がる
				value += (k == un_col) * ishi * (10 / (dame + 1));
			}
			if (safe == 4) continue;	// 眼には打たない。
			if (capture == 0) {		// 石が取れない場合は実際に置いてみて自殺手かどうか判定
				int kz = kou_z;			// コウの位置を退避
				flag = move_one(z, col);
				board[z] = 0;
				kou_z = kz;
				if (flag == MOVE_SUICIDE) continue;	// 自殺手
			}
			//		PRT("x,y=(%d,%d)=%d\n",x,y,value);
			if (value > max) { max = value; ret_z = z; }
		}
		return ret_z;
	}

	// 現在の盤面を表示
	void print_board(void)
	{
		int x, y, z;
		_TCHAR* str[4] = { _T("・"), _T("●"), _T("○"), _T("＋") };

		for (y = 0; y<board_size + 2; y++) for (x = 0; x<board_size + 2; x++) {
			z = (y + 0) * 256 + (x + 0);
			PRT(_T("%s"), str[board[z]]);
			if (x == board_size + 1) PRT(_T("\n"));
		}
	}

	// 終局処理（サンプルでは簡単な判断で死石と地の判定をしています）
	int endgame_status(int endgame_board[])
	{
		int x, y, z, sum, i, k;
		int *p;

		for (y = 0; y<board_size; y++) for (x = 0; x<board_size; x++) {
			z = get_z(x, y);
			p = endgame_board + z;
			if (board[z] == 0) {
				*p = GTP_DAME;
				sum = 0;
				for (i = 0; i<4; i++) {
					k = board[z + dir4[i]];
					if (k == WAKU) continue;
					sum |= k;
				}
				if (sum == BLACK) *p = GTP_BLACK_TERRITORY;
				if (sum == WHITE) *p = GTP_WHITE_TERRITORY;
			}
			else {
				*p = GTP_ALIVE;
				count_dame(z);
				//			PRT("(%2d,%2d),ishi=%2d,dame=%2d\n",z&0xff,z>>8,ishi,dame);
				if (dame <= 1) *p = GTP_DEAD;
			}
		}
		return 0;
	}

	// 図形を描く
	int endgame_draw_figure(int endgame_board[])
	{
		int x, y, z;
		int *p;

		for (y = 0; y<board_size; y++) for (x = 0; x<board_size; x++) {
			z = get_z(x, y);
			p = endgame_board + z;
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
		int x, y, z;
		int *p;

		for (y = 0; y<board_size; y++) for (x = 0; x<board_size; x++) {
			z = get_z(x, y);
			p = endgame_board + z;
			*p = (rand() % 110) - 55;
		}
		return 0;
	}

	// (x,y)を1つの座標に変換
	int get_z(int x, int y)
	{
		return (y + 1) * 256 + (x + 1);
	}

	// 位置 tz におけるダメの数と石の数を計算。結果はグローバル変数に。
	void count_dame(int tz)
	{
		int i;

		dame = ishi = 0;
		for (i = 0; i<BOARD_MAX; i++) check_board[i] = 0;
		count_dame_sub(tz, board[tz]);
	}

	// ダメと石の数える再帰関数
	// 4方向を調べて、空白だったら+1、自分の石なら再帰で。相手の石、壁ならそのまま。
	void count_dame_sub(int tz, int col)
	{
		int z, i;

		check_board[tz] = 1;			// この石は検索済み	
		ishi++;							// 石の数
		for (i = 0; i<4; i++) {
			z = tz + dir4[i];
			if (check_board[z]) continue;
			if (board[z] == 0) {
				check_board[z] = 1;	// この空点は検索済み
				dame++;				// ダメの数
			}
			if (board[z] == col) count_dame_sub(z, col);	// 未探索の自分の石
		}
	}

	// 石を消す
	void del_stone(int tz, int col)
	{
		int z, i;

		board[tz] = 0;
		for (i = 0; i < 4; i++) {
			z = tz + dir4[i];
			if (board[z] == col) del_stone(z, col);
		}
	}

	// 手を進める。z ... 座標、col ... 石の色
	int move_one(int z, int col)
	{
		int i, z1, sum, del_z = 0;
		int all_ishi = 0;	// 取った石の合計
		int un_col = UNCOL(col);

		if (z == 0) {	// PASSの場合
			kou_z = 0;
			return MOVE_SUCCESS;
		}
		if (z == kou_z) {
			PRT(_T("move() Err: コウ！z=%04x\n"), z);
			return MOVE_KOU;
		}
		if (board[z] != 0) {
			PRT(_T("move() Err: 空点ではない！z=%04x\n"), z);
			return MOVE_EXIST;
		}
		board[z] = col;	// とりあえず置いてみる

		for (i = 0; i<4; i++) {
			z1 = z + dir4[i];
			if (board[z1] != un_col) continue;
			// 敵の石が取れるか？
			count_dame(z1);
			if (dame == 0) {
				hama[col - 1] += ishi;
				all_ishi += ishi;
				del_z = z1;	// 取られた石の座標。コウの判定で使う。
				del_stone(z1, un_col);
			}
		}
		// 自殺手を判定
		count_dame(z);
		if (dame == 0) {
			PRT(_T("move() Err: 自殺手! z=%04x\n"), z);
			board[z] = 0;
			return MOVE_SUICIDE;
		}

		// 次にコウになる位置を判定。石を1つだけ取った場合。
		kou_z = 0;	// コウではない
		if (all_ishi == 1) {
			// 取られた石の4方向に自分のダメ1の連が1つだけある場合、その位置はコウ。
			kou_z = del_z;	// 取り合えず取られた石の場所をコウの位置とする
			sum = 0;
			for (i = 0; i<4; i++) {
				z1 = del_z + dir4[i];
				if (board[z1] != col) continue;
				count_dame(z1);
				if (dame == 1 && ishi == 1) sum++;
			}
			if (sum >= 2) {
				PRT(_T("１つ取られて、コウの位置へ打つと、１つの石を2つ以上取れる？z=%04x\n"), z);
				return MOVE_FATAL;
			}
			if (sum == 0) kou_z = 0;	// コウにはならない。
		}
		return MOVE_SUCCESS;
	}


}