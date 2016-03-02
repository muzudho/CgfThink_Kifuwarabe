/*** cgf_win.cpp --- Windowsに依存する部分を主に含む --- ***/
//#pragma warning( disable : 4115 4201 4214 4057 4514 )  // 警告メッセージ 4201 を無効にする。
#include <Windows.h>
#include <MMSystem.h>	// PlaySound()を使う場合に必要。Windows.h の後に組み込む。さらに WINMM.LIB を追加リンク
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include <CommCtrl.h>	// ToolBar を使用するための定義 comctl32.lib をリンクする必要あり
#if (_MSC_VER >= 1100)	// VC++5.0以降でサポート
#include <zmouse.h>		// IntelliMouseをサポートするために無理やりリンク（大丈夫？）
#endif

#include "cgf.h"
#include "cgf_rc.h"
#include "cgf_wsk.h"
#include "cgf_pipe.h"

//#include "cgfthink.h"





int endgame_board[BANMAX];	// 終局処理、図形、数値表示用の盤面
int endgame_board_type;		// 現在、終局、図形、数値のどれを表示中か

/* プロトタイプ宣言 ---------------------- 以下Windows固有の宣言を含む */

HWND ghWindow;					/* WINDOWSのハンドル  */
HINSTANCE ghInstance;			/* インスタンス       */
HWND hPrint = NULL;				/* 情報出力用のモードレスダイアログ */
extern HWND hWndToolBar;		// TOOLBAR用のハンドル
HWND hWndDraw;
HWND hWndMain;
HACCEL hAccel;					// アクセラレータキーのテープルのハンドル
HWND hWndToolBar;				// ToolBar

#define PMaxY 400				// 情報画面の縦サイズ（全長）
#define PHeight 42				// 画面の標準高さ(32が通常) 
#define PRetX 100				// 100行で改行
#define PMaxX 256				// 1行の最大文字数(意味なし？）

char cPrintBuff[PMaxY][PMaxX];	// バッファ			
static int nHeight;
static int nWidth;
static int PRT_y;
static int PRT_x = 0;

int ID_NowBanColor = ID_BAN_1;	// デフォルトの盤の色
int fFileWrite = 0;				// ファイルに書き込むフラグ
int UseDefaultScreenSize;		// 固定の画面サイズを使う
char cDragFile[MAX_PATH];		// ドラッグされた開くべきファイル名 ---> 今はダブルクリックで開くときに
char sCgfDir[MAX_PATH];			// cgfgoabn.exe があるディレクトリ
int SoundDrop = 0;				// 着手音なし
int fInfoWindow = 0;			// 情報表示あり
int fPassWindows;				// Windowsに処理を渡してる。表示以外を全てカット
int fChineseRule = 0;			// 中国ルールの場合
int thinking_utikiri_flag;
int turn_player[2] = { 0,1 };	// 対局者の種類が入る。基本は 黒:人間, 白:Computer
char sDllName[] = "CPU";		// 標準の名前です。適当に編集して下さい。
//char sGnugoDir[MAX_PATH];		// gnugo.exeがあるフルパス
int fGnugoLifeDeathView=0;		// gnugoの判定で死活表示をする
int fATView = 0;				// 座標表示をA～Tに。チェス形式（A-TでIがない）



/*
2006/03/01 1.02 Visual Studio 2005 Express Edtionでもコンパイルできるように。
2007/01/22      名前やGTPエンジンPATH,nngsサーバ名を複数記憶するように。
2007/09/19 1.03 MoGoが動くように。stderrを無視するように。version名が長すぎる場合は削除
2008/01/31 1.04 UEC杯のnngsに対応。GTPエンジンが存在チェックを無視。Program Filesのように空白をはさむと認識できなかった。path欄を大きく。
2009/06/25 1.05 nngsの切れ負けの時間(分)を設定可能に。
2011/11/16 1.06 Win64でのコンパイルエラーを解消。警告はたくさん出ます・・・。
2013/11/14 1.07 コミを7.0を指定可能に。
2015/03/16 1.08 nngsでhumanが投了を送れるように。AT座標表示
*/
char sVersion[32] = "CgfGoBan 1.08";	// CgfGobanのバージョン
char sVerDate[32] = "2015/03/16";		// 日付

int dll_col     = 2;	// DLLの色。通信対戦時に利用。
int fNowPlaying = 0;	// 現在対局中かどうか。

int total_time[2];		// 思考時間。先手[0]、後手[1]の累計思考時間。

int RetZ;				// 通信で受け取る手
int RetComm = 0;		// 通信状態を示す。0...Err, 1...手, 2...OK, 3...NewGame, 4...Answer

int nCommNumber = 1;	// COMポートの番号。

int nHandicap = 0;		// 置石はなし
int fAutoSave = 0;		// 棋譜の自動セーブ（通信対戦時）
int RealMem_MB;			// 実メモリ
int LatestScore = 0;	// 最後に計算した地
int fGtpClient = 0;		// GTPの傀儡として動く場合

static clock_t gct1;	// 時間計測用

#define RECENT_FILE_MAX 20		// 最近開いたファイルの最大数。
char sRecentFile[RECENT_FILE_MAX][TMP_BUF_LEN];	// ファイルの絶対パス
int nRecentFile = 0;			// ファイルの数
#define IDM_RECENT_FILE 2000	// メッセージの先頭
#define RECENT_POS 12			// 上から12番目に挿入

char sYourCPU[TMP_BUF_LEN];		// このマシンのCPUとFSB

#define RGB_BOARD_MAX 8	// 盤面色の数

COLORREF rgbBoard[RGB_BOARD_MAX] = {	// 盤面の色
	RGB(231,134, 49),	// 基本(こげ茶)
	RGB(255,128,  0),	// 淡
	RGB(198,105, 57),	// 濃い木目色
	RGB(192, 64,  9),	// 濃
	RGB(255,188,121),	// ゴム板（彩の色）
	RGB(  0,255,  0),	// 緑
	RGB(255,255,  0),	// 黄
	RGB(255,255,255),	// 白
};

COLORREF rgbBack  = RGB(  0,  0,  0);	// 黒、背景の色
//COLORREF rgbBack  = RGB(231,255,231);	// 背景の色
//COLORREF rgbBack  = RGB(247,215,181);	// 背景の色
//COLORREF rgbBack  = RGB(  0,128,  0);	// 濃緑
//COLORREF rgbBack  = RGB(  5, 75, 49);	// 濃緑(彩の背景)

COLORREF rgbText  = RGB(255,255,255);	// 位置、名前の色

COLORREF rgbBlack = RGB(  0,  0,  0);
COLORREF rgbWhite = RGB(255,255,255);

#define NAME_LIST_MAX 30
char sNameList[NAME_LIST_MAX][TMP_BUF_LEN];	// 対局者の名前の候補
int nNameList;								// 名前候補の数
//int nNameListSelect[2];					// 現在の対局者の番号

#define GTP_PATH_MAX 30
char sGtpPath[GTP_PATH_MAX][TMP_BUF_LEN];	// GTPエンジンの場所
int nGtpPath;								// 名前候補の数

#define NNGS_IP_MAX 30
string[] sNngsIP;	// nngsのサーバ名 [NNGS_IP_MAX]
int nNngsIP;							// 候補の数

#define GUI_DLL_MOVE  0
#define GUI_USER_PASS 1
#define GUI_GTP_MOVE  2

// SGF にあるデータ
char sPlayerName[2][TMP_BUF_LEN];	// 対局者の名前。[0] 黒、[1] 白。
char sDate[TMP_BUF_LEN];			// 日付
char sPlace[TMP_BUF_LEN];			// 場所
char sEvent[TMP_BUF_LEN];			// 大会名
char sRound[TMP_BUF_LEN];			// 何回戦か
char sKomi[TMP_BUF_LEN];			// こみ
char sTime[TMP_BUF_LEN];			// 時間
char sGameName[TMP_BUF_LEN];		// 対局名
char sResult[TMP_BUF_LEN];			// 結果

#define CGF_ICON "CGF1"


#ifdef CGF_E
char szTitleName[] = "CgfGoBan";
char szInfoWinName[] = "Information Window";
#else
char szTitleName[] = "CGF碁盤";
char szInfoWinName[] = "情報表示";
#endif





// 現在、通信対戦中か
int IsCommunication(void)
{
	int i,n;

	for (i=0;i<2;i++) {
		n = turn_player[i];
		if ( n == PLAYER_NNGS || n == PLAYER_SGMP ) return 1;
	}
	return 0;
}

// 対局中断処理。通信対戦中ならポートなどを閉じる。
void StopPlayingMode(void)
{
	if ( IsCommunication() ) cgf_win_PlayDialog.CloseConnection();
	SendMessage(ghWindow,WM_COMMAND, IDM_BREAK, 0L);
	PaintUpdate();
}

// 着手を通信で送ったり、次の手番に移ったり
int SendUpdateSetKifu(HWND hWnd, int z, int gui_move )
{
	if ( IsCommunication() ) SendMove(z);
	if ( gui_move != GUI_GTP_MOVE && IsGnugoBoard() ) UpdateGnugoBoard(z);

	int ret = set_kifu_time(z,GetTurnCol(),(int)GetSpendTime(gct1));
	if ( ret ) {
		GameStopMessage(hWnd,ret,z);
		StopPlayingMode();
		return ret;
	}

	if ( gui_move == GUI_USER_PASS ) PaintUpdate();
	else {
		if ( z == 0 && turn_player[GetTurnCol()-1] == PLAYER_HUMAN ) {
			MessageBox(hWnd,"Computer PASS","Pass",MB_OK);
		}
		if ( SoundDrop ) sndPlaySound("stone.wav", SND_ASYNC | SND_NOSTOP);	// MMSYSTEM.H をインクルードする必要あり。またWinMM.libをリンク
	}

	// 次の手番の処理に移る
	PostMessage(hWnd,WM_COMMAND,IDM_NEXT_TURN,0L);
	return 0;
}

