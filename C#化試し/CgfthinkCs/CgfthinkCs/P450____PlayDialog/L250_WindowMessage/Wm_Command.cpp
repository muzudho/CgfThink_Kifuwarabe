using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P450____PlayDialog.L250_WindowMessage
{
    public abstract class Wm_InitDialog
    {
		// nngsに接続するテスト
		string[] nngs_player_name = {//[2][TMP_BUF_LEN]
		//	"aya",
		//	"katsunari",
			"test1",
			"test2",
		//	"brown",
		};

        public static void Perform()
        {
			if ( wParam == IDCANCEL ) EndDialog(hDlg,FALSE);		// キャンセル
			if ( wParam == IDOK ) {
				// 入力をチェック
				turn_player[0] = SendDlgItemMessage(hDlg,IDC_BLACK_TYPE, CB_GETCURSEL, 0, 0L);
				turn_player[1] = SendDlgItemMessage(hDlg,IDC_WHITE_TYPE, CB_GETCURSEL, 0, 0L);
				PRT("turn[0]=%d,%d\n",turn_player[0],turn_player[1]);
				fUseNngs = 0;
				int sgmp = 0;
				for (i=0;i<2;i++) {
					n = turn_player[i];
					if ( n == PLAYER_NNGS || n == PLAYER_SGMP ) sgmp |= (i+1);
					if ( n == PLAYER_NNGS ) fUseNngs = 1;
				}
				if ( sgmp == 3 ) {
					MessageBox(hDlg,err_str[0],"Player Select Error",MB_OK);
					break;
				}
				if ( sgmp ) dll_col = UN_COL(sgmp);

//				gct1 = clock();			// user の思考時間計測用
				
				for (n=0;n<2;n++) {
					int type_col[2] = { IDC_BLACK_NAME, IDC_WHITE_NAME };
					GetDlgItemText(hDlg, type_col[n], sPlayerName[n], TMP_BUF_LEN-1);
					if ( strlen(sPlayerName[n]) > 0 ) {	// 文字列配列の候補リストを更新。
						UpdateStrList(sPlayerName[n], &nNameList,NAME_LIST_MAX, sNameList );
					}
				}
			
				// 新規対局では盤面を初期化
				if ( fInitBoard ) {
					// サイズ
					n = SendDlgItemMessage(hDlg, IDC_BOARD_SIZE, CB_GETCURSEL, 0, 0L);
					ban_size = BAN_9;
					if ( n == 1 ) ban_size = BAN_13;
					if ( n == 2 ) ban_size = BAN_19;
					init_ban();
					SendMessage(ghWindow,WM_COMMAND,IDM_BAN_CLEAR,0L);	// 盤面初期化

					// 置石
					n = SendMessage(GetDlgItem(hDlg, IDM_HANDICAP), CB_GETCURSEL, 0, 0L);
					if ( n == CB_ERR ) { PRT("置石取得Err\n"); n = 0; }
					if ( n ) {	// 置石がある
						nHandicap = n + 1;	// 白が先手で打つ
						SetHandicapStones();
					} else nHandicap = 0;
				}

				// コミ
				n = SendDlgItemMessage(hDlg, IDM_KOMI, CB_GETCURSEL, 0, 0L);
				komi = n/2.0;
				if ( komi < 0 ) komi = 0;
				
				// NNGS情報の取得
				{
					char sTmp[TMP_BUF_LEN];
					GetDlgItemText(hDlg, IDM_NNGS_SERVER, sTmp, TMP_BUF_LEN-1);
					if ( strlen(sTmp) > 0 ) {
						UpdateStrList(sTmp, &nNngsIP,NNGS_IP_MAX, sNngsIP );
					}
					GetDlgItemText(hDlg, IDM_NNGS_NAME1,nngs_player_name[0],TMP_BUF_LEN-1);
					GetDlgItemText(hDlg, IDM_NNGS_NAME2,nngs_player_name[1],TMP_BUF_LEN-1);
					nngs_minutes = GetDlgItemInt(hDlg, IDM_NNGS_MINUTES,NULL,FALSE);
				}

				// COMポートの取得
				nCommNumber = 1 + SendDlgItemMessage(hDlg,IDM_COM_PORT, CB_GETCURSEL, 0, 0L);
		
				// 対局中に入る
				fNowPlaying = 1;
				MenuEnablePlayMode(0);	// 対局中は棋譜移動メニューを禁止する

				//
				// GnuGoの初期化
				//
				for (i=0;i<2;i++) {
					if ( turn_player[i] == PLAYER_GNUGO ) break;
				}

				if ( i!=2 )
				{
					if ( fGnugoPipe )
					{
						close_gnugo_pipe();
					}

					if ( fGnugoPipe==0 ) {
						if ( open_gnugo_pipe() == 0 ) {
							MessageBox(hDlg,"GTP engine path error","GTP err",MB_OK);
							break;	
						}
					}

					string sName;

					if ( init_gnugo_gtp(sName) == 0 )
					{
							break;
					}

					for (i=0;i<2;i++) {
						if ( turn_player[i] == PLAYER_GNUGO ) strcpy(sPlayerName[i],sName);
					}

					// Gnugo側の盤面を設定
					int x,y,z,k;
					for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
						z = ((y+1)<<8) + x+1 ;
						k = ban[z];
						if ( k==0 ) continue;
						PutStoneGnuGo(z, (k==1));
					}
				}

				if ( sgmp ) {
					PostMessage(ghWindow,WM_COMMAND,IDM_INIT_COMM,0L);	// 通信対戦の初期化処理
				} else {
					PostMessage(ghWindow,WM_COMMAND,IDM_NEXT_TURN,0L);
				}

				EndDialog(hDlg,FALSE);			
				PRT("ban=%d,nHandi=%d,komi=%.1f,nComm=%d\n",ban_size,nHandicap,komi,nCommNumber);
			}

			switch ( LOWORD(wParam) ) {
				case IDC_BLACK_TYPE:
				case IDC_WHITE_TYPE:
					if ( HIWORD(wParam) == CBN_SELCHANGE ) {
						int kind = 0;
						int flag;

						int[] p = new int[]{
							SendDlgItemMessage(hDlg,IDC_BLACK_TYPE, CB_GETCURSEL, 0, 0L),
							SendDlgItemMessage(hDlg,IDC_WHITE_TYPE, CB_GETCURSEL, 0, 0L),
						};

						PRT("turn[0]=%d,%d\n",p[0],p[1]);

						for (i=0;i<2;i++) {
							n = p[i];
							if ( n == PLAYER_NNGS ) kind |= 1;
							if ( n == PLAYER_SGMP ) kind |= 2;
						}

						flag = FALSE;

						if ( kind & 2 )
						{
							flag = TRUE;
						}

						EnableWindow(GetDlgItem(hDlg, IDM_COM_PORT), flag);
						flag = TRUE;

						if ( kind & 1 )
						{
							flag = FALSE;
						}

						EnableWindow(GetDlgItem(hDlg, IDM_NNGS_SERVER), !flag);
						SendDlgItemMessage(hDlg,IDM_NNGS_NAME1,  EM_SETREADONLY, flag, 0L);
						SendDlgItemMessage(hDlg,IDM_NNGS_NAME2,  EM_SETREADONLY, flag, 0L);

						// 標準の名前をセット
						if ( LOWORD(wParam) == IDC_BLACK_TYPE )
						{
							SetDlgItemText(hDlg, IDC_BLACK_NAME, sType[p[0]] );
						}

						if ( LOWORD(wParam) == IDC_WHITE_TYPE )
						{
							SetDlgItemText(hDlg, IDC_WHITE_NAME, sType[p[1]] );
						}

						SendDlgItemMessage(hDlg,IDM_NNGS_MINUTES, EM_SETREADONLY, flag, 0L);
					}
					break;
/*
				case IDC_BLACK_NAME:
					PRT("%08x,%08x\n",wParam, HIWORD(wParam));
					if ( HIWORD(wParam) == CBN_SELCHANGE ) {
						GetDlgItemText(hDlg, IDC_BLACK_NAME, sTmp, TMP_BUF_LEN-1);
						SetDlgItemText(hDlg, IDM_NNGS_NAME1,sTmp);
					}
					break;

				case IDC_WHITE_NAME:
					if ( HIWORD(wParam) == CBN_SELCHANGE ) {
						GetDlgItemText(hDlg, IDC_WHITE_NAME, sTmp, TMP_BUF_LEN-1);
						SetDlgItemText(hDlg, IDM_NNGS_NAME2,sTmp);
					}
					break;
*/
			}
        }
    }
}
