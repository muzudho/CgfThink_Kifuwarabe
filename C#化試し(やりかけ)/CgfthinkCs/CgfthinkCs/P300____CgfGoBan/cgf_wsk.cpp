/* cgf_wsk.cpp */
// WinSock���g�����ʐM�ΐ�p�Bwsock32.lib �������N����K�v����
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
//#include <winsock.h>

#include "cgf.h"
#include "cgf_wsk.h"




int nngs_minutes = 40;		// 40���؂ꕉ��




//char sNNGS_IP[TMP_BUF_LEN] = "nngs.computer-go.jp";	// nngs.computer-go.jp 9696	
//char sNNGS_IP[TMP_BUF_LEN] = "192.168.11.11";

int fUseNngs = 0;	// nngs�o�R�Ŕ񓯊��őΐ킷��ꍇ
extern HWND ghWindow;					/* WINDOWS�̃n���h��  */



HWND hNngsWaitDlg = NULL;
#define IDM_NNGS_MES 1000