/*
// 置石は2子～9子までは下のように。

＋＋●	// 2子  
＋＋＋
●＋＋

＋＋●	// 3子
＋＋＋
●＋●

●＋●	// 5子
＋●＋
●＋●

●＋●	// 6子
●＋●
●＋●

●＋●	// 7子
●●●
●＋●

●●●	// 8子
●＋●
●●●

＋＋＋＋●	// 10子
＋●●●＋
＋●●●＋
＋●●●＋
＋＋＋＋＋

＋＋＋＋●	// 12子
＋●●●＋
＋●●●＋
＋●●●＋
●＋＋＋●
*/
// 置石を設定
void SetHandicapStones(void)
{
	int i,k;
	
	SendMessage(ghWindow,WM_COMMAND,IDM_BAN_CLEAR,0L);	// 盤面初期化
	k = nHandicap;
	if ( ban_size == BAN_19 ) {
		ban[0x0410] = 1;		// 対角線には必ず置く
		ban[0x1004] = 1;	
		if ( k >= 4 ) {
			ban[0x0404] = 1;	// 4子以上は星は全部
			ban[0x1010] = 1;	
		}
		if ( k >= 6 ) {
			ban[0x0a04] = 1;	// 6子以上は左右中央の星
			ban[0x0a10] = 1;	
		}
		if ( k >= 8 ) {
			ban[0x040a] = 1;	// 8子以上は上下中央の星
			ban[0x100a] = 1;
		}
		if ( k == 3 ) ban[0x1010] = 1;
		if ( k == 5 || k == 7 || k >= 9 ) ban[0x0a0a] = 1;
	} else if ( ban_size == 13 ) {
		ban[0x040a] = 1;		// 対角線には必ず置く
		ban[0x0a04] = 1;	
		if ( k >= 4 ) {
			ban[0x0404] = 1;	// 4子以上は星は全部
			ban[0x0a0a] = 1;	
		}
		if ( k >= 6 ) {
			ban[0x0704] = 1;	// 6子以上は左右中央の星
			ban[0x070a] = 1;	
		}
		if ( k >= 8 ) {
			ban[0x0407] = 1;	// 8子以上は上下中央の星
			ban[0x0a07] = 1;
		}
		if ( k == 3 ) ban[0x0a0a] = 1;
		if ( k == 5 || k == 7 || k >= 9 ) ban[0x0707] = 1;
	} else if ( ban_size == BAN_9 ) {
		ban[0x0307] = 1;		// 対角線には必ず置く
		ban[0x0703] = 1;	
		if ( k >= 4 ) {
			ban[0x0303] = 1;	// 4子以上は星は全部
			ban[0x0707] = 1;	
		}
		if ( k >= 6 ) {
			ban[0x0503] = 1;	// 6子以上は左右中央の星
			ban[0x0507] = 1;	
		}
		if ( k >= 8 ) {
			ban[0x0305] = 1;	// 8子以上は上下中央の星
			ban[0x0705] = 1;
		}
		if ( k == 3 ) ban[0x0707] = 1;
		if ( k == 5 || k == 7 || k >= 9 ) ban[0x0505] = 1;
	}

	for (i=0;i<BANMAX;i++) {
		ban_start[i] = ban[i];	// 開始盤面にも代入
	}
	SendMessage(ghWindow,WM_COMMAND,IDM_SET_BOARD,0L);	// 盤面初期化
}


/* 情報表示ダイアログ */
void OpenPrintWindow(HWND hWnd)
{
	WNDCLASS wc;
	HBRUSH hBrush;
//	RECT rc;
	static int bFirstTime = TRUE;

	if ( bFirstTime ) {
		bFirstTime = FALSE;
		wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;//PARENTDC;
		wc.lpszClassName = "PrintClass";
		wc.hInstance     = ghInstance;
		wc.lpfnWndProc   = PrintProc;
		wc.hCursor       = LoadCursor( NULL, IDC_ARROW);
		wc.hIcon         = LoadIcon( ghInstance, CGF_ICON );
		wc.lpszMenuName  = NULL;
		hBrush = CreateSolidBrush( RGB( 0,0,128 ) );	// 背景を緑に(wc.中のオブジェクトは終了時に自動的に削除される）
		wc.hbrBackground = hBrush;//	GetStockObject(WHITE_BRUSH);
//		wc.hbrBackground = GetStockObject(WHITE_BRUSH);
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		if ( !RegisterClass(&wc) ) MessageBox(ghWindow,"情報表示Winodwの登録に失敗","エラー",MB_OK);
	}
	/********** ウインドウの作成 ********/
//	GetWindowRect( hWnd, &rc );	// 現在の画面の絶対位置を取得。
//	PRT("%d,%d,%d,%d\n",rc.left, rc.top,rc.right, rc.bottom);

	hPrint = CreateWindow(
				"PrintClass",						/* Class name.   */                                  
				szInfoWinName,						/* Title.        */
				WS_VSCROLL | WS_OVERLAPPEDWINDOW,	/* Style bits.   */
//				rc.right, rc.top,					/* X,Y  表示座標 */
				640, 20,							/* X,Y  表示座標 */
//				600, 50,							/* X,Y  表示座標 */
//				700,560,							/* CX, CY 大きさ */
				700,PHeight*17,						/* CX, CY 大きさ */
				hWnd,								/* 親はなし      */
				NULL,								/* Class Menu.   */
				ghInstance,							/* Creator       */
				NULL								/* Params.       */
			   );

	LoadMainWindowPlacement(hPrint,2);	// レジストリから情報を取得
	ShowWindow( hPrint, SW_SHOW);
}

LRESULT CALLBACK PrintProc( HWND hWnd, UINT msg, WPARAM wParam, Int64 lParam )
{
	int i,y=0;
	PAINTSTRUCT ps;
	HMENU hMenu;
	TEXTMETRIC tm;
	static HDC hDC;
	static HFONT hFont,hOldFont;

	switch( msg ) {
		case WM_CREATE:
			PRT_y = PMaxY - PHeight;		// 基本は25行表示
			SetScrollRange( hWnd, SB_VERT, 0, PRT_y, FALSE);	
			SetScrollPos( hWnd, SB_VERT, PRT_y, FALSE );
			hDC = GetDC(hWnd);

//			hFont = GetStockObject(OEM_FIXED_FONT);
//			hFont = GetStockObject(SYSTEM_FONT);
//			hFont = GetStockObject(ANSI_FIXED_FONT);

			hFont = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);	// Win98で文字幅が崩れないように
			hOldFont = (HFONT)SelectObject(hDC,hFont);

			GetTextMetrics(hDC, &tm);
			nHeight = tm.tmHeight;
			nHeight -= 2;	// わざと2ドット縦を減らしている。情報量が増える 上のFIXED_FONTで使用
			nWidth  = tm.tmAveCharWidth;
			SetTextColor( hDC, RGB(255,255,255) );	// 文字を白に
			SetBkColor( hDC, RGB(0,0,128) );		// 背景を濃い青に
			SetBkMode( hDC, TRANSPARENT);			// 背景を消さない（文字の上下が1ドット消されるのを防ぐ）
			ReleaseDC( hWnd, hDC );
			break;

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			for (i=PRT_y; i<PMaxY; i++) {
				TextOut(hDC,0,(i-PRT_y)*(nHeight),cPrintBuff[i],strlen(cPrintBuff[i]));
			}
			EndPaint(hWnd, &ps);
			break;
		
		case WM_KEYDOWN:
		case WM_VSCROLL:
			y = PRT_y;
			switch ( (int)LOWORD(wParam) ) {
				case SB_THUMBPOSITION:	// サム移動
					PRT_y = (short int) HIWORD( wParam );
//					PRT("wParam=%x ,(short int)HIWORD()=%d, lParam=%x\n",wParam,(short int )HIWORD(wParam ),lParam );
					break;
				case SB_LINEDOWN:
				case VK_DOWN:
					PRT_y++;
					break;
				case SB_PAGEDOWN:
				case VK_NEXT:
					PRT_y += PHeight - 1;
					break;
				case SB_LINEUP:
				case VK_UP:
					PRT_y--;
					break;
				case SB_PAGEUP:
				case VK_PRIOR:
					PRT_y -= PHeight - 1;
					break;
				case VK_HOME:
					PRT_y = 0;
					break;
				case VK_END:
					PRT_y = PMaxY;
					break;
			}

#if (_MSC_VER >= 1100)		// VC++5.0以降でサポート
		case WM_MOUSEWHEEL:	// ---> これは無理やり <zmouse.h> で定義している。
			if ( msg == WM_MOUSEWHEEL ) {
				y = PRT_y;
				PRT_y -= ( (short) HIWORD(wParam) / WHEEL_DELTA ) * 3; 
			}
#endif
			if ( PRT_y <= 0 ) PRT_y = 0;
			if ( PRT_y >= PMaxY - PHeight ) PRT_y = PMaxY - PHeight;
			
			SetScrollPos( hWnd, SB_VERT, PRT_y, TRUE);
			if ( y != PRT_y ) ScrollWindow( hWnd, 0, (nHeight) * (y-PRT_y), NULL, NULL );
			break;

		case WM_LBUTTONDOWN:
			break;
		
		case WM_LBUTTONUP:
			break;

		case WM_RBUTTONDOWN:
			{	POINT pt;
				HMENU hSubmenu;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
//				PRT("右クリックされたね (x,y)=%d,%d\n",pt.x,pt.y);
				hMenu = LoadMenu(ghInstance, "PRT_POPUP");
				hSubmenu = GetSubMenu(hMenu, 0);
			
				ClientToScreen(hWnd, &pt);
				TrackPopupMenu(hSubmenu, TPM_LEFTALIGN, pt.x, pt.y, 0, ghWindow, NULL);	// POPUPの親をメインにしないとメッセージが正しく飛ばない。
				DestroyMenu(hMenu);
			}
			break;

		case WM_CLOSE:
			fInfoWindow = 0;	// ユーザがPrintWindowのみを閉じようとした場合。
			return( DefWindowProc( hWnd, msg, wParam, lParam) );

		case WM_DESTROY:
			hMenu = GetMenu(ghWindow);
			CheckMenuItem(hMenu, IDM_PRINT, MF_UNCHECKED);
			DeleteObject(SelectObject(hDC, hOldFont));	/* フォントを解放 */
			if ( SaveMainWindowPlacement(hPrint,2) == 0 ) PRT("レジストリ２のエラー（書き込み）\n");
			hPrint = NULL;
			break;

		default:
			return( DefWindowProc( hWnd, msg, wParam, lParam) );
	}
	return 0L;
}


