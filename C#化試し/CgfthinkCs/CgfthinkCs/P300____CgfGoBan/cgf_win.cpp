/*** cgf_win.cpp --- Windows�Ɉˑ����镔������Ɋ܂� --- ***/
//#pragma warning( disable : 4115 4201 4214 4057 4514 )  // �x�����b�Z�[�W 4201 �𖳌��ɂ���B
#include <Windows.h>
#include <MMSystem.h>	// PlaySound()���g���ꍇ�ɕK�v�BWindows.h �̌�ɑg�ݍ��ށB����� WINMM.LIB ��ǉ������N
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include <CommCtrl.h>	// ToolBar ���g�p���邽�߂̒�` comctl32.lib �������N����K�v����
#if (_MSC_VER >= 1100)	// VC++5.0�ȍ~�ŃT�|�[�g
#include <zmouse.h>		// IntelliMouse���T�|�[�g���邽�߂ɖ�����胊���N�i���v�H�j
#endif

#include "cgf.h"
#include "cgf_rc.h"
#include "cgf_wsk.h"
#include "cgf_pipe.h"

//#include "cgfthink.h"





int endgame_board[BANMAX];	// �I�Ǐ����A�}�`�A���l�\���p�̔Ֆ�
int endgame_board_type;		// ���݁A�I�ǁA�}�`�A���l�̂ǂ��\������

/* �v���g�^�C�v�錾 ---------------------- �ȉ�Windows�ŗL�̐錾���܂� */

HWND ghWindow;					/* WINDOWS�̃n���h��  */
HINSTANCE ghInstance;			/* �C���X�^���X       */
HWND hPrint = NULL;				/* ���o�͗p�̃��[�h���X�_�C�A���O */
extern HWND hWndToolBar;		// TOOLBAR�p�̃n���h��
HWND hWndDraw;
HWND hWndMain;
HACCEL hAccel;					// �A�N�Z�����[�^�L�[�̃e�[�v���̃n���h��
HWND hWndToolBar;				// ToolBar

#define PMaxY 400				// ����ʂ̏c�T�C�Y�i�S���j
#define PHeight 42				// ��ʂ̕W������(32���ʏ�) 
#define PRetX 100				// 100�s�ŉ��s
#define PMaxX 256				// 1�s�̍ő啶����(�Ӗ��Ȃ��H�j

char cPrintBuff[PMaxY][PMaxX];	// �o�b�t�@			
static int nHeight;
static int nWidth;
static int PRT_y;
static int PRT_x = 0;

int ID_NowBanColor = ID_BAN_1;	// �f�t�H���g�̔Ղ̐F
int fFileWrite = 0;				// �t�@�C���ɏ������ރt���O
int UseDefaultScreenSize;		// �Œ�̉�ʃT�C�Y���g��
char cDragFile[MAX_PATH];		// �h���b�O���ꂽ�J���ׂ��t�@�C���� ---> ���̓_�u���N���b�N�ŊJ���Ƃ���
char sCgfDir[MAX_PATH];			// cgfgoabn.exe ������f�B���N�g��
int SoundDrop = 0;				// ���艹�Ȃ�
int fInfoWindow = 0;			// ���\������
int fPassWindows;				// Windows�ɏ�����n���Ă�B�\���ȊO��S�ăJ�b�g
int fChineseRule = 0;			// �������[���̏ꍇ
int thinking_utikiri_flag;
int turn_player[2] = { 0,1 };	// �΋ǎ҂̎�ނ�����B��{�� ��:�l��, ��:Computer
char sDllName[] = "CPU";		// �W���̖��O�ł��B�K���ɕҏW���ĉ������B
//char sGnugoDir[MAX_PATH];		// gnugo.exe������t���p�X
int fGnugoLifeDeathView=0;		// gnugo�̔���Ŏ����\��������
int fATView = 0;				// ���W�\����A�`T�ɁB�`�F�X�`���iA-T��I���Ȃ��j



/*
2006/03/01 1.02 Visual Studio 2005 Express Edtion�ł��R���p�C���ł���悤�ɁB
2007/01/22      ���O��GTP�G���W��PATH,nngs�T�[�o���𕡐��L������悤�ɁB
2007/09/19 1.03 MoGo�������悤�ɁBstderr�𖳎�����悤�ɁBversion������������ꍇ�͍폜
2008/01/31 1.04 UEC�t��nngs�ɑΉ��BGTP�G���W�������݃`�F�b�N�𖳎��BProgram Files�̂悤�ɋ󔒂��͂��ނƔF���ł��Ȃ������Bpath����傫���B
2009/06/25 1.05 nngs�̐؂ꕉ���̎���(��)��ݒ�\�ɁB
2011/11/16 1.06 Win64�ł̃R���p�C���G���[�������B�x���͂�������o�܂��E�E�E�B
2013/11/14 1.07 �R�~��7.0���w��\�ɁB
2015/03/16 1.08 nngs��human�������𑗂��悤�ɁBAT���W�\��
*/
char sVersion[32] = "CgfGoBan 1.08";	// CgfGoban�̃o�[�W����
char sVerDate[32] = "2015/03/16";		// ���t

int dll_col     = 2;	// DLL�̐F�B�ʐM�ΐ펞�ɗ��p�B
int fNowPlaying = 0;	// ���ݑ΋ǒ����ǂ����B

int total_time[2];		// �v�l���ԁB���[0]�A���[1]�̗݌v�v�l���ԁB

int RetZ;				// �ʐM�Ŏ󂯎���
int RetComm = 0;		// �ʐM��Ԃ������B0...Err, 1...��, 2...OK, 3...NewGame, 4...Answer

int nCommNumber = 1;	// COM�|�[�g�̔ԍ��B

int nHandicap = 0;		// �u�΂͂Ȃ�
int fAutoSave = 0;		// �����̎����Z�[�u�i�ʐM�ΐ펞�j
int RealMem_MB;			// ��������
int LatestScore = 0;	// �Ō�Ɍv�Z�����n
int fGtpClient = 0;		// GTP�̘��S�Ƃ��ē����ꍇ

static clock_t gct1;	// ���Ԍv���p

#define RECENT_FILE_MAX 20		// �ŋߊJ�����t�@�C���̍ő吔�B
char sRecentFile[RECENT_FILE_MAX][TMP_BUF_LEN];	// �t�@�C���̐�΃p�X
int nRecentFile = 0;			// �t�@�C���̐�
#define IDM_RECENT_FILE 2000	// ���b�Z�[�W�̐擪
#define RECENT_POS 12			// �ォ��12�Ԗڂɑ}��

char sYourCPU[TMP_BUF_LEN];		// ���̃}�V����CPU��FSB

