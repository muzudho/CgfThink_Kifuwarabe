/* cgf_wsk.cpp */
// WinSockを使った通信対戦用。wsock32.lib をリンクする必要あり
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
//#include <winsock.h>

#include "cgf.h"
#include "cgf_wsk.h"




int nngs_minutes = 40;		// 40分切れ負け




//char sNNGS_IP[TMP_BUF_LEN] = "nngs.computer-go.jp";	// nngs.computer-go.jp 9696	
//char sNNGS_IP[TMP_BUF_LEN] = "192.168.11.11";

int fUseNngs = 0;	// nngs経由で非同期で対戦する場合
extern HWND ghWindow;					/* WINDOWSのハンドル  */



HWND hNngsWaitDlg = NULL;
#define IDM_NNGS_MES 1000