#define CGF_URL "http://www32.ocn.ne.jp/~yss/cgfgoban.html"

/* アバウトの表示 */
LRESULT CALLBACK AboutDlgProc( HWND hDlg, UINT msg, WPARAM wParam, Int64 /*lParam*/ )
{
	char sTmp[TMP_BUF_LEN];

	switch( msg ) {
		case WM_INITDIALOG:
			sprintf(sTmp,"%s %s",sVersion,sVerDate);
			SetDlgItemText(hDlg, IDM_CGF_VERSION, sTmp);
			SetDlgItemText(hDlg, IDM_HOMEPAGE, CGF_URL);
			return TRUE;	// TRUE を返せば SetFocus(hDlg); は必要ない？

		case WM_COMMAND:
			if ( wParam == IDM_HOMEPAGE ) ShellExecute(hDlg, NULL, CGF_URL, NULL, NULL, SW_SHOWNORMAL);	// これだけでIE5.0?が起動される
			if ( wParam == IDOK || wParam == IDCANCEL ) EndDialog( hDlg, 0);
			break;
	}
	return FALSE;
}

// Gnugo用の設定をするダイアログの処理
BOOL CALLBACK GnuGoSettingProc( HWND hDlg, UINT msg, WPARAM wParam, Int64 /*lParam*/)
{
	HWND hWnd;
	int i;

	switch(msg) {
		case WM_INITDIALOG:
			if ( nGtpPath == 0 ) {	// 未登録なら標準を追加。最後が最新。
				UpdateStrList("c:\\go\\aya\\aya.exe --mode gtp",                    &nGtpPath,GTP_PATH_MAX, sGtpPath );
				UpdateStrList("c:\\go\\MoGo\\mogo.exe --19 --time 12",              &nGtpPath,GTP_PATH_MAX, sGtpPath );
				UpdateStrList("c:\\go\\mogo\\mogo.exe --9 --nbTotalSimulations 10000", &nGtpPath,GTP_PATH_MAX, sGtpPath );
//				UpdateStrList("c:\\go\\CrazyStone\\CrazyStone-0005.exe",            &nGtpPath,GTP_PATH_MAX, sGtpPath );
//				UpdateStrList("c:\\go\\cgfgtp\\Release\\cgfgtp.exe",                &nGtpPath,GTP_PATH_MAX, sGtpPath );
				UpdateStrList("c:\\go\\gnugo\\gnugo.exe --mode gtp --never-resign", &nGtpPath,GTP_PATH_MAX, sGtpPath );
				UpdateStrList("c:\\go\\gnugo\\gnugo.exe --mode gtp",                &nGtpPath,GTP_PATH_MAX, sGtpPath );
			}
			hWnd = GetDlgItem(hDlg, IDM_GTP_PATH);
			for (i=0;i<nGtpPath;i++) SendMessage(hWnd, CB_INSERTSTRING, i, (Int64)sGtpPath[i]);
			SendMessage(hWnd, CB_SETCURSEL, 0,0);

//			SetDlgItemText(hDlg, IDM_GNUGO_DIRECTORY, sGnugoDir);
//			SetDlgItemText(hDlg, IDC_GNUGO_EX1, "c:\\go\\gnugo\\gnugo.exe --mode gtp");
//			SetDlgItemText(hDlg, IDC_GNUGO_EX2, "c:\\go\\gnugo\\gnugo.exe --mode gtp --never-resign --level 12");
//			SetDlgItemText(hDlg, IDC_GNUGO_EX3, "c:\\go\\aya\\aya.exe --mode gtp");
			CheckDlgButton(hDlg, IDM_GNUGO_LIFEDEATH, fGnugoLifeDeathView);
			return TRUE;

		case WM_COMMAND:
			if ( wParam == IDCANCEL ) EndDialog(hDlg,FALSE);		// キャンセル
			if ( wParam == IDOK ) {
				char sTmp[TMP_BUF_LEN];
				GetDlgItemText(hDlg, IDM_GTP_PATH, sTmp, TMP_BUF_LEN-1);
				if ( strlen(sTmp) > 0 ) {	// 文字列配列の候補リストを更新。
					UpdateStrList(sTmp, &nGtpPath,GTP_PATH_MAX, sGtpPath );
				}
//				GetDlgItemText(hDlg, IDM_GNUGO_DIRECTORY, sGnugoDir, MAX_PATH-1);
				fGnugoLifeDeathView = IsDlgButtonChecked( hDlg, IDM_GNUGO_LIFEDEATH );
				EndDialog(hDlg,FALSE);
			}
			return TRUE;
	}
	return FALSE;
}


// 旧名：StartProc
//
// 対局条件を設定するダイアログボックスを表示するイベント・ハンドラー。
bool CALLBACK Dlgt_PlayDialogStart( HWND hDlg, UINT msg, WPARAM wParam, Int64 lParam)
{
	int i,n;
	string sCom;
	const int nTypeMax = 5;
	string sType[nTypeMax] = { "Human","Computer(DLL)","LAN(nngs)","SGMP(RS232C)","Computer(GTP)", };
	const int handi_max = 9;
#ifdef CGF_E
	static string handi_str[handi_max] = { "None","2","3","4","5","6","7","8","9" };
	static string err_str[] = { "One player must be human or computer." };
	static string sKomi[3] = { "None","","" };
	static string sSize[3] = { "9","13","19" };
#else
	static string handi_str[handi_max] = { "なし","2子","3子","4子","5子","6子","7子","8子","9子" };
	static string err_str[] = { "両者とも通信対局は指定できません" };
	static string sKomi[3] = { "なし","半目","目半" };
	static string sSize[3] = { "9路","13路","19路" };
#endif
	static int fInitBoard = 0;	// 局面を初期化する場合

	switch(msg) {
		case WM_INITDIALOG:
			Wm_InitDialog.Perform();
			break;

		case WM_COMMAND:
			Wm_Command.Perform();
			return TRUE;
	}
	return FALSE;
}

// メッセージを表示して対局を停止
void GameStopMessage(HWND hWnd,int ret, int z)
{
	char sTmp[TMP_BUF_LEN];
	static string err_str[] = {
#ifdef CGF_E
		"OK",
		"Suicide move",
		"Ko!",
		"Stone is already here!",
		"Unknown error",
		"Both player passed. Game end.",
		"Too many moves.",
		"resign",
#else
		"OK",
		"自殺手",
		"コウ",
		"既に石があります",
		"その他のエラー",
		"連続パスで終局します",
		"手数が長すぎるため中断します",
		"投了です",
#endif
	};
	if ( ret >= MOVE_PASS_PASS ) {
		MessageBox(hWnd,err_str[ret],"Game Stop",MB_OK);
	} else {
		int x = z & 0xff;
		int y = z >> 8;
		sprintf(sTmp,"(%2d,%2d)\n\n%s",x,y,err_str[ret]);
		MessageBox(hWnd,sTmp,"move err",MB_OK);
	}
}


/*** 描画用子WINDOWの処理。このWindowに描画を行う。 ***/
LRESULT CALLBACK DrawWndProc( HWND hWnd, UINT msg, WPARAM wParam, Int64 lParam )
{
	int k,z,col;
	static string err_str[] = {
#ifdef CGF_E
		"You can not put or get a stone whlie viewing life and death.\n",
		"Stone is already here!\n",
		"Suicide move!",
		"Ko!",
		"You can not put or get ston whlie viewing life and death stone.\n",
#else
		"死活表示中は石は置けません。\n",
		"そこには石がありますよ。\n",
		"自殺手です！",
		"劫ですよ！",
		"死活表示中は石は取れません。\n",
#endif
	};

	if ( fPassWindows ) {	// 思考中はすべてカットする。
		return( DefWindowProc( hWnd, msg, wParam, lParam) );
	}

	switch( msg ) {
		case WM_PAINT:
			PaintBackScreen(hWnd);
			return 0;

		case WM_LBUTTONDOWN:
			if ( fNowPlaying == 0 ) {	// 盤面編集　
//				SetIshi( z, 1 );
				return 0; 
			}
			z = ChangeXY(lParam);
//			PRT("左＝%x  ",z);
			if ( z == 0 ) return 0;
			if ( endgame_board_type ) {
				PRT("%s",err_str[0]);
				return 0;
			}
			if ( ban[z] != 0 ) {
				PRT("%s",err_str[1]);
				return 0;
			}

			col = GetTurnCol();		// 手番と置石ありか？で色を決める

			k = make_move(z,col);
			if ( k==MOVE_SUICIDE ) {
				MessageBox(hWnd,err_str[2],"Move error",MB_OK);	// 自殺手
				return 0;
			}
			if ( k==MOVE_KOU ) {
				MessageBox(hWnd,err_str[3],"Move error",MB_OK);	// コウ
				return 0;
			}
			if ( k!=MOVE_SUCCESS ) {
				PRT("move_err\n");
				debug();
			}
			
			if ( IsGnugoBoard()    ) UpdateGnugoBoard(z);
			if ( IsCommunication() ) SendMove(z);

			tesuu++;	all_tesuu = tesuu;
			kifu[tesuu][0] = z;	// 棋譜を記憶
			kifu[tesuu][1] = col;	// 手番で色を決める
			kifu[tesuu][2] = (int)GetSpendTime(gct1);

			PaintUpdate();
			if ( SoundDrop ) {
//				MessageBeep(MB_OK);	// 音を鳴らしてみますか
				sndPlaySound("stone.wav", SND_ASYNC | SND_NOSTOP);	// MMSYSTEM.H をインクルードする必要あり。またWinMM.libをリンク
			}
			PostMessage(hWndMain, WM_COMMAND, IDM_NEXT_TURN, 0L);
			return 0;
/*
		case WM_RBUTTONDOWN:
			z = ChangeXY(lParam);
			if ( z == 0 ) return (0);
			if ( endgame_board_type ) {
				PRT("%s",err_str[4]);
				return 0;
			}
			if ( fNowPlaying == 0 ) {	// 盤面編集　
				SetIshi( z, 2 );
			}
			return 0;
*/
		default:
			return( DefWindowProc( hWnd, msg, wParam, lParam) );
	}
//	return 0L;
}