#define RGB_BOARD_MAX 8	// �ՖʐF�̐�

COLORREF rgbBoard[RGB_BOARD_MAX] = {	// �Ֆʂ̐F
	RGB(231,134, 49),	// ��{(������)
	RGB(255,128,  0),	// �W
	RGB(198,105, 57),	// �Z���ؖڐF
	RGB(192, 64,  9),	// �Z
	RGB(255,188,121),	// �S���i�ʂ̐F�j
	RGB(  0,255,  0),	// ��
	RGB(255,255,  0),	// ��
	RGB(255,255,255),	// ��
};

COLORREF rgbBack  = RGB(  0,  0,  0);	// ���A�w�i�̐F
//COLORREF rgbBack  = RGB(231,255,231);	// �w�i�̐F
//COLORREF rgbBack  = RGB(247,215,181);	// �w�i�̐F
//COLORREF rgbBack  = RGB(  0,128,  0);	// �Z��
//COLORREF rgbBack  = RGB(  5, 75, 49);	// �Z��(�ʂ̔w�i)

COLORREF rgbText  = RGB(255,255,255);	// �ʒu�A���O�̐F

COLORREF rgbBlack = RGB(  0,  0,  0);
COLORREF rgbWhite = RGB(255,255,255);

#define NAME_LIST_MAX 30
char sNameList[NAME_LIST_MAX][TMP_BUF_LEN];	// �΋ǎ҂̖��O�̌��
int nNameList;								// ���O���̐�
//int nNameListSelect[2];					// ���݂̑΋ǎ҂̔ԍ�

#define GTP_PATH_MAX 30
char sGtpPath[GTP_PATH_MAX][TMP_BUF_LEN];	// GTP�G���W���̏ꏊ
int nGtpPath;								// ���O���̐�

#define NNGS_IP_MAX 30
string[] sNngsIP;	// nngs�̃T�[�o�� [NNGS_IP_MAX]
int nNngsIP;							// ���̐�

#define GUI_DLL_MOVE  0
#define GUI_USER_PASS 1
#define GUI_GTP_MOVE  2

// SGF �ɂ���f�[�^
char sPlayerName[2][TMP_BUF_LEN];	// �΋ǎ҂̖��O�B[0] ���A[1] ���B
char sDate[TMP_BUF_LEN];			// ���t
char sPlace[TMP_BUF_LEN];			// �ꏊ
char sEvent[TMP_BUF_LEN];			// ��
char sRound[TMP_BUF_LEN];			// ����킩
char sKomi[TMP_BUF_LEN];			// ����
char sTime[TMP_BUF_LEN];			// ����
char sGameName[TMP_BUF_LEN];		// �΋ǖ�
char sResult[TMP_BUF_LEN];			// ����

#define CGF_ICON "CGF1"


#ifdef CGF_E
char szTitleName[] = "CgfGoBan";
char szInfoWinName[] = "Information Window";
#else
char szTitleName[] = "CGF���";
char szInfoWinName[] = "���\��";
#endif





// ���݁A�ʐM�ΐ풆��
int IsCommunication(void)
{
	int i,n;

	for (i=0;i<2;i++) {
		n = turn_player[i];
		if ( n == PLAYER_NNGS || n == PLAYER_SGMP ) return 1;
	}
	return 0;
}

// �΋ǒ��f�����B�ʐM�ΐ풆�Ȃ�|�[�g�Ȃǂ����B
void StopPlayingMode(void)
{
	if ( IsCommunication() ) cgf_win_PlayDialog.CloseConnection();
	SendMessage(ghWindow,WM_COMMAND, IDM_BREAK, 0L);
	PaintUpdate();
}

// �����ʐM�ő�������A���̎�ԂɈڂ�����
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
		if ( SoundDrop ) sndPlaySound("stone.wav", SND_ASYNC | SND_NOSTOP);	// MMSYSTEM.H ���C���N���[�h����K�v����B�܂�WinMM.lib�������N
	}

	// ���̎�Ԃ̏����Ɉڂ�
	PostMessage(hWnd,WM_COMMAND,IDM_NEXT_TURN,0L);
	return 0;
}

/*
// �u�΂�2�q�`9�q�܂ł͉��̂悤�ɁB

�{�{��	// 2�q  
�{�{�{
���{�{

�{�{��	// 3�q
�{�{�{
���{��

���{��	// 5�q
�{���{
���{��

���{��	// 6�q
���{��
���{��

���{��	// 7�q
������
���{��

������	// 8�q
���{��
������

�{�{�{�{��	// 10�q
�{�������{
�{�������{
�{�������{
�{�{�{�{�{

�{�{�{�{��	// 12�q
�{�������{
�{�������{
�{�������{
���{�{�{��
*/
// �u�΂�ݒ�
void SetHandicapStones(void)
{
	int i,k;
	
	SendMessage(ghWindow,WM_COMMAND,IDM_BAN_CLEAR,0L);	// �Ֆʏ�����
	k = nHandicap;
	if ( ban_size == BAN_19 ) {
		ban[0x0410] = 1;		// �Ίp���ɂ͕K���u��
		ban[0x1004] = 1;	
		if ( k >= 4 ) {
			ban[0x0404] = 1;	// 4�q�ȏ�͐��͑S��
			ban[0x1010] = 1;	
		}
		if ( k >= 6 ) {
			ban[0x0a04] = 1;	// 6�q�ȏ�͍��E�����̐�
			ban[0x0a10] = 1;	
		}
		if ( k >= 8 ) {
			ban[0x040a] = 1;	// 8�q�ȏ�͏㉺�����̐�
			ban[0x100a] = 1;
		}
		if ( k == 3 ) ban[0x1010] = 1;
		if ( k == 5 || k == 7 || k >= 9 ) ban[0x0a0a] = 1;
	} else if ( ban_size == 13 ) {
		ban[0x040a] = 1;		// �Ίp���ɂ͕K���u��
		ban[0x0a04] = 1;	
		if ( k >= 4 ) {
			ban[0x0404] = 1;	// 4�q�ȏ�͐��͑S��
			ban[0x0a0a] = 1;	
		}
		if ( k >= 6 ) {
			ban[0x0704] = 1;	// 6�q�ȏ�͍��E�����̐�
			ban[0x070a] = 1;	
		}
		if ( k >= 8 ) {
			ban[0x0407] = 1;	// 8�q�ȏ�͏㉺�����̐�
			ban[0x0a07] = 1;
		}
		if ( k == 3 ) ban[0x0a0a] = 1;
		if ( k == 5 || k == 7 || k >= 9 ) ban[0x0707] = 1;
	} else if ( ban_size == BAN_9 ) {
		ban[0x0307] = 1;		// �Ίp���ɂ͕K���u��
		ban[0x0703] = 1;	
		if ( k >= 4 ) {
			ban[0x0303] = 1;	// 4�q�ȏ�͐��͑S��
			ban[0x0707] = 1;	
		}
		if ( k >= 6 ) {
			ban[0x0503] = 1;	// 6�q�ȏ�͍��E�����̐�
			ban[0x0507] = 1;	
		}
		if ( k >= 8 ) {
			ban[0x0305] = 1;	// 8�q�ȏ�͏㉺�����̐�
			ban[0x0705] = 1;
		}
		if ( k == 3 ) ban[0x0707] = 1;
		if ( k == 5 || k == 7 || k >= 9 ) ban[0x0505] = 1;
	}

	for (i=0;i<BANMAX;i++) {
		ban_start[i] = ban[i];	// �J�n�Ֆʂɂ����
	}
	SendMessage(ghWindow,WM_COMMAND,IDM_SET_BOARD,0L);	// �Ֆʏ�����
}


