using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P450____PlayDialog.L250_WindowMessage
{
    public abstract class Wm_InitDialog
    {
        public static void Perform()
        {
			SetFocus(hDlg);
//			PRT("lParam=%d\n",lParam);
			if ( lParam == IDYES ) {	// 初期化して対局
				fInitBoard = 1;
			} else {
				fInitBoard = 0;
				EnableWindow(GetDlgItem(hDlg, IDM_HANDICAP),   FALSE);
				EnableWindow(GetDlgItem(hDlg, IDC_BOARD_SIZE), FALSE);
			}

			for (n=0;n<2;n++) {
				int type_col[2] = { IDC_BLACK_TYPE, IDC_WHITE_TYPE };
				HWND hWnd = GetDlgItem(hDlg, type_col[n]);
				SendMessage(hWnd, CB_RESETCONTENT, (WPARAM)0, (Int64)0);
				for (i=0;i<nTypeMax;i++) SendMessage(hWnd, CB_INSERTSTRING, i, (Int64)sType[i]);
				SendMessage(hWnd, CB_SETCURSEL, turn_player[n], 0L);
			}

			// 最低でも1つの名前を登録する
			UpdateStrList(sPlayerName[1], &nNameList,NAME_LIST_MAX, sNameList );
			UpdateStrList(sPlayerName[0], &nNameList,NAME_LIST_MAX, sNameList );
			for (n=0;n<2;n++) {
				int type_col[2] = { IDC_BLACK_NAME, IDC_WHITE_NAME };
				HWND hWnd = GetDlgItem(hDlg, type_col[n]);
				SendMessage(hWnd, CB_RESETCONTENT, (WPARAM)0, (Int64)0);
				for (i=0;i<nNameList;i++) SendMessage(hWnd, CB_INSERTSTRING, i, (Int64)sNameList[i]);
				SendMessage(hWnd, CB_SETCURSEL, n, 0L);
			}
			SendMessage(hDlg, WM_COMMAND, MAKELONG(IDC_BLACK_TYPE,CBN_SELCHANGE), 0L);	// NNGS,SGMPの表示をdisenableにする
//			SetDlgItemText(hDlg, IDC_BLACK_NAME, sPlayerName[0] );
//			SetDlgItemText(hDlg, IDC_WHITE_NAME, sPlayerName[1] );

			{
				HWND hWnd = GetDlgItem(hDlg, IDM_COM_PORT);
				SendMessage(hWnd, CB_RESETCONTENT, (WPARAM)0, (Int64)0);
				for (i=0;i<8;i++) {
					sprintf(sCom,"COM%d",i+1);
					SendMessage(hWnd, CB_INSERTSTRING, i, (Int64)sCom);
				}
				SendMessage(hWnd, CB_SETCURSEL, 0, 0L);
			}
			
			// 置石
			{
				HWND hWnd = GetDlgItem(hDlg, IDM_HANDICAP);
				for (i=0;i<handi_max;i++) SendMessage(hWnd, CB_INSERTSTRING, i, (Int64)handi_str[i]);
				SendMessage(hWnd, CB_SETCURSEL, 0, 0L);
//				if ( ban_size != BAN_19 ) EnableWindow(hWnd,FALSE);
				n = nHandicap;
				if ( n < 0 || n > handi_max ) n = 0;
				if ( n > 1 ) SendMessage(hWnd, CB_SETCURSEL, n-1, 0L);
			}

			// コミ
			{
				HWND hWnd = GetDlgItem(hDlg, IDM_KOMI);
				int n = 0;
				for (i=0;i<20;i++) {
					double k = i/2.0;
					sprintf(sCom,"%.1f",k);
					if ( komi == k ) n = i;
//					if ( k >= 2 ) sprintf(sCom,"%d%s",i-1,sKomi[2]);
//					else          sprintf(sCom,"%s",      sKomi[i]);
					SendMessage(hWnd, CB_INSERTSTRING, i, (Int64)sCom);
				}
				SendMessage(hWnd, CB_SETCURSEL, n, 0L);
			}

			// 盤のサイズ
			{
				HWND hWnd = GetDlgItem(hDlg, IDC_BOARD_SIZE);
				for (i=0;i<3;i++) {
					sprintf(sCom,"%s",sSize[i]);
					SendMessage(hWnd, CB_INSERTSTRING, i, (Int64)sCom);
				}
				n = 0;
				if ( ban_size == 13 ) n = 1;
				if ( ban_size == 19 ) n = 2;
				SendMessage(hWnd, CB_SETCURSEL, n, 0L);
			}

			if ( nNngsIP == 0 ) {	// 未登録なら標準を追加。最後が最新。
				UpdateStrList("192.168.0.1",         &nNngsIP,NNGS_IP_MAX, sNngsIP );
				UpdateStrList("nngs.computer-go.jp", &nNngsIP,NNGS_IP_MAX, sNngsIP );	// CGFのnngsサーバ
			}
			{
				HWND hWnd = GetDlgItem(hDlg, IDM_NNGS_SERVER);
				for (i=0;i<nNngsIP;i++) SendMessage(hWnd, CB_INSERTSTRING, i, (Int64)sNngsIP[i]);
				SendMessage(hWnd, CB_SETCURSEL, 0, 0L);

				// テスト用のプレイヤー名をセット。
				SetDlgItemText(hDlg, IDM_NNGS_NAME1,nngs_player_name[0]);
				SetDlgItemText(hDlg, IDM_NNGS_NAME2,nngs_player_name[1]);

				SetDlgItemInt(hDlg,  IDM_NNGS_MINUTES,nngs_minutes,FALSE);
			}
        }
    }
}