// 描画用Windowを同時に動かす
void MoveDrawWindow(HWND hWnd)
{
    RECT rc,rcClient;

    GetClientRect(hWnd, &rcClient);

    GetWindowRect(hWndToolBar,&rc);
    ScreenToClient(hWnd,(LPPOINT)&rc.right);
    rcClient.top = rc.bottom;

     MoveWindow(hWndDraw,
                   rcClient.left,
                   rcClient.top,
                   rcClient.right-rcClient.left,
                   rcClient.bottom-rcClient.top,
                   TRUE);
}

// Windowsへ一時的に制御を渡す。User からの指示による思考停止。
void PassWindowsSystem(void)
{
	MSG msg;

	fPassWindows = 1;	// Windowsに処理を渡してる。
	if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) ) {
		TranslateMessage( &msg );						// keyboard input.
		DispatchMessage( &msg );
	}
	fPassWindows = 0;
}	


/*** 棋譜をセーブする ***/
void KifuSave(void)
{
	OPENFILENAME ofn;
	char szDirName[256];
	char szFile[256], szFileTitle[256];
	UINT  cbString;
	char  chReplace;    /* szFilter文字列の区切り文字 */
	char  szFilter[256];
	char  Title[256];	// Title
	HANDLE hFile;		// ファイルのハンドル
	int i;
	unsigned long RetByte;		// 書き込まれたバイト数（ダミー）
	static int OnlyOne = 0;	// 連続対戦の回数（最初の1回だけ書き込まれる)。

	time_t timer;
	struct tm *tblock;
	unsigned int month,day,hour,minute;

	/* システム ディレクトリ名を取得してszDirNameに格納 */
	GetSystemDirectory(szDirName, sizeof(szDirName));
	szFile[0] = '\0';

	if ((cbString = LoadString(ghInstance, IDS_SAVE_FILTER,
	        szFilter, sizeof(szFilter))) == 0) {
	    return;
	}
	chReplace = szFilter[cbString - 1]; /* ワイルド カードを取得 */

	for (i = 0; szFilter[i] != '\0'; i++) {
	    if (szFilter[i] == chReplace)
	       szFilter[i] = '\0';
	}


	/* 時刻の取得 */
	timer = time(NULL);
	tblock = localtime(&timer);   /* 日付／時刻を構造体に変換する */
//	PRT("Local time is: %s", asctime(tblock));
//	second = tblock->tm_sec;      /* 秒 */
	minute = tblock->tm_min;      /* 分 */
	hour   = tblock->tm_hour;     /* 時 (0～23) */
	day    = tblock->tm_mday;     /* 月内の通し日数 (1～31) */
	month  = tblock->tm_mon+1;    /* 月 (0～11) */
//	year   = tblock->tm_year + 1900;     /* 年 (西暦 - 1900) */
//	week   = tblock->tm_wday;     /* 曜日 (0～6; 日曜 = 0) */
//	PRT( "Date %02x-%02d-%02x / Time %02x:%02x:%02x\n", year, month, day, hour, minute, second );

//	sprintf( szFileTitle, "%02d%02d%02d%02d", month, day, hour, minute );	// ファイル名を日付＋秒に
//	if ( fAutoSave ) sprintf( szFile, "fig%03d.sgf",++OnlyOne );	// 自動セーブの時。
	if ( fAutoSave ) sprintf( szFile, "%x%02d@%02d%02d.sgf", month, day, hour,++OnlyOne );	// 自動セーブの時。
	else             sprintf( szFile, "%x%02d-%02d%02d",     month, day, hour, minute );	// ファイル名を日付＋秒に


	/* 構造体のすべてのメンバを0に設定 */
	memset(&ofn, 0, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = ghWindow;
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile= szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);
//	ofn.lpstrInitialDir = szDirName;
	ofn.lpstrInitialDir = NULL;		// カレントディレクトリに
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt	= "sgf";	// デフォルトの拡張子を ".sgf" に
	
	if ( fAutoSave || GetSaveFileName(&ofn) ) {
		hFile = CreateFile(ofn.lpstrFile,      /* create MYFILE.TXT  */
	             GENERIC_WRITE,                /* open for writing   */
	             0,                            /* do not share       */
		         (LPSECURITY_ATTRIBUTES) NULL, /* no security        */
			     CREATE_ALWAYS,                /* overwrite existing */
				 FILE_ATTRIBUTE_NORMAL,        /* normal file        */
	             (HANDLE) NULL);               /* no attr. template  */
		if (hFile == INVALID_HANDLE_VALUE) {
			PRT("Could not open file.");       /* process error      */
			return;
		}

		// タイトルにファイル名を付ける
		sprintf(Title,"%s - %s",ofn.lpstrFile, szTitleName);
		SetWindowText(ghWindow, Title);

		// ファイルリストを更新。一番最近開いたものを先頭へ。
		UpdateRecentFile(ofn.lpstrFile);
		UpdateRecentFileMenu(ghWindow);

		if ( stricmp(ofn.lpstrFile + ofn.nFileExtension, "sgf" ) == 0 || fAutoSave ) {
			SaveSGF();	// SGFで書き出す
		} else {
			SaveIG1();	// IG1 形式で書き込む。
		}
		WriteFile( hFile,(void *)SgfBuf, strlen(SgfBuf), &RetByte, NULL );
		CloseHandle(hFile);		// ファイルをクローズ
	}
}


/*** 棋譜をロードする ***/
int KifuOpen(void)
{
	OPENFILENAME ofn;
	char szDirName[256];
	char szFile[256], szFileTitle[256];
	UINT  cbString;
	char  chReplace;    /* szFilter文字列の区切り文字 */
	char  szFilter[256];
	char  Title[256];	// Title

	HANDLE hFile;		// ファイルのハンドル
	int i;

	szFile[0] = '\0';
	if ((cbString = LoadString(ghInstance, IDS_LOAD_FILTER, szFilter, sizeof(szFilter))) == 0) return FALSE;
	chReplace = szFilter[cbString - 1]; /* ワイルド カードを取得 */
	for (i = 0; szFilter[i] != '\0'; i++) if (szFilter[i] == chReplace) szFilter[i] = '\0';

	/* 構造体のすべてのメンバを0に設定 */
	memset(&ofn, 0, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = ghWindow;
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile= szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);
	GetCurrentDirectory(sizeof(szDirName), szDirName);	// カレントディレクトリ名を取得してszDirNameに格納
	ofn.lpstrInitialDir = szDirName;	// カレントディレクトリに
//	ofn.lpstrInitialDir = NULL;			// ---> これだとWin98以降で*.amzファイルがないとMyDocumentsが標準にされる
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if ( strlen(cDragFile) || GetOpenFileName(&ofn) ) {
		if ( strlen(cDragFile) ) {	// ダブルクリックで起動された時
			string p;

			if ( cDragFile[0] == '"' ) {	// 前後の "" を消す
				strcpy(szFile,cDragFile+1);
				strcpy(cDragFile,szFile);
				cDragFile[ strlen(cDragFile)-1 ] = 0;
			}

			strcpy(szFile,cDragFile);	// szFileにコピー
			ofn.lpstrFile = szFile;
			p = strrchr( ofn.lpstrFile, '.' );	// 最後のピリオドを探す
			if ( p == NULL ) ofn.nFileExtension = 0;	// どのみちエラー
			else             ofn.nFileExtension = (p - ofn.lpstrFile)+1;
			PRT("%s,%d,%s\n",ofn.lpstrFile,ofn.nFileExtension,ofn.lpstrFile + ofn.nFileExtension);
			strcpy(ofn.lpstrFileTitle,ofn.lpstrFile);
			cDragFile[0] = '\0';	// ファイル名を消す	
		}
		hFile = CreateFile(ofn.lpstrFile,      /* create MYFILE.TXT  */
	             GENERIC_READ,                 /* open for reading   */
	             0,                            /* do not share       */
		         (LPSECURITY_ATTRIBUTES) NULL, /* no security        */
			     OPEN_EXISTING,                /* overwrite existing */
				 FILE_ATTRIBUTE_NORMAL,        /* normal file        */
	             (HANDLE) NULL);               /* no attr. template  */
		if (hFile == INVALID_HANDLE_VALUE) {
			PRT("Could not open file =%s\n",ofn.lpstrFile);       /* process error      */
			return FALSE;
		}
 
		// タイトルにファイル名を付ける
		sprintf(Title,"%s - %s",ofn.lpstrFileTitle, szTitleName);
		SetWindowText(ghWindow, Title);

		// ファイルリストを更新。一番最近開いたものを先頭へ。
		UpdateRecentFile(ofn.lpstrFile);
		UpdateRecentFileMenu(ghWindow);


		PRT("\n%s\n",ofn.lpstrFile);

		// 全てのファイルをバッファに読み込む
		ReadFile( hFile, (void *)SgfBuf, SGF_BUF_MAX, &nSgfBufSize, NULL );
		CloseHandle(hFile);		// ファイルをクローズ

		int fSgf = 0;
		// SGF (Standard Go Format) を読み込む。
		if ( stricmp(ofn.lpstrFile + ofn.nFileExtension, "sgf" ) == 0 ) fSgf = 1;
		// IG1 (Nifty-Serve GOTERM) を読み込む。
		if ( stricmp(ofn.lpstrFile + ofn.nFileExtension, "ig1" ) == 0 ) fSgf = 0;

		LoadSetSGF(fSgf);
	}

//	PRT("読み込んだ手数=%d,全手数=%d\n",tesuu,all_tesuu);
	return TRUE;
}








/*** 囲碁盤の初期座標値を取得（囲碁盤自体の縮小に備える）***/
void GetScreenParam( HWND hWnd,int *dx, int *sx,int *sy, int *db )
{
	RECT rc;
	
	GetClientRect( hWnd, &rc );

	*sx = 22;			// 囲碁盤の左端ｘ座標	 
	*sy = 20;

	*dx = 2*(rc.bottom - *sy -15 ) / ( 2*ban_size+1);	// -15 は気分
		
	if ( *dx <= 10 ) *dx = 10;	// 最低は10
	if ( *dx & 1 ) *dx -= 1;	// 偶数に
//	if ( !(*dx & 2) ) *dx -= 2;	// 18 + 4*x の倍数に

	*db = (*dx*3) / 4;	// 囲碁盤の端と線の間隔
	if ( !(*db & 1) ) *db += 1;	// 奇数に
//	if ( iBanSize==BAN_19 ) *dx = 26;
//	if ( iBanSize==BAN_13 ) *dx = 38;
//	if ( iBanSize==BAN_9  ) *dx = 50;
//	*dx = 26;		// 升目の間隔 (640*480..20, 800*600..25, 1280*780..34)
					// 18 <= dx <= 36 (24 +- 4 の倍数は暗い盤で枠がにじむ)
}