/* ���\���_�C�A���O */
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
		hBrush = CreateSolidBrush( RGB( 0,0,128 ) );	// �w�i��΂�(wc.���̃I�u�W�F�N�g�͏I�����Ɏ����I�ɍ폜�����j
		wc.hbrBackground = hBrush;//	GetStockObject(WHITE_BRUSH);
//		wc.hbrBackground = GetStockObject(WHITE_BRUSH);
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		if ( !RegisterClass(&wc) ) MessageBox(ghWindow,"���\��Winodw�̓o�^�Ɏ��s","�G���[",MB_OK);
	}
	/********** �E�C���h�E�̍쐬 ********/
//	GetWindowRect( hWnd, &rc );	// ���݂̉�ʂ̐�Έʒu���擾�B
//	PRT("%d,%d,%d,%d\n",rc.left, rc.top,rc.right, rc.bottom);

	hPrint = CreateWindow(
				"PrintClass",						/* Class name.   */                                  
				szInfoWinName,						/* Title.        */
				WS_VSCROLL | WS_OVERLAPPEDWINDOW,	/* Style bits.   */
//				rc.right, rc.top,					/* X,Y  �\�����W */
				640, 20,							/* X,Y  �\�����W */
//				600, 50,							/* X,Y  �\�����W */
//				700,560,							/* CX, CY �傫�� */
				700,PHeight*17,						/* CX, CY �傫�� */
				hWnd,								/* �e�͂Ȃ�      */
				NULL,								/* Class Menu.   */
				ghInstance,							/* Creator       */
				NULL								/* Params.       */
			   );

	LoadMainWindowPlacement(hPrint,2);	// ���W�X�g����������擾
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
			PRT_y = PMaxY - PHeight;		// ��{��25�s�\��
			SetScrollRange( hWnd, SB_VERT, 0, PRT_y, FALSE);	
			SetScrollPos( hWnd, SB_VERT, PRT_y, FALSE );
			hDC = GetDC(hWnd);

//			hFont = GetStockObject(OEM_FIXED_FONT);
//			hFont = GetStockObject(SYSTEM_FONT);
//			hFont = GetStockObject(ANSI_FIXED_FONT);

			hFont = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);	// Win98�ŕ�����������Ȃ��悤��
			hOldFont = (HFONT)SelectObject(hDC,hFont);

			GetTextMetrics(hDC, &tm);
			nHeight = tm.tmHeight;
			nHeight -= 2;	// �킴��2�h�b�g�c�����炵�Ă���B���ʂ������� ���FIXED_FONT�Ŏg�p
			nWidth  = tm.tmAveCharWidth;
			SetTextColor( hDC, RGB(255,255,255) );	// �����𔒂�
			SetBkColor( hDC, RGB(0,0,128) );		// �w�i��Z����
			SetBkMode( hDC, TRANSPARENT);			// �w�i�������Ȃ��i�����̏㉺��1�h�b�g�������̂�h���j
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
				case SB_THUMBPOSITION:	// �T���ړ�
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

#if (_MSC_VER >= 1100)		// VC++5.0�ȍ~�ŃT�|�[�g
		case WM_MOUSEWHEEL:	// ---> ����͖������ <zmouse.h> �Œ�`���Ă���B
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
//				PRT("�E�N���b�N���ꂽ�� (x,y)=%d,%d\n",pt.x,pt.y);
				hMenu = LoadMenu(ghInstance, "PRT_POPUP");
				hSubmenu = GetSubMenu(hMenu, 0);
			
				ClientToScreen(hWnd, &pt);
				TrackPopupMenu(hSubmenu, TPM_LEFTALIGN, pt.x, pt.y, 0, ghWindow, NULL);	// POPUP�̐e�����C���ɂ��Ȃ��ƃ��b�Z�[�W����������΂Ȃ��B
				DestroyMenu(hMenu);
			}
			break;

		case WM_CLOSE:
			fInfoWindow = 0;	// ���[�U��PrintWindow�݂̂���悤�Ƃ����ꍇ�B
			return( DefWindowProc( hWnd, msg, wParam, lParam) );

		case WM_DESTROY:
			hMenu = GetMenu(ghWindow);
			CheckMenuItem(hMenu, IDM_PRINT, MF_UNCHECKED);
			DeleteObject(SelectObject(hDC, hOldFont));	/* �t�H���g����� */
			if ( SaveMainWindowPlacement(hPrint,2) == 0 ) PRT("���W�X�g���Q�̃G���[�i�������݁j\n");
			hPrint = NULL;
			break;

		default:
			return( DefWindowProc( hWnd, msg, wParam, lParam) );
	}
	return 0L;
}


#define CGF_URL "http://www32.ocn.ne.jp/~yss/cgfgoban.html"

/* �A�o�E�g�̕\�� */
LRESULT CALLBACK AboutDlgProc( HWND hDlg, UINT msg, WPARAM wParam, Int64 /*lParam*/ )
{
	char sTmp[TMP_BUF_LEN];

	switch( msg ) {
		case WM_INITDIALOG:
			sprintf(sTmp,"%s %s",sVersion,sVerDate);
			SetDlgItemText(hDlg, IDM_CGF_VERSION, sTmp);
			SetDlgItemText(hDlg, IDM_HOMEPAGE, CGF_URL);
			return TRUE;	// TRUE ��Ԃ��� SetFocus(hDlg); �͕K�v�Ȃ��H

		case WM_COMMAND:
			if ( wParam == IDM_HOMEPAGE ) ShellExecute(hDlg, NULL, CGF_URL, NULL, NULL, SW_SHOWNORMAL);	// ���ꂾ����IE5.0?���N�������
			if ( wParam == IDOK || wParam == IDCANCEL ) EndDialog( hDlg, 0);
			break;
	}
	return FALSE;
}