/*** WM_PAINT で画面を再描画する関数 ***/

string info_str[] = {
#ifdef CGF_E
	"B", 
	"W",
	"Hama: %d",
	"N= %3d",
	"Black Turn",
	"White Turn",
	"Draw",
	"%s %.1f Win",
	"(Chinese)",
	"Black", 
	"White",
#else
	"黒", 
	"白",
	"揚浜：%d",
	"第%3d手",
	"黒の番です",
	"白の番です",
	"持碁",
	"%sの %.1f 目勝",
	"(中国)",
	"黒", 
	"白",
#endif
};

#ifdef CGF_E
char FontName[] = "FixedSys";	// "MS UI Gothic" "MS Sans Serif" "Courier New" "BatangChe"(win98では見にくい)
#else
char FontName[] = "標準明朝";
#endif

// 盤上に数値を書き込む
void DrawValueNumber(HDC hDC,int size, int col, int xx, int yy, string sNum)
{
	TEXTMETRIC tm;
	HFONT hFont,hFontOld;
	int n = strlen(sNum);

	hFont = CreateFont(					/* フォントの作成 */
		size,0,0,0,FW_NORMAL,
		0,0,0,	DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
           DEFAULT_PITCH, FontName);
	hFontOld = (HFONT)SelectObject(hDC,hFont);
	GetTextMetrics(hDC, &tm);
	if ( col==2 ) SetTextColor( hDC, rgbBlack );	// 黒で
	else          SetTextColor( hDC, rgbWhite );	// 白で
	TextOut( hDC, xx-tm.tmAveCharWidth*n/2 - (n==3)*1, yy-size/2, sNum ,n);
	SelectObject( hDC, hFontOld );
	DeleteObject(hFont);
}


// 終局処理をした後で、地の数のみを数える。(黒地 － 白地)
int count_endgame_score(void)
{
	int x,y,z,col,n,sum;
	
	sum = hama[0] - hama[1];
	for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
		z = ((y+1)<<8) + (x+1);
		col = ban[z];
		n   = endgame_board[z];
		if ( col == 0 ) {	// 空点
			if ( n == GTP_BLACK_TERRITORY ) sum++;	// 黒地
			if ( n == GTP_WHITE_TERRITORY ) sum--;
		} else {
			if ( n == GTP_DEAD ) {
				if ( col == 1 ) sum -= 2;
				if ( col == 2 ) sum += 2;
			}
		}
	}
	return sum;
}

// 中国ルールで計算（アゲハマは無視し、死石除去後の自分の地と自分の石、の合計を数える。ダメに打つ場合も1目となる）
int count_endgame_score_chinese(void)
{
	int x,y,z,col,n,sum;

	sum = 0;
	for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
		z = ((y+1)<<8) + (x+1);
		col = ban[z];
		n   = endgame_board[z];
		if ( col == 0 ) {	// 空点
			if ( n == GTP_BLACK_TERRITORY ) sum++;	// 黒地
			if ( n == GTP_WHITE_TERRITORY ) sum--;
		} else {
			if ( n == GTP_DEAD ) {
				if ( col == 1 ) sum--;
				if ( col == 2 ) sum++;
			} else {
				if ( col == 1 ) sum++;
				if ( col == 2 ) sum--;
			}
		}
	}
	return sum;
}

// 地を数えて結果を文字列で返す
void endgame_score_str(string str, int flag)
{
	double score;
	
	if ( flag == 0 ) score = count_endgame_score();
	else             score = count_endgame_score_chinese();
	LatestScore = (int)score;
	score -= komi;
	if ( score == 0.0 ) sprintf(str,info_str[6]);
	else {
		char sTmp[TMP_BUF_LEN];
		if ( score > 0 ) {
			sprintf(sTmp,info_str[9]);
		} else {
			sprintf(sTmp,info_str[10]);
			score = -score;
		}
		sprintf(str,info_str[7],sTmp,score);
		if ( flag ) strcat(str,info_str[8]);
//		PRT("score=%.3f\n",score);
	}
}

// 裏画面に書いて一気に転送する。BeginPaintはこっちで行う。
void PaintBackScreen( HWND hWnd )
{
	PAINTSTRUCT ps;
	HDC hDC;
	HDC memDC;
	HBITMAP bmp,old_bmp;
	int maxX,maxY;
	RECT rc;

	hDC = BeginPaint(hWnd, &ps);

	// 仮想最大画面サイズを取得(Desktopのサイズ）SM_CXSCREEN だとマルチディスプレイに未対応。
	GetClientRect(hWnd,&rc);
	maxX = rc.right;
	maxY = rc.bottom;
//	maxX = GetSystemMetrics(SM_CXVIRTUALSCREEN);	// コンパイルが通らない。VC6ではだめ。VC7(.net)ならOK。
//	maxY = GetSystemMetrics(SM_CYVIRTUALSCREEN);
//	PRT("maxX,Y=(%d,%d)\n",maxX,maxY);

	// 仮想画面のビットマップを作成
	memDC   = CreateCompatibleDC(hDC);
	bmp     = CreateCompatibleBitmap(hDC,maxX,maxY);
	old_bmp = (HBITMAP)SelectObject(memDC, bmp);

	PaintScreen( hWnd, memDC );		// 実際に描画を行う関数

	BitBlt(hDC,0,0,maxX,maxY,memDC,0,0, SRCCOPY);	// 裏画面を一気に描画

	// 後始末
	SelectObject(memDC,old_bmp);	// 関連付けられたオブジェクトを元に戻す
	DeleteObject(bmp);				// HDCに関連付けられたGDIオブジェクトは削除できないから
	DeleteDC(memDC);

	EndPaint(hWnd, &ps);
}


/*** マウスの左クリックの時の位置をxy座標に変換して返す ***/
/*** どこも指してないときは０を返す                     ***/
int ChangeXY(LONG lParam)
{
	int dx,sx,sy,db;
	int x,y;
	
	GetScreenParam( hWndDraw,&dx, &sx, &sy, &db );
	x = LOWORD(lParam);
	y = HIWORD(lParam);
	x -= sx + db - dx/2;	// 盤の左隅（のさらに石１つ分の左隅）に平行移動
	y -= sy + db - dx/2;
	if ( x < 0	|| x > dx*ban_size ) return 0;
	if ( y < 0  || y > dx*ban_size ) return 0;
	x = x / dx;
	y = y / dx;
	return ( ((y+1)<<8) + x+1);
}

// 任意の手数の局面に盤面を移動させる
// 前に進む場合は簡単、戻る場合は初手からやり直す。
void move_tesuu(int n)
{
	int i,z,col,loop;

//	PRT("現在の手数=%d, 全手数=%d, 希望の手数=%d ...in \n",tesuu,all_tesuu,n);

	if ( n == tesuu ) return;
	if ( endgame_board_type ) {
		endgame_board_type = 0;
		UpdateLifeDethButton(0);
		PaintUpdate();
	}

	if ( n < tesuu ) {	// 手を戻す。
		if ( n <= 0 ) n = 0;
		loop = n;
		// 盤面を初期盤面に
		for (i=0;i<BANMAX;i++) ban[i] = ban_start[i];
		reconstruct_ban();
	} else {			// 手を進める
		loop = n - tesuu;
		if ( loop + tesuu >= all_tesuu ) loop = all_tesuu - tesuu;	// 最終局面へ。
	}

//	PRT("loop=%d,手数=%d\n",loop,tesuu);
	for (i=0;i<loop;i++) {
		tesuu++;
		z   = kifu[tesuu][0];	// 位置
		col = kifu[tesuu][1];	// 石の色
//		PRT("手を進める z=%x, col=%d\n",z,col);
		if ( make_move(z,col) != MOVE_SUCCESS )	{
			PRT("棋譜再現中にエラー! tesuu=%d, z=%x,col=%d\n",tesuu,z,col);
			break;
		}
	}
//	PRT("現在の手数=%d, 全手数=%d ..end\n",tesuu, all_tesuu);
}

// 現在の手番の色を返す（現在手数と初手が黒番かどうかで判断する）
int GetTurnCol(void)
{
	return 1 + ((tesuu+(nHandicap!=0)) & 1);	// 石の色
}

// 黒の手番か？
int IsBlackTurn(void)
{
	if ( GetTurnCol() == 1 ) return 1;
	return 0;
}

// Gnugoと対局中か？
int IsGnugoBoard(void)
{
	int i;
	for (i=0;i<2;i++) {
		if ( turn_player[i] == PLAYER_GNUGO ) return 1;
	}
	return 0;
}


// 画面の全書き換え
void PaintUpdate(void)
{
	InvalidateRect(hWndDraw, NULL, FALSE);
	UpdateWindow(hWndDraw);
	return;
}

// 盤面色のメニューにチェックを
void MenuColorCheck(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	int i;
	
	for (i=0;i<RGB_BOARD_MAX;i++) {
		CheckMenuItem(hMenu, ID_BAN_1 + i, MF_UNCHECKED);
	}
	CheckMenuItem(hMenu, ID_NowBanColor, MF_CHECKED);
}


int ReplayMenu[] = {
	IDM_BACK1,
	IDM_BACK10,
	IDM_BACKFIRST,
	IDM_FORTH1,
	IDM_FORTH10,
	IDM_FORTHEND,

	IDM_PLAY_START,
	IDM_KIFU_OPEN,
	IDM_PASS,
	IDM_BREAK,
	-1
};

// メニューとツールバーのボタンを淡色表示に
void MenuEnableID(int id, int on)
{
	HMENU hMenu = GetMenu(ghWindow);

	EnableMenuItem(hMenu, id, MF_BYCOMMAND | (on ? MF_ENABLED : MF_GRAYED) );
	SendMessage(hWndToolBar, TB_SETSTATE, id, MAKELPARAM(on ? TBSTATE_ENABLED : TBSTATE_INDETERMINATE, 0));
}

// 対局中は棋譜移動メニューを禁止する
void MenuEnablePlayMode(int fLifeDeath)
{
	int i,id,on;

	for (i=0; ;i++) {
		id = ReplayMenu[i];
		if ( id < 0 ) break;
		on = !fNowPlaying;
		if ( id==IDM_PASS || id==IDM_BREAK ) on = !on;
		if ( fLifeDeath ) on = 0;
		MenuEnableID(id, on);
	}
}

// 死活表示中は全てのメニューを禁止する
void UpdateLifeDethButton(int check_on)
{
	MenuEnablePlayMode(check_on);
	UINT iFlags = check_on ? (TBSTATE_PRESSED | TBSTATE_ENABLED) : TBSTATE_ENABLED;
	SendMessage(hWndToolBar, TB_SETSTATE, IDM_DEADSTONE, MAKELPARAM(iFlags, 0));
}


// ToolBarの設定
#define NUMIMAGES       17	// ボタンの数
#define IMAGEWIDTH      16
#define IMAGEHEIGHT     15
#define BUTTONWIDTH      0
#define BUTTONHEIGHT     0

#define IDTBB_PASS       0
#define IDTBB_BACKFIRST  1
#define IDTBB_BACK10     2
#define IDTBB_BACK1      3
#define IDTBB_FORTH1     4
#define IDTBB_FORTH10    5
#define IDTBB_FORTHEND   6
#define IDTBB_DEADSTONE  7
#define IDTBB_DLL_BREAK  8
#define IDTBB_SET_BOARD  9
#define IDTBB_UNDO      10
#define IDTBB_BAN_EDIT  11
#define IDTBB_NEWGAME   12
#define IDTBB_OPEN      13
#define IDTBB_SAVE      14
#define IDTBB_BREAK     15
#define IDTBB_RESTART   16