// Gnugo�p�̐ݒ������_�C�A���O�̏���
BOOL CALLBACK GnuGoSettingProc( HWND hDlg, UINT msg, WPARAM wParam, Int64 /*lParam*/)
{
	HWND hWnd;
	int i;

	switch(msg) {
		case WM_INITDIALOG:
			if ( nGtpPath == 0 ) {	// ���o�^�Ȃ�W����ǉ��B�Ōオ�ŐV�B
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
			if ( wParam == IDCANCEL ) EndDialog(hDlg,FALSE);		// �L�����Z��
			if ( wParam == IDOK ) {
				char sTmp[TMP_BUF_LEN];
				GetDlgItemText(hDlg, IDM_GTP_PATH, sTmp, TMP_BUF_LEN-1);
				if ( strlen(sTmp) > 0 ) {	// ������z��̌�⃊�X�g���X�V�B
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


// �����FStartProc
//
// �΋Ǐ�����ݒ肷��_�C�A���O�{�b�N�X��\������C�x���g�E�n���h���[�B
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
	static string handi_str[handi_max] = { "�Ȃ�","2�q","3�q","4�q","5�q","6�q","7�q","8�q","9�q" };
	static string err_str[] = { "���҂Ƃ��ʐM�΋ǂ͎w��ł��܂���" };
	static string sKomi[3] = { "�Ȃ�","����","�ڔ�" };
	static string sSize[3] = { "9�H","13�H","19�H" };
#endif
	static int fInitBoard = 0;	// �ǖʂ�����������ꍇ

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

// ���b�Z�[�W��\�����đ΋ǂ��~
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
		"���E��",
		"�R�E",
		"���ɐ΂�����܂�",
		"���̑��̃G���[",
		"�A���p�X�ŏI�ǂ��܂�",
		"�萔���������邽�ߒ��f���܂�",
		"�����ł�",
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


/*** �`��p�qWINDOW�̏����B����Window�ɕ`����s���B ***/
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
		"�����\�����͐΂͒u���܂���B\n",
		"�����ɂ͐΂�����܂���B\n",
		"���E��ł��I",
		"���ł���I",
		"�����\�����͐΂͎��܂���B\n",
#endif
	};

	if ( fPassWindows ) {	// �v�l���͂��ׂăJ�b�g����B
		return( DefWindowProc( hWnd, msg, wParam, lParam) );
	}

	switch( msg ) {
		case WM_PAINT:
			PaintBackScreen(hWnd);
			return 0;

		case WM_LBUTTONDOWN:
			if ( fNowPlaying == 0 ) {	// �ՖʕҏW�@
//				SetIshi( z, 1 );
				return 0; 
			}
			z = ChangeXY(lParam);
//			PRT("����%x  ",z);
			if ( z == 0 ) return 0;
			if ( endgame_board_type ) {
				PRT("%s",err_str[0]);
				return 0;
			}
			if ( ban[z] != 0 ) {
				PRT("%s",err_str[1]);
				return 0;
			}

			col = GetTurnCol();		// ��Ԃƒu�΂��肩�H�ŐF�����߂�

			k = make_move(z,col);
			if ( k==MOVE_SUICIDE ) {
				MessageBox(hWnd,err_str[2],"Move error",MB_OK);	// ���E��
				return 0;
			}
			if ( k==MOVE_KOU ) {
				MessageBox(hWnd,err_str[3],"Move error",MB_OK);	// �R�E
				return 0;
			}
			if ( k!=MOVE_SUCCESS ) {
				PRT("move_err\n");
				debug();
			}
			
			if ( IsGnugoBoard()    ) UpdateGnugoBoard(z);
			if ( IsCommunication() ) SendMove(z);

			tesuu++;	all_tesuu = tesuu;
			kifu[tesuu][0] = z;	// �������L��
			kifu[tesuu][1] = col;	// ��ԂŐF�����߂�
			kifu[tesuu][2] = (int)GetSpendTime(gct1);

			PaintUpdate();
			if ( SoundDrop ) {
//				MessageBeep(MB_OK);	// ����炵�Ă݂܂���
				sndPlaySound("stone.wav", SND_ASYNC | SND_NOSTOP);	// MMSYSTEM.H ���C���N���[�h����K�v����B�܂�WinMM.lib�������N
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
			if ( fNowPlaying == 0 ) {	// �ՖʕҏW�@
				SetIshi( z, 2 );
			}
			return 0;
*/
		default:
			return( DefWindowProc( hWnd, msg, wParam, lParam) );
	}
//	return 0L;
}

// �`��pWindow�𓯎��ɓ�����
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

// Windows�ֈꎞ�I�ɐ����n���BUser ����̎w���ɂ��v�l��~�B
void PassWindowsSystem(void)
{
	MSG msg;

	fPassWindows = 1;	// Windows�ɏ�����n���Ă�B
	if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) ) {
		TranslateMessage( &msg );						// keyboard input.
		DispatchMessage( &msg );
	}
	fPassWindows = 0;
}	


/*** �������Z�[�u���� ***/
void KifuSave(void)
{
	OPENFILENAME ofn;
	char szDirName[256];
	char szFile[256], szFileTitle[256];
	UINT  cbString;
	char  chReplace;    /* szFilter������̋�؂蕶�� */
	char  szFilter[256];
	char  Title[256];	// Title
	HANDLE hFile;		// �t�@�C���̃n���h��
	int i;
	unsigned long RetByte;		// �������܂ꂽ�o�C�g���i�_�~�[�j
	static int OnlyOne = 0;	// �A���ΐ�̉񐔁i�ŏ���1�񂾂��������܂��)�B

	time_t timer;
	struct tm *tblock;
	unsigned int month,day,hour,minute;

	/* �V�X�e�� �f�B���N�g�������擾����szDirName�Ɋi�[ */
	GetSystemDirectory(szDirName, sizeof(szDirName));
	szFile[0] = '\0';

	if ((cbString = LoadString(ghInstance, IDS_SAVE_FILTER,
	        szFilter, sizeof(szFilter))) == 0) {
	    return;
	}
	chReplace = szFilter[cbString - 1]; /* ���C���h �J�[�h���擾 */

	for (i = 0; szFilter[i] != '\0'; i++) {
	    if (szFilter[i] == chReplace)
	       szFilter[i] = '\0';
	}


	/* �����̎擾 */
	timer = time(NULL);
	tblock = localtime(&timer);   /* ���t�^�������\���̂ɕϊ����� */
//	PRT("Local time is: %s", asctime(tblock));
//	second = tblock->tm_sec;      /* �b */
	minute = tblock->tm_min;      /* �� */
	hour   = tblock->tm_hour;     /* �� (0�`23) */
	day    = tblock->tm_mday;     /* �����̒ʂ����� (1�`31) */
	month  = tblock->tm_mon+1;    /* �� (0�`11) */
//	year   = tblock->tm_year + 1900;     /* �N (���� - 1900) */
//	week   = tblock->tm_wday;     /* �j�� (0�`6; ���j = 0) */
//	PRT( "Date %02x-%02d-%02x / Time %02x:%02x:%02x\n", year, month, day, hour, minute, second );

//	sprintf( szFileTitle, "%02d%02d%02d%02d", month, day, hour, minute );	// �t�@�C��������t�{�b��
//	if ( fAutoSave ) sprintf( szFile, "fig%03d.sgf",++OnlyOne );	// �����Z�[�u�̎��B
	if ( fAutoSave ) sprintf( szFile, "%x%02d@%02d%02d.sgf", month, day, hour,++OnlyOne );	// �����Z�[�u�̎��B
	else             sprintf( szFile, "%x%02d-%02d%02d",     month, day, hour, minute );	// �t�@�C��������t�{�b��


	/* �\���̂̂��ׂẴ����o��0�ɐݒ� */
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
	ofn.lpstrInitialDir = NULL;		// �J�����g�f�B���N�g����
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt	= "sgf";	// �f�t�H���g�̊g���q�� ".sgf" ��
	
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

		// �^�C�g���Ƀt�@�C������t����
		sprintf(Title,"%s - %s",ofn.lpstrFile, szTitleName);
		SetWindowText(ghWindow, Title);

		// �t�@�C�����X�g���X�V�B��ԍŋߊJ�������̂�擪�ցB
		UpdateRecentFile(ofn.lpstrFile);
		UpdateRecentFileMenu(ghWindow);

		if ( stricmp(ofn.lpstrFile + ofn.nFileExtension, "sgf" ) == 0 || fAutoSave ) {
			SaveSGF();	// SGF�ŏ����o��
		} else {
			SaveIG1();	// IG1 �`���ŏ������ށB
		}
		WriteFile( hFile,(void *)SgfBuf, strlen(SgfBuf), &RetByte, NULL );
		CloseHandle(hFile);		// �t�@�C�����N���[�Y
	}
}


/*** ���������[�h���� ***/
int KifuOpen(void)
{
	OPENFILENAME ofn;
	char szDirName[256];
	char szFile[256], szFileTitle[256];
	UINT  cbString;
	char  chReplace;    /* szFilter������̋�؂蕶�� */
	char  szFilter[256];
	char  Title[256];	// Title

	HANDLE hFile;		// �t�@�C���̃n���h��
	int i;

	szFile[0] = '\0';
	if ((cbString = LoadString(ghInstance, IDS_LOAD_FILTER, szFilter, sizeof(szFilter))) == 0) return FALSE;
	chReplace = szFilter[cbString - 1]; /* ���C���h �J�[�h���擾 */
	for (i = 0; szFilter[i] != '\0'; i++) if (szFilter[i] == chReplace) szFilter[i] = '\0';

	/* �\���̂̂��ׂẴ����o��0�ɐݒ� */
	memset(&ofn, 0, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = ghWindow;
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile= szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);
	GetCurrentDirectory(sizeof(szDirName), szDirName);	// �J�����g�f�B���N�g�������擾����szDirName�Ɋi�[
	ofn.lpstrInitialDir = szDirName;	// �J�����g�f�B���N�g����
//	ofn.lpstrInitialDir = NULL;			// ---> ���ꂾ��Win98�ȍ~��*.amz�t�@�C�����Ȃ���MyDocuments���W���ɂ����
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if ( strlen(cDragFile) || GetOpenFileName(&ofn) ) {
		if ( strlen(cDragFile) ) {	// �_�u���N���b�N�ŋN�����ꂽ��
			string p;

			if ( cDragFile[0] == '"' ) {	// �O��� "" ������
				strcpy(szFile,cDragFile+1);
				strcpy(cDragFile,szFile);
				cDragFile[ strlen(cDragFile)-1 ] = 0;
			}

			strcpy(szFile,cDragFile);	// szFile�ɃR�s�[
			ofn.lpstrFile = szFile;
			p = strrchr( ofn.lpstrFile, '.' );	// �Ō�̃s���I�h��T��
			if ( p == NULL ) ofn.nFileExtension = 0;	// �ǂ݂̂��G���[
			else             ofn.nFileExtension = (p - ofn.lpstrFile)+1;
			PRT("%s,%d,%s\n",ofn.lpstrFile,ofn.nFileExtension,ofn.lpstrFile + ofn.nFileExtension);
			strcpy(ofn.lpstrFileTitle,ofn.lpstrFile);
			cDragFile[0] = '\0';	// �t�@�C����������	
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
 
		// �^�C�g���Ƀt�@�C������t����
		sprintf(Title,"%s - %s",ofn.lpstrFileTitle, szTitleName);
		SetWindowText(ghWindow, Title);

		// �t�@�C�����X�g���X�V�B��ԍŋߊJ�������̂�擪�ցB
		UpdateRecentFile(ofn.lpstrFile);
		UpdateRecentFileMenu(ghWindow);


		PRT("\n%s\n",ofn.lpstrFile);

		// �S�Ẵt�@�C�����o�b�t�@�ɓǂݍ���
		ReadFile( hFile, (void *)SgfBuf, SGF_BUF_MAX, &nSgfBufSize, NULL );
		CloseHandle(hFile);		// �t�@�C�����N���[�Y

		int fSgf = 0;
		// SGF (Standard Go Format) ��ǂݍ��ށB
		if ( stricmp(ofn.lpstrFile + ofn.nFileExtension, "sgf" ) == 0 ) fSgf = 1;
		// IG1 (Nifty-Serve GOTERM) ��ǂݍ��ށB
		if ( stricmp(ofn.lpstrFile + ofn.nFileExtension, "ig1" ) == 0 ) fSgf = 0;

		LoadSetSGF(fSgf);
	}

//	PRT("�ǂݍ��񂾎萔=%d,�S�萔=%d\n",tesuu,all_tesuu);
	return TRUE;
}








/*** �͌�Ղ̏������W�l���擾�i�͌�Վ��̂̏k���ɔ�����j***/
void GetScreenParam( HWND hWnd,int *dx, int *sx,int *sy, int *db )
{
	RECT rc;
	
	GetClientRect( hWnd, &rc );

	*sx = 22;			// �͌�Ղ̍��[�����W	 
	*sy = 20;

	*dx = 2*(rc.bottom - *sy -15 ) / ( 2*ban_size+1);	// -15 �͋C��
		
	if ( *dx <= 10 ) *dx = 10;	// �Œ��10
	if ( *dx & 1 ) *dx -= 1;	// ������
//	if ( !(*dx & 2) ) *dx -= 2;	// 18 + 4*x �̔{����

	*db = (*dx*3) / 4;	// �͌�Ղ̒[�Ɛ��̊Ԋu
	if ( !(*db & 1) ) *db += 1;	// ���
//	if ( iBanSize==BAN_19 ) *dx = 26;
//	if ( iBanSize==BAN_13 ) *dx = 38;
//	if ( iBanSize==BAN_9  ) *dx = 50;
//	*dx = 26;		// ���ڂ̊Ԋu (640*480..20, 800*600..25, 1280*780..34)
					// 18 <= dx <= 36 (24 +- 4 �̔{���͈Â��ՂŘg���ɂ���)
}

/*** WM_PAINT �ŉ�ʂ��ĕ`�悷��֐� ***/

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
	"��", 
	"��",
	"�g�l�F%d",
	"��%3d��",
	"���̔Ԃł�",
	"���̔Ԃł�",
	"����",
	"%s�� %.1f �ڏ�",
	"(����)",
	"��", 
	"��",
#endif
};

#ifdef CGF_E
char FontName[] = "FixedSys";	// "MS UI Gothic" "MS Sans Serif" "Courier New" "BatangChe"(win98�ł͌��ɂ���)
#else
char FontName[] = "�W������";
#endif

// �Տ�ɐ��l����������
void DrawValueNumber(HDC hDC,int size, int col, int xx, int yy, string sNum)
{
	TEXTMETRIC tm;
	HFONT hFont,hFontOld;
	int n = strlen(sNum);

	hFont = CreateFont(					/* �t�H���g�̍쐬 */
		size,0,0,0,FW_NORMAL,
		0,0,0,	DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
           DEFAULT_PITCH, FontName);
	hFontOld = (HFONT)SelectObject(hDC,hFont);
	GetTextMetrics(hDC, &tm);
	if ( col==2 ) SetTextColor( hDC, rgbBlack );	// ����
	else          SetTextColor( hDC, rgbWhite );	// ����
	TextOut( hDC, xx-tm.tmAveCharWidth*n/2 - (n==3)*1, yy-size/2, sNum ,n);
	SelectObject( hDC, hFontOld );
	DeleteObject(hFont);
}


// �I�Ǐ�����������ŁA�n�̐��݂̂𐔂���B(���n �| ���n)
int count_endgame_score(void)
{
	int x,y,z,col,n,sum;
	
	sum = hama[0] - hama[1];
	for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
		z = ((y+1)<<8) + (x+1);
		col = ban[z];
		n   = endgame_board[z];
		if ( col == 0 ) {	// ��_
			if ( n == GTP_BLACK_TERRITORY ) sum++;	// ���n
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

// �������[���Ōv�Z�i�A�Q�n�}�͖������A���Ώ�����̎����̒n�Ǝ����̐΁A�̍��v�𐔂���B�_���ɑłꍇ��1�ڂƂȂ�j
int count_endgame_score_chinese(void)
{
	int x,y,z,col,n,sum;

	sum = 0;
	for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
		z = ((y+1)<<8) + (x+1);
		col = ban[z];
		n   = endgame_board[z];
		if ( col == 0 ) {	// ��_
			if ( n == GTP_BLACK_TERRITORY ) sum++;	// ���n
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

// �n�𐔂��Č��ʂ𕶎���ŕԂ�
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

// ����ʂɏ����Ĉ�C�ɓ]������BBeginPaint�͂������ōs���B
void PaintBackScreen( HWND hWnd )
{
	PAINTSTRUCT ps;
	HDC hDC;
	HDC memDC;
	HBITMAP bmp,old_bmp;
	int maxX,maxY;
	RECT rc;

	hDC = BeginPaint(hWnd, &ps);

	// ���z�ő��ʃT�C�Y���擾(Desktop�̃T�C�Y�jSM_CXSCREEN ���ƃ}���`�f�B�X�v���C�ɖ��Ή��B
	GetClientRect(hWnd,&rc);
	maxX = rc.right;
	maxY = rc.bottom;
//	maxX = GetSystemMetrics(SM_CXVIRTUALSCREEN);	// �R���p�C�����ʂ�Ȃ��BVC6�ł͂��߁BVC7(.net)�Ȃ�OK�B
//	maxY = GetSystemMetrics(SM_CYVIRTUALSCREEN);
//	PRT("maxX,Y=(%d,%d)\n",maxX,maxY);

	// ���z��ʂ̃r�b�g�}�b�v���쐬
	memDC   = CreateCompatibleDC(hDC);
	bmp     = CreateCompatibleBitmap(hDC,maxX,maxY);
	old_bmp = (HBITMAP)SelectObject(memDC, bmp);

	PaintScreen( hWnd, memDC );		// ���ۂɕ`����s���֐�

	BitBlt(hDC,0,0,maxX,maxY,memDC,0,0, SRCCOPY);	// ����ʂ���C�ɕ`��

	// ��n��
	SelectObject(memDC,old_bmp);	// �֘A�t����ꂽ�I�u�W�F�N�g�����ɖ߂�
	DeleteObject(bmp);				// HDC�Ɋ֘A�t����ꂽGDI�I�u�W�F�N�g�͍폜�ł��Ȃ�����
	DeleteDC(memDC);

	EndPaint(hWnd, &ps);
}


/*** �}�E�X�̍��N���b�N�̎��̈ʒu��xy���W�ɕϊ����ĕԂ� ***/
/*** �ǂ����w���ĂȂ��Ƃ��͂O��Ԃ�                     ***/
int ChangeXY(LONG lParam)
{
	int dx,sx,sy,db;
	int x,y;
	
	GetScreenParam( hWndDraw,&dx, &sx, &sy, &db );
	x = LOWORD(lParam);
	y = HIWORD(lParam);
	x -= sx + db - dx/2;	// �Ղ̍����i�̂���ɐ΂P���̍����j�ɕ��s�ړ�
	y -= sy + db - dx/2;
	if ( x < 0	|| x > dx*ban_size ) return 0;
	if ( y < 0  || y > dx*ban_size ) return 0;
	x = x / dx;
	y = y / dx;
	return ( ((y+1)<<8) + x+1);
}

// �C�ӂ̎萔�̋ǖʂɔՖʂ��ړ�������
// �O�ɐi�ޏꍇ�͊ȒP�A�߂�ꍇ�͏��肩���蒼���B
void move_tesuu(int n)
{
	int i,z,col,loop;

//	PRT("���݂̎萔=%d, �S�萔=%d, ��]�̎萔=%d ...in \n",tesuu,all_tesuu,n);

	if ( n == tesuu ) return;
	if ( endgame_board_type ) {
		endgame_board_type = 0;
		UpdateLifeDethButton(0);
		PaintUpdate();
	}

	if ( n < tesuu ) {	// ���߂��B
		if ( n <= 0 ) n = 0;
		loop = n;
		// �Ֆʂ������Ֆʂ�
		for (i=0;i<BANMAX;i++) ban[i] = ban_start[i];
		reconstruct_ban();
	} else {			// ���i�߂�
		loop = n - tesuu;
		if ( loop + tesuu >= all_tesuu ) loop = all_tesuu - tesuu;	// �ŏI�ǖʂցB
	}

//	PRT("loop=%d,�萔=%d\n",loop,tesuu);
	for (i=0;i<loop;i++) {
		tesuu++;
		z   = kifu[tesuu][0];	// �ʒu
		col = kifu[tesuu][1];	// �΂̐F
//		PRT("���i�߂� z=%x, col=%d\n",z,col);
		if ( make_move(z,col) != MOVE_SUCCESS )	{
			PRT("�����Č����ɃG���[! tesuu=%d, z=%x,col=%d\n",tesuu,z,col);
			break;
		}
	}
//	PRT("���݂̎萔=%d, �S�萔=%d ..end\n",tesuu, all_tesuu);
}

// ���݂̎�Ԃ̐F��Ԃ��i���ݎ萔�Ə��肪���Ԃ��ǂ����Ŕ��f����j
int GetTurnCol(void)
{
	return 1 + ((tesuu+(nHandicap!=0)) & 1);	// �΂̐F
}

// ���̎�Ԃ��H
int IsBlackTurn(void)
{
	if ( GetTurnCol() == 1 ) return 1;
	return 0;
}

// Gnugo�Ƒ΋ǒ����H
int IsGnugoBoard(void)
{
	int i;
	for (i=0;i<2;i++) {
		if ( turn_player[i] == PLAYER_GNUGO ) return 1;
	}
	return 0;
}


// ��ʂ̑S��������
void PaintUpdate(void)
{
	InvalidateRect(hWndDraw, NULL, FALSE);
	UpdateWindow(hWndDraw);
	return;
}

// �ՖʐF�̃��j���[�Ƀ`�F�b�N��
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

// ���j���[�ƃc�[���o�[�̃{�^����W�F�\����
void MenuEnableID(int id, int on)
{
	HMENU hMenu = GetMenu(ghWindow);

	EnableMenuItem(hMenu, id, MF_BYCOMMAND | (on ? MF_ENABLED : MF_GRAYED) );
	SendMessage(hWndToolBar, TB_SETSTATE, id, MAKELPARAM(on ? TBSTATE_ENABLED : TBSTATE_INDETERMINATE, 0));
}

// �΋ǒ��͊����ړ����j���[���֎~����
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

// �����\�����͑S�Ẵ��j���[���֎~����
void UpdateLifeDethButton(int check_on)
{
	MenuEnablePlayMode(check_on);
	UINT iFlags = check_on ? (TBSTATE_PRESSED | TBSTATE_ENABLED) : TBSTATE_ENABLED;
	SendMessage(hWndToolBar, TB_SETSTATE, IDM_DEADSTONE, MAKELPARAM(iFlags, 0));
}


// ToolBar�̐ݒ�
#define NUMIMAGES       17	// �{�^���̐�
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

	InitCommonControls();	// �R�����R���g���[����dll��������
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

// �c�[���o�[�{�^���̏�Ԃ�ύX
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


// WinMain()�̃��b�Z�[�W���[�v�ŏ�������Ɏg��
int IsDlgDoneMsg(MSG msg)
{
	if ( hAccel != NULL && TranslateAccelerator(hWndMain, hAccel, &msg)	!= 0 ) return 0;

//	if ( IsDlgMsg(hDlgPntFgr,      msg) == 0 ) return 0;
	return 1;
}

// ���W�X�g���̏���
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

// ���W�X�g��������i��ʃT�C�Y�ƈʒu�j��ǂݍ���	 flag == 1 ... main window, 2... print window
BOOL LoadMainWindowPlacement (HWND hWnd,int flag)	
{
	WINDOWPLACEMENT   WindowPlacement; 
	TCHAR             szWindowPlacement [TMP_BUF_LEN] ;
	HKEY              hKeyNames;
	DWORD             Size,Type,Status;

	if ( UseDefaultScreenSize ) return TRUE;	// �Œ�̉�ʃT�C�Y���g��

	Status = RegOpenKeyEx(HKEY_CURRENT_USER, CgfNamesKey, 0L, KEY_READ, &hKeyNames) ;
	if (Status == ERROR_SUCCESS) {
		// get the window placement data
		Size = sizeof(szWindowPlacement);

		// Main Window �̈ʒu�𕜌�
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
	PRT("���W�X�g���̃I�[�v���Ɏ��s�I\n");
	ShowWindow (hWnd, SW_SHOW) ;
	return (FALSE) ;
}

// ���W�X�g���ɏ��i��ʃT�C�Y�ƈʒu�j���������� flag == 1 ... main window, 2... print window
BOOL SaveMainWindowPlacement(HWND hWnd,int flag)
{
	WINDOWPLACEMENT   WindowPlacement;
	TCHAR             ObjectType [2] ;
    TCHAR             szWindowPlacement [TMP_BUF_LEN] ;
    HKEY              hKeyNames = 0 ;
    DWORD             Size ;
    DWORD             Status ;
    DWORD             dwDisposition ;
 
	if ( hWnd == NULL ) return FALSE;	// ���\����ʂ���Ă���
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

// ���W�X�g���ɏ��i���ʉ��A��ʂ̐F�j����������
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

	sprintf(SubNamesKey,"%s\\%s",CgfNamesKey,FileListName);	// Yss�̉���File�p�̊i�[�ꏊ�����
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
//	DWORD size = sizeof(str);	// ���ꂾ�ƌ����_�̕�����̒�����Ԃ��Ă��܂��BNULL����0�BERROR_MORE_DATA �ɂȂ�B
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
	if (Status == ERROR_SUCCESS) {	// ���������擾
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


// ���W�X�g���������ǂݍ��� ---> ���Window�̓��C�����E���O�ɏ������̂Ŕ��肪����E�E�E�B
BOOL LoadRegCgfInfo(void)	
{
	HKEY    hKeyNames;
	DWORD   Status;
	TCHAR   SubNamesKey[TMP_BUF_LEN] ;

	sprintf(SubNamesKey,"%s\\%s",CgfNamesKey,FileListName);	// Yss�̉���File�p�̊i�[�ꏊ��
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

		if ( UseDefaultScreenSize ) fInfoWindow = 1;	// ��Ƀf�o�O��ʂ͕\��
		RegCloseKey (hKeyNames);
		return (TRUE);
	}

	// open registry failed, use input from startup info or Max as default
	PRT("���W�X�g���̃I�[�v��(�ׂ������)�Ɏ��s�I\n");
	return (FALSE) ;
}

// ���W�X�g������CPU�̏���ǂݍ���
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
		if (Status != ERROR_SUCCESS) PRT("CPU�擾���s\n");
	}
	PRT("%s\n",sYourCPU);
}



// ������z��̌�⃊�X�g���X�V�B��ԍŋߑI�񂾂��̂�擪�ցB
void UpdateStrList(string sAdd, int *pListNum, const int nListMax, char sList[][TMP_BUF_LEN] )
{
	int i,j,nLen;
	char sTmp[TMP_BUF_LEN];

	nLen = strlen(sAdd);
	if ( nLen >= TMP_BUF_LEN ) return;
	for (i=0;i<(*pListNum);i++) {
		if ( strcmp(sList[i], sAdd) == 0 ) break;	// ���Ƀ��X�g�ɂ���t�@�C�����J����
	}
	if ( i == (*pListNum) ) {	// �V�K�ɒǉ��B�Â����̂��폜�B
		for (i=nListMax-1;i>0;i--) strcpy(sList[i],sList[i-1]);	// �X�N���[��
		strcpy(sList[0],sAdd);
		if ( (*pListNum) < nListMax ) (*pListNum)++;
	} else {					// ���ɊJ�����t�@�C����擪�ɁB
		strcpy(sTmp,    sList[i]);
		for (j=i;j>0;j--) strcpy(sList[j],sList[j-1]);	// �X�N���[��
		strcpy(sList[0],sTmp    );
	}
}

// �t�@�C�����X�g���X�V�B��ԍŋߊJ�������̂�擪�ցB
void UpdateRecentFile(string sAdd)
{
	UpdateStrList(sAdd, &nRecentFile, RECENT_FILE_MAX, sRecentFile );
}

// ���j���[��ύX����
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

// �t�@�C�����X�g����1�����폜�B�t�@�C�����������̂ŁB
void DeleteRecentFile(int n)
{
	int i;

	// �V�K�ɒǉ��B�Â����̂��폜�B
	for (i=n;i<nRecentFile-1;i++) strcpy(sRecentFile[i],sRecentFile[i+1]);	// �X�N���[��
	nRecentFile--;
	if ( nRecentFile < 0 ) { PRT("DeleteRecent Err!\n"); debug(); }
}



HMODULE hLib = NULL;	// �A�}�]���v�l�p��DLL�̃n���h��

#define CGFTHINK_DLL "cgfthink.dll"

// DLL������������B���������ꍇ��0���B�G���[�̏ꍇ�͂���ȊO��Ԃ�
int InitCgfGuiDLL(void)
{
	hLib = LoadLibrary(CGFTHINK_DLL);
	if ( hLib == NULL ) return 1;

	// �g���֐��̃A�h���X���擾����B
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

// DLL���������B
void FreeCgfGuiDLL(void)
{
	if ( hLib == NULL ) return;
	cgfgui_thinking_close();
	FreeLibrary(hLib);
}



#define BOARD_SIZE_MAX ((19+2)*256)		// 19�H�Ղ��ő�T�C�Y�Ƃ���

// ����ʒu��Ԃ�
int ThinkingDLL(int endgame_type)
{
	// DLL�ɓn���ϐ�
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

	// ���݂̊������Z�b�g����B
	for (i=0;i<BOARD_SIZE_MAX;i++) {
		dll_init_board[i]    = 3;	// �g�Ŗ��߂�
		dll_endgame_board[i] = 3;
	}
	for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
		z = (y+1) * 256 + (x+1);
		dll_init_board[z]    = ban_start[z];
		dll_endgame_board[z] = 0;
	}
	for (i=0;i<tesuu;i++) {
		dll_kifu[i][0] = kifu[i+1][0];	// CgfGui������kifu[0]�͖��g�p�Ȃ̂ł��炷�B
		dll_kifu[i][1] = kifu[i+1][1];
		dll_kifu[i][2] = kifu[i+1][2];
	}
	dll_tesuu        = tesuu;
	dll_black_turn   = (GetTurnCol() == 1);
	dll_board_size   = ban_size;
	dll_komi         = komi;
	dll_endgame_type = endgame_type;

	// DLL�̊֐����Ăяo���B
	ct1 = clock();
	thinking_utikiri_flag = 0;	// ���f�{�^���������ꂽ�ꍇ��1�ɁB
	fPassWindows = 1;			// DLL���Ăяo�����͒��f�{�^���ȊO���g�p�s�\��
	MenuEnableID(IDM_DLL_BREAK, 1);
	ret_z = cgfgui_thinking(dll_init_board, dll_kifu, dll_tesuu, dll_black_turn, dll_board_size, dll_komi, dll_endgame_type, dll_endgame_board);
	fPassWindows = 0;
	MenuEnableID(IDM_DLL_BREAK, 0);
	PRT("�ʒu=(%2d,%2d),����=%2.1f(�b),�萔=%d,�I��=%d\n",ret_z&0xff,ret_z>>8,GetSpendTime(ct1),dll_tesuu,endgame_type);
	PRT("----------------------------------------------\n");

	// �I�ǁA�}�`�A���l�\���p�̔Ֆʂɒl���Z�b�g
	if ( endgame_type != ENDGAME_MOVE ) {
		for (i=0;i<BOARD_SIZE_MAX;i++) endgame_board[i] = dll_endgame_board[i];
	}

	return ret_z;
}

/*
void TestDllThink(void)
{
	static int fFirst = 1;	// �ŏ���1�x����DLL��������
	if ( fFirst ) {	
		if ( InitCgfGuiDLL() == 0 ) return;	// DLL������������B
		cgfgui_thinking_init(&thinking_utikiri_flag);
		fFirst = 0;
	}
	ThinkingTestDLL();
//	FreeCgfGuiDLL();	// DLL���������B
}
*/