TBBUTTON tbButton[] =           // Array defining the toolbar buttons
{
    {0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
#ifndef CGF_DEV
    {IDTBB_RESTART,  IDM_PLAY_START,TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
    {IDTBB_OPEN,     IDM_KIFU_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {IDTBB_SAVE,     IDM_KIFU_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
    {0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
#endif
    {IDTBB_PASS,     IDM_PASS,      TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
//  {IDTBB_UNDO,     IDM_UNDO,      TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
//  {0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
    {0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},

    {IDTBB_BACKFIRST,IDM_BACKFIRST, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},

    {IDTBB_BACK10,   IDM_BACK10,    TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {IDTBB_BACK1,    IDM_BACK1,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {IDTBB_FORTH1,   IDM_FORTH1,    TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {IDTBB_FORTH10,  IDM_FORTH10,   TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {IDTBB_FORTHEND, IDM_FORTHEND,  TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},

    {0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
    {0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
    {IDTBB_DEADSTONE,IDM_DEADSTONE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},

    {0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
    {0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
#ifdef CGF_DEV
    {IDTBB_SET_BOARD,IDM_SET_BOARD, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
    {0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
#endif
    {IDTBB_BREAK,    IDM_BREAK,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
	{0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
	{0, 0,                          TBSTATE_ENABLED, TBSTYLE_SEP,    0, 0},
	{IDTBB_DLL_BREAK,IDM_DLL_BREAK, TBSTATE_INDETERMINATE,  TBSTYLE_BUTTON, 0, 0},

};


BOOL CreateTBar(HWND hWnd)
{

	InitCommonControls();	// コモンコントロールのdllを初期化
	hWndToolBar = CreateToolbarEx(hWnd,
                                  WS_CHILD | WS_VISIBLE | TBSTYLE_TOOLTIPS,
                                  IDM_TOOLBAR,
                                  NUMIMAGES,
                                  ghInstance,
                                  IDB_TOOLBAR,
                                  tbButton,
                                  sizeof(tbButton)/sizeof(TBBUTTON),
                                  BUTTONWIDTH,
                                  BUTTONHEIGHT,
                                  IMAGEWIDTH,
                                  IMAGEHEIGHT,
                                  sizeof(TBBUTTON));

    return (hWndToolBar != 0);
}


LRESULT SetTooltipText(HWND /*hWnd*/, Int64 lParam)
{
    LPTOOLTIPTEXT lpToolTipText;
    static char   szBuffer[64];

    lpToolTipText = (LPTOOLTIPTEXT)lParam;
    if (lpToolTipText->hdr.code == TTN_NEEDTEXT)
    {
        LoadString(ghInstance,
                   lpToolTipText->hdr.idFrom,   // string ID == command ID
                   szBuffer,
                   sizeof(szBuffer));

        lpToolTipText->lpszText = szBuffer;
    }
    return 0;
}

// ツールバーボタンの状態を変更
void UpdateButton(UINT iID, UINT iFlags)
{
    int iCurrentFlags;

    iCurrentFlags = (int)SendMessage(hWndToolBar, 
                                     TB_GETSTATE, 
                                     iID, 0L);

    if (iCurrentFlags & TBSTATE_PRESSED)
        iFlags |= TBSTATE_PRESSED;

    SendMessage(hWndToolBar, 
                TB_SETSTATE, 
                iID, 
                MAKELPARAM(iFlags, 0));
    return;
}


// WinMain()のメッセージループで条件判定に使う
int IsDlgDoneMsg(MSG msg)
{
	if ( hAccel != NULL && TranslateAccelerator(hWndMain, hAccel, &msg)	!= 0 ) return 0;

//	if ( IsDlgMsg(hDlgPntFgr,      msg) == 0 ) return 0;
	return 1;
}

// レジストリの処理
static TCHAR CgfNamesKey[]     = TEXT("Software\\Yss\\cgfgoban");
static TCHAR WindowKeyName[]   = TEXT("WindowPos") ;
static TCHAR PrtWinKeyName[]   = TEXT("PrtWinPos") ;
static TCHAR SoundDropName[]   = TEXT("SoundDrop") ;
static TCHAR BoardColorName[]  = TEXT("BoardColor") ;
static TCHAR InfoWindowName[]  = TEXT("InfoWindow") ;
static TCHAR FileListName[]    = TEXT("Recent File List") ;
static TCHAR BoardSizeName[]   = TEXT("BoardSize") ;
static TCHAR GnugoDirName[]    = TEXT("GnugoDir") ;
static TCHAR GnugoLifeName[]   = TEXT("GnugoLifeDeath") ;
static TCHAR TurnPlayer0Name[] = TEXT("TurnPlayerBlack") ;
static TCHAR TurnPlayer1Name[] = TEXT("TurnPlayerWhite") ;
static TCHAR NameListName[]    = TEXT("Recent Name List") ;
static TCHAR NameListNumName[] = TEXT("NameList") ;
static TCHAR GtpPathName[]     = TEXT("Recent Gtp List") ;
static TCHAR GtpPathNumName[]  = TEXT("GtpList") ;
static TCHAR NngsIPName[]      = TEXT("Recent Nngs List") ;
static TCHAR NngsIPNumName[]   = TEXT("NngsIP") ;
static TCHAR NngsTimeMinutes[] = TEXT("NngsMinutes") ;
static TCHAR ATViewName[]      = TEXT("ATView") ;

// レジストリから情報（画面サイズと位置）を読み込む	 flag == 1 ... main window, 2... print window
BOOL LoadMainWindowPlacement (HWND hWnd,int flag)	
{
	WINDOWPLACEMENT   WindowPlacement; 
	TCHAR             szWindowPlacement [TMP_BUF_LEN] ;
	HKEY              hKeyNames;
	DWORD             Size,Type,Status;

	if ( UseDefaultScreenSize ) return TRUE;	// 固定の画面サイズを使う

	Status = RegOpenKeyEx(HKEY_CURRENT_USER, CgfNamesKey, 0L, KEY_READ, &hKeyNames) ;
	if (Status == ERROR_SUCCESS) {
		// get the window placement data
		Size = sizeof(szWindowPlacement);

		// Main Window の位置を復元
		if ( flag==1 ) Status = RegQueryValueEx(hKeyNames, WindowKeyName, NULL, &Type, (LPBYTE)szWindowPlacement, &Size) ;
		else           Status = RegQueryValueEx(hKeyNames, PrtWinKeyName, NULL, &Type, (LPBYTE)szWindowPlacement, &Size) ;
		RegCloseKey (hKeyNames);

		if (Status == ERROR_SUCCESS) {
			sscanf (szWindowPlacement, "%d %d %d %d %d %d %d %d %d", &WindowPlacement.showCmd,
				&WindowPlacement.ptMinPosition.x, &WindowPlacement.ptMinPosition.y,
				&WindowPlacement.ptMaxPosition.x, &WindowPlacement.ptMaxPosition.y,
				&WindowPlacement.rcNormalPosition.left,  &WindowPlacement.rcNormalPosition.top,
				&WindowPlacement.rcNormalPosition.right, &WindowPlacement.rcNormalPosition.bottom );
   
   			WindowPlacement.showCmd = SW_SHOW;
			WindowPlacement.length  = sizeof(WINDOWPLACEMENT);
			WindowPlacement.flags   = WPF_SETMINPOSITION;

			if ( !SetWindowPlacement(hWnd, &WindowPlacement) ) return (FALSE);

			return (TRUE) ;
		} else return (FALSE);
	}

	// open registry failed, use input from startup info or Max as default
	PRT("レジストリのオープンに失敗！\n");
	ShowWindow (hWnd, SW_SHOW) ;
	return (FALSE) ;
}

// レジストリに情報（画面サイズと位置）を書き込む flag == 1 ... main window, 2... print window
BOOL SaveMainWindowPlacement(HWND hWnd,int flag)
{
	WINDOWPLACEMENT   WindowPlacement;
	TCHAR             ObjectType [2] ;
    TCHAR             szWindowPlacement [TMP_BUF_LEN] ;
    HKEY              hKeyNames = 0 ;
    DWORD             Size ;
    DWORD             Status ;
    DWORD             dwDisposition ;
 
	if ( hWnd == NULL ) return FALSE;	// 情報表示画面を閉じている
	ObjectType [0] = TEXT(' ') ;
	ObjectType [1] = TEXT('\0') ;

    WindowPlacement.length = sizeof(WINDOWPLACEMENT);
    if ( !GetWindowPlacement(hWnd, &WindowPlacement) ) return FALSE;
		sprintf(szWindowPlacement, "%d %d %d %d %d %d %d %d %d",
			WindowPlacement.showCmd, 
            WindowPlacement.ptMinPosition.x, WindowPlacement.ptMinPosition.y,
            WindowPlacement.ptMaxPosition.x, WindowPlacement.ptMaxPosition.y,
            WindowPlacement.rcNormalPosition.left,  WindowPlacement.rcNormalPosition.top,
            WindowPlacement.rcNormalPosition.right, WindowPlacement.rcNormalPosition.bottom) ;

	// try to create it first
	Status = RegCreateKeyEx(HKEY_CURRENT_USER, CgfNamesKey, 0L,
		ObjectType, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WRITE, NULL, &hKeyNames, &dwDisposition) ;

	// if it has been created before, then open it
	if (dwDisposition == REG_OPENED_EXISTING_KEY) {
		Status = RegOpenKeyEx(HKEY_CURRENT_USER, CgfNamesKey, 0L, KEY_WRITE, &hKeyNames);
    }

	// we got the handle, now store the window placement data
	if (Status == ERROR_SUCCESS) {
		Size = (lstrlen (szWindowPlacement) + 1) * sizeof (TCHAR) ;
		if ( flag==1 ) Status = RegSetValueEx(hKeyNames, WindowKeyName, 0, REG_SZ, (LPBYTE)szWindowPlacement, Size) ;
		else           Status = RegSetValueEx(hKeyNames, PrtWinKeyName, 0, REG_SZ, (LPBYTE)szWindowPlacement, Size) ;

		RegCloseKey (hKeyNames);
	}

	return (Status == ERROR_SUCCESS) ;
}

DWORD SaveRegList(HKEY hKeyNames, TCHAR *SubNamesKey,int nList, char sList[][TMP_BUF_LEN])
{
	TCHAR             ObjectType [2] ;
    DWORD             Size ;
    DWORD             Status ;
    DWORD             dwDisposition ;

	// try to create it first
	Status = RegCreateKeyEx(HKEY_CURRENT_USER, SubNamesKey, 0L,
		ObjectType, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WRITE, NULL, &hKeyNames, &dwDisposition) ;
	// if it has been created before, then open it
	if (dwDisposition == REG_OPENED_EXISTING_KEY) {
		Status = RegOpenKeyEx(HKEY_CURRENT_USER, SubNamesKey, 0L, KEY_WRITE, &hKeyNames);
    }
	if (Status == ERROR_SUCCESS) {
		int i;
		char sTmp[TMP_BUF_LEN];

		Size = sizeof(sList[0]);
//		PRT("size=%d\n",Size);
		for (i=0;i<nList;i++) {
			sprintf(sTmp,"File%d",i+1);
//			PRT("%s\n",sTmp);
			Status |= RegSetValueEx(hKeyNames, sTmp, 0, REG_SZ, (LPBYTE)sList[i], Size) ;
		}
		RegCloseKey (hKeyNames);
	}
	return Status;
}

// レジストリに情報（効果音、画面の色）を書き込む
BOOL SaveRegCgfInfo(void)
{
	TCHAR             ObjectType [2] ;
    HKEY              hKeyNames = 0 ;
    DWORD             Status ;
    DWORD             dwDisposition ;
	TCHAR   SubNamesKey[TMP_BUF_LEN] ;
 
	ObjectType [0] = TEXT(' ') ;
	ObjectType [1] = TEXT('\0') ;

	// try to create it first
	Status = RegCreateKeyEx(HKEY_CURRENT_USER, CgfNamesKey, 0L,
		ObjectType, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WRITE, NULL, &hKeyNames, &dwDisposition) ;

	// if it has been created before, then open it
	if (dwDisposition == REG_OPENED_EXISTING_KEY) {
		Status = RegOpenKeyEx(HKEY_CURRENT_USER, CgfNamesKey, 0L, KEY_WRITE, &hKeyNames);
    }

//	fInfoWindow = (hPrint != NULL);
	// we got the handle, now store the window placement data
	if (Status == ERROR_SUCCESS) {
		Status  = 0;
		Status |= RegSetValueEx(hKeyNames, SoundDropName,  0, REG_DWORD, (LPBYTE)&SoundDrop,      sizeof(DWORD) ) ;
		Status |= RegSetValueEx(hKeyNames, BoardColorName, 0, REG_DWORD, (LPBYTE)&ID_NowBanColor, sizeof(DWORD) ) ;
		Status |= RegSetValueEx(hKeyNames, InfoWindowName, 0, REG_DWORD, (LPBYTE)&fInfoWindow,    sizeof(DWORD) ) ;
		Status |= RegSetValueEx(hKeyNames, BoardSizeName,  0, REG_DWORD, (LPBYTE)&ban_size,       sizeof(DWORD) ) ;
		Status |= RegSetValueEx(hKeyNames, GnugoLifeName,  0, REG_DWORD, (LPBYTE)&fGnugoLifeDeathView, sizeof(DWORD) ) ;
		Status |= RegSetValueEx(hKeyNames, TurnPlayer0Name,0, REG_DWORD, (LPBYTE)&turn_player[0], sizeof(DWORD) ) ;
		Status |= RegSetValueEx(hKeyNames, TurnPlayer1Name,0, REG_DWORD, (LPBYTE)&turn_player[1], sizeof(DWORD) ) ;
		Status |= RegSetValueEx(hKeyNames, NameListNumName,0, REG_DWORD, (LPBYTE)&nNameList     , sizeof(DWORD) ) ;
		Status |= RegSetValueEx(hKeyNames, GtpPathNumName, 0, REG_DWORD, (LPBYTE)&nGtpPath      , sizeof(DWORD) ) ;
		Status |= RegSetValueEx(hKeyNames, NngsIPNumName,  0, REG_DWORD, (LPBYTE)&nNngsIP       , sizeof(DWORD) ) ;
		Status |= RegSetValueEx(hKeyNames, NngsTimeMinutes,0, REG_DWORD, (LPBYTE)&nngs_minutes  , sizeof(DWORD) ) ;
		Status |= RegSetValueEx(hKeyNames, ATViewName,     0, REG_DWORD, (LPBYTE)&fATView       , sizeof(DWORD) ) ;

		if ( Status != ERROR_SUCCESS ) PRT("RegSetValueEx Err\n");
		RegCloseKey (hKeyNames);
	}

	sprintf(SubNamesKey,"%s\\%s",CgfNamesKey,FileListName);	// Yssの下にFile用の格納場所を作る
	SaveRegList(hKeyNames, SubNamesKey, nRecentFile, sRecentFile);
	sprintf(SubNamesKey,"%s\\%s",CgfNamesKey,NameListName);
	SaveRegList(hKeyNames, SubNamesKey, nNameList, sNameList);
	sprintf(SubNamesKey,"%s\\%s",CgfNamesKey,GtpPathName);
	SaveRegList(hKeyNames, SubNamesKey, nGtpPath, sGtpPath);
	sprintf(SubNamesKey,"%s\\%s",CgfNamesKey,NngsIPName);
	SaveRegList(hKeyNames, SubNamesKey, nNngsIP, sNngsIP);

	return (Status == ERROR_SUCCESS) ;
}

void ReadRegistoryInt(HKEY hKey, TCHAR *sName, int *pValue, int default_value)
{
	DWORD size = sizeof(int);
	DWORD Type;
	DWORD status = RegQueryValueEx(hKey, sName, NULL, &Type, (LPBYTE)pValue, &size) ;
	if ( status == ERROR_SUCCESS) return;
	PRT("load reg err. set default %s=%d\n",sName,default_value);
	*pValue = default_value;
}

void ReadRegistoryStr(HKEY hKey, TCHAR *sName, string str, DWORD size, string default_str)
{
//	DWORD size = sizeof(str);	// これだと現時点の文字列の長さを返してしまう。NULLだと0。ERROR_MORE_DATA になる。
	DWORD Type;
	DWORD status = RegQueryValueEx(hKey, sName, NULL, &Type, (LPBYTE)str, &size) ;
	if ( status == ERROR_SUCCESS) return;
	PRT("load reg err=%d size=%d,%s=%s, %s\n",status,size,sName,str,default_str);
	strcpy(str,default_str);
}

DWORD LoadRegList(TCHAR *SubNamesKey,int *pList, int nListMax, char sList[][TMP_BUF_LEN])
{
	HKEY hKeyNames;
	DWORD Size,Type,Status;
	Status = RegOpenKeyEx(HKEY_CURRENT_USER, SubNamesKey, 0L, KEY_READ, &hKeyNames) ;
	if (Status == ERROR_SUCCESS) {	// 履歴情報を取得
		int i;
		char sTmp[TMP_BUF_LEN];
		for (i=0;i<nListMax;i++) {
			sprintf(sTmp,"File%d",i+1);
			Size = sizeof(sList[0]);
			Status = RegQueryValueEx(hKeyNames, sTmp,  NULL, &Type, (LPBYTE)sList[i],  &Size) ;
//			PRT("%s,%s\n",sTmp,sRecentFile[i]);
			if (Status != ERROR_SUCCESS) break;
			(*pList) = i+1;
		}
		RegCloseKey (hKeyNames);
	}
	return Status;
}


// レジストリから情報を読み込む ---> 情報Windowはメインを殺す前に消されるので判定が難しい・・・。
BOOL LoadRegCgfInfo(void)	
{
	HKEY    hKeyNames;
	DWORD   Status;
	TCHAR   SubNamesKey[TMP_BUF_LEN] ;

	sprintf(SubNamesKey,"%s\\%s",CgfNamesKey,FileListName);	// Yssの下にFile用の格納場所を
	LoadRegList(SubNamesKey, &nRecentFile, RECENT_FILE_MAX, sRecentFile);
	sprintf(SubNamesKey,"%s\\%s",CgfNamesKey,NameListName);
	LoadRegList(SubNamesKey, &nNameList, NAME_LIST_MAX, sNameList);
	sprintf(SubNamesKey,"%s\\%s",CgfNamesKey,GtpPathName);
	LoadRegList(SubNamesKey, &nGtpPath, GTP_PATH_MAX, sGtpPath);
	sprintf(SubNamesKey,"%s\\%s",CgfNamesKey,NngsIPName);
	LoadRegList(SubNamesKey, &nNngsIP, NNGS_IP_MAX, sNngsIP);

	Status = RegOpenKeyEx(HKEY_CURRENT_USER, CgfNamesKey, 0L, KEY_READ, &hKeyNames) ;
	if (Status == ERROR_SUCCESS) {
		ReadRegistoryInt(hKeyNames, SoundDropName,   &SoundDrop,             1);
		ReadRegistoryInt(hKeyNames, BoardColorName,  &ID_NowBanColor, ID_BAN_1);
		ReadRegistoryInt(hKeyNames, InfoWindowName,  &fInfoWindow,           0);
		ReadRegistoryInt(hKeyNames, BoardSizeName,   &ban_size,         BAN_19);
		ReadRegistoryInt(hKeyNames, GnugoLifeName,   &fGnugoLifeDeathView,   1);
		ReadRegistoryInt(hKeyNames, TurnPlayer0Name, &turn_player[0], PLAYER_HUMAN);
		ReadRegistoryInt(hKeyNames, TurnPlayer1Name, &turn_player[1], PLAYER_DLL);
		ReadRegistoryInt(hKeyNames, NameListNumName, &nNameList,             0);
		ReadRegistoryInt(hKeyNames, GtpPathNumName,  &nGtpPath,              0);
		ReadRegistoryInt(hKeyNames, NngsIPNumName,   &nNngsIP,               0);
		ReadRegistoryInt(hKeyNames, NngsTimeMinutes, &nngs_minutes,         40);
		ReadRegistoryInt(hKeyNames, ATViewName,      &fATView,               0);

		if ( UseDefaultScreenSize ) fInfoWindow = 1;	// 常にデバグ画面は表示
		RegCloseKey (hKeyNames);
		return (TRUE);
	}

	// open registry failed, use input from startup info or Max as default
	PRT("レジストリのオープン(細かい情報)に失敗！\n");
	return (FALSE) ;
}

// レジストリからCPUの情報を読み込む
void LoadRegCpuInfo(void)
{
	HKEY  hKeyNames;
	DWORD Size,Type,Status;
	static TCHAR CpuFsbKey[] = TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");

	sprintf(sYourCPU,"");
	Status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, CpuFsbKey, 0L, KEY_READ, &hKeyNames);
	if (Status == ERROR_SUCCESS) {
		Size = sizeof(sYourCPU);
		Status = RegQueryValueEx(hKeyNames, "ProcessorNameString",  NULL, &Type, (LPBYTE)sYourCPU, &Size);
		RegCloseKey (hKeyNames);
		if (Status != ERROR_SUCCESS) PRT("CPU取得失敗\n");
	}
	PRT("%s\n",sYourCPU);
}



// 文字列配列の候補リストを更新。一番最近選んだものを先頭へ。
void UpdateStrList(string sAdd, int *pListNum, const int nListMax, char sList[][TMP_BUF_LEN] )
{
	int i,j,nLen;
	char sTmp[TMP_BUF_LEN];

	nLen = strlen(sAdd);
	if ( nLen >= TMP_BUF_LEN ) return;
	for (i=0;i<(*pListNum);i++) {
		if ( strcmp(sList[i], sAdd) == 0 ) break;	// 既にリストにあるファイルを開いた
	}
	if ( i == (*pListNum) ) {	// 新規に追加。古いものを削除。
		for (i=nListMax-1;i>0;i--) strcpy(sList[i],sList[i-1]);	// スクロール
		strcpy(sList[0],sAdd);
		if ( (*pListNum) < nListMax ) (*pListNum)++;
	} else {					// 既に開いたファイルを先頭に。
		strcpy(sTmp,    sList[i]);
		for (j=i;j>0;j--) strcpy(sList[j],sList[j-1]);	// スクロール
		strcpy(sList[0],sTmp    );
	}
}

// ファイルリストを更新。一番最近開いたものを先頭へ。
void UpdateRecentFile(string sAdd)
{
	UpdateStrList(sAdd, &nRecentFile, RECENT_FILE_MAX, sRecentFile );
}

// メニューを変更する
void UpdateRecentFileMenu(HWND hWnd)
{
	HMENU hMenu,hSubMenu;
	int i;
	char sTmp[TMP_BUF_LEN];

	hMenu    = GetMenu(hWnd);
	hSubMenu = GetSubMenu(hMenu, 0);

	for (i=0;i<RECENT_FILE_MAX;i++) {
		DeleteMenu(hSubMenu, IDM_RECENT_FILE + i, MF_BYCOMMAND );
	}

	for (i=0;i<nRecentFile;i++) {
		if ( i < 10 ) sprintf(sTmp,"&%d ",(i+1)%10);
		else          sprintf(sTmp,"  ");
		strcat(sTmp,sRecentFile[i]);
		InsertMenu(hSubMenu, RECENT_POS+i,MF_BYPOSITION,IDM_RECENT_FILE + i,sTmp);
//		AppendMenu(hSubMenu, MF_STRING, IDM_RECENT_FILE + i, sTmp);
	}
}

// ファイルリストから1つだけ削除。ファイルが消えたので。
void DeleteRecentFile(int n)
{
	int i;

	// 新規に追加。古いものを削除。
	for (i=n;i<nRecentFile-1;i++) strcpy(sRecentFile[i],sRecentFile[i+1]);	// スクロール
	nRecentFile--;
	if ( nRecentFile < 0 ) { PRT("DeleteRecent Err!\n"); debug(); }
}



HMODULE hLib = NULL;	// アマゾン思考用のDLLのハンドル

#define CGFTHINK_DLL "cgfthink.dll"

// DLLを初期化する。成功した場合は0を。エラーの場合はそれ以外を返す
int InitCgfGuiDLL(void)
{
	hLib = LoadLibrary(CGFTHINK_DLL);
	if ( hLib == NULL ) return 1;

	// 使う関数のアドレスを取得する。
	cgfgui_thinking = (int(*)(int[], int [][3], int, int, int, double, int, int[]))GetProcAddress(hLib,"cgfgui_thinking");
	if ( cgfgui_thinking == NULL ) {
		PRT("Fail GetProcAdreess 1\n");
		FreeLibrary(hLib);
		return 2;
	}
	cgfgui_thinking_init = (void(*)(int *))GetProcAddress(hLib,"cgfgui_thinking_init");
	if ( cgfgui_thinking_init == NULL ) {
		PRT("Fail GetProcAdreess 2\n");
		FreeLibrary(hLib);
		return 3;
	}
	cgfgui_thinking_close = (void(*)(void))GetProcAddress(hLib,"cgfgui_thinking_close");
	if ( cgfgui_thinking_close == NULL ) {
		PRT("Fail GetProcAdreess 3\n");
		FreeLibrary(hLib);
		return 4;
	}

	return 0;
}

// DLLを解放する。
void FreeCgfGuiDLL(void)
{
	if ( hLib == NULL ) return;
	cgfgui_thinking_close();
	FreeLibrary(hLib);
}



#define BOARD_SIZE_MAX ((19+2)*256)		// 19路盤を最大サイズとする

// 着手位置を返す
int ThinkingDLL(int endgame_type)
{
	// DLLに渡す変数
	static int dll_init_board[BOARD_SIZE_MAX];
	static int dll_kifu[KIFU_MAX][3];
	static int dll_tesuu;
	static int dll_black_turn;
	static int dll_board_size;
	static double dll_komi;
	static int dll_endgame_type;
	static int dll_endgame_board[BOARD_SIZE_MAX];
	// 
	int x,y,z,i,ret_z;
	clock_t ct1;

	// 現在の棋譜をセットする。
	for (i=0;i<BOARD_SIZE_MAX;i++) {
		dll_init_board[i]    = 3;	// 枠で埋める
		dll_endgame_board[i] = 3;
	}
	for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
		z = (y+1) * 256 + (x+1);
		dll_init_board[z]    = ban_start[z];
		dll_endgame_board[z] = 0;
	}
	for (i=0;i<tesuu;i++) {
		dll_kifu[i][0] = kifu[i+1][0];	// CgfGui内部でkifu[0]は未使用なのでずらす。
		dll_kifu[i][1] = kifu[i+1][1];
		dll_kifu[i][2] = kifu[i+1][2];
	}
	dll_tesuu        = tesuu;
	dll_black_turn   = (GetTurnCol() == 1);
	dll_board_size   = ban_size;
	dll_komi         = komi;
	dll_endgame_type = endgame_type;

	// DLLの関数を呼び出す。
	ct1 = clock();
	thinking_utikiri_flag = 0;	// 中断ボタンを押された場合に1に。
	fPassWindows = 1;			// DLLを呼び出し中は中断ボタン以外を使用不能に
	MenuEnableID(IDM_DLL_BREAK, 1);
	ret_z = cgfgui_thinking(dll_init_board, dll_kifu, dll_tesuu, dll_black_turn, dll_board_size, dll_komi, dll_endgame_type, dll_endgame_board);
	fPassWindows = 0;
	MenuEnableID(IDM_DLL_BREAK, 0);
	PRT("位置=(%2d,%2d),時間=%2.1f(秒),手数=%d,終局=%d\n",ret_z&0xff,ret_z>>8,GetSpendTime(ct1),dll_tesuu,endgame_type);
	PRT("----------------------------------------------\n");

	// 終局、図形、数値表示用の盤面に値をセット
	if ( endgame_type != ENDGAME_MOVE ) {
		for (i=0;i<BOARD_SIZE_MAX;i++) endgame_board[i] = dll_endgame_board[i];
	}

	return ret_z;
}

/*
void TestDllThink(void)
{
	static int fFirst = 1;	// 最初の1度だけDLLを初期化
	if ( fFirst ) {	
		if ( InitCgfGuiDLL() == 0 ) return;	// DLLを初期化する。
		cgfgui_thinking_init(&thinking_utikiri_flag);
		fFirst = 0;
	}
	ThinkingTestDLL();
//	FreeCgfGuiDLL();	// DLLを解放する。
}
*/

