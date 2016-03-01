using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P493____WinProc.L400_WindowsMessage
{
    public abstract class Wm_Create
    {
        public static void Perform()
        {
			switch ( LOWORD(wParam) ) {
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
			
				case IDM_TEST:
					gct1 = clock();	// 探索開始時間。1000分の１秒単位で測定。
//					{ void init_gnugo_pipe(int); init_gnugo_pipe(1); }	// GnuGoとGTP経由で連続対戦
					PRT("%2.1f 秒\n", GetSpendTime(gct1));
					break;

				case IDM_THINKING:			// 思考ルーチン呼び出し
					z = ThinkingDLL(ENDGAME_MOVE);	// 着手位置を返す
					ret = SendUpdateSetKifu(hWnd, z, GUI_DLL_MOVE);		// 着手を通信で送ったり、次の手番に移ったり
					break;
				
				case IDM_PASS:
					if ( fNowPlaying == 0 ) return 0;	 
					ret = SendUpdateSetKifu(hWnd, 0, GUI_USER_PASS);	// 着手を通信で送ったり、次の手番に移ったり
					break;

				case IDM_RESIGN:
					if ( fNowPlaying == 0 ) return 0;	 
					ret = SendUpdateSetKifu(hWnd, -1, GUI_USER_PASS);
					break;

				case IDM_GNUGO_PLAY:	// GnuGo(GTP Engine)に手を打たせる
					z = PlayGnugo(ENDGAME_MOVE);	// 着手位置を返す
					ret = SendUpdateSetKifu(hWnd, z, GUI_GTP_MOVE);		// 着手を通信で送ったり、次の手番に移ったり
					break;

				case IDM_INIT_COMM:		// 通信対戦の初期化処理
					if ( init_comm(hWnd) ) PostMessage(hWnd,WM_COMMAND,IDM_NEXT_TURN,0L);
					else StopPlayingMode();
					break;

				case IDM_NEXT_TURN:		// 次の手はどうすべきかを処理
					// COM同士の対戦中に停止するため(GnuGo同士でも、COM－GnuGoも)
					for (i=0;i<2;i++) {
						k = turn_player[i];
						if ( k == PLAYER_DLL || k == PLAYER_GNUGO ) continue;
						else break;
					}
					if ( i==2 ) {
						for (i=0;i<100;i++) PassWindowsSystem();
						if ( thinking_utikiri_flag ) {
							thinking_utikiri_flag = 0;
							StopPlayingMode();
							break;
						}
					}

					{
						gct1 = clock();	// 思考開始時間
						// 連続パスなら終局処理を呼んで中断
//						if ( kifu[tesuu-1][0]==0 && UseDefaultScreenSize == 0 ) SendMessage(hWnd,WM_COMMAND,IDM_DEADSTONE,0L);	// 終局処理を呼ぶ
						int next = turn_player[GetTurnCol()-1];
//						if ( next == PLAYER_HUMAN ) ;	// 何もしない
						if ( next == PLAYER_DLL   ) PostMessage(hWnd,WM_COMMAND,IDM_THINKING,0L);
						if ( next == PLAYER_NNGS  ) PostMessage(hWnd,WM_COMMAND,IDM_NNGS_WAIT,0L);
						if ( next == PLAYER_SGMP  ) PostMessage(hWnd,WM_COMMAND,IDM_SGMP_WAIT,0L);
						if ( next == PLAYER_GNUGO ) PostMessage(hWnd,WM_COMMAND,IDM_GNUGO_PLAY,0L);
					}
					break;

				case IDM_NNGS_WAIT:		// 通信相手の手を待つ
				case IDM_SGMP_WAIT:
				{
					int fBreak = 1;
					for (;;) {
						gct1 = clock();
						if ( fUseNngs ) {
							if ( DialogBoxParam(ghInstance, "NNGS_WAIT", hWnd, (DLGPROC)Dlgt_NngsWait, 1) == FALSE ) {
								PRT("メインループに戻ります\n");
								break;
							}
							if ( RetZ < 0 ) break;	// nngsの場合は 0...PASS, 1以上...場所, マイナス...エラーを示す
							RetComm = 1;
						} else if ( DialogBox(ghInstance, "COMM_DIALOG", hWnd, (DLGPROC)CommProc) == FALSE ) {
							PRT("メインループに戻ります\n");
							break;
						}
						if ( RetComm == 2 ) continue;	// OKを受信して抜けた時はもう一度待つ
						if ( RetComm != 1 ) {	// move 以外は一度待つ
							if ( RetComm == 3 ) {
								PRT("NewGame がまた来ている！でもOKを返してあげよう\n");
								ReturnOK();	// 条件を満たしたのでOKを送り、手を待つ
							}
							continue;
						}
						PRT("RetZ = %x\n",RetZ);
						if ( IsGnugoBoard()    ) UpdateGnugoBoard(RetZ);

						int ret = set_kifu_time(RetZ,GetTurnCol(),(int)GetSpendTime(gct1));
						if ( ret ) {
							GameStopMessage(hWnd,ret,RetZ);
							break;
						}
						// 次の手番の処理に移る
						PostMessage(hWnd,WM_COMMAND,IDM_NEXT_TURN,0L);
						fBreak = 0;
						break;
					}
					if ( fBreak ) StopPlayingMode();
					break;
				}

				case IDM_PLAY_START:
					{
						int fRet = IDYES;
						if ( all_tesuu != 0 ) {
#ifdef CGF_E
							char sTmp[] = "Initialize Board?";
#else
							char sTmp[] = "局面を初期化しますか？";
#endif
							fRet = MessageBox(hWnd,sTmp,"Start Game",MB_YESNOCANCEL | MB_DEFBUTTON2);
							if ( fRet == IDCANCEL ) break;
						}
						DialogBoxParam(ghInstance, "START_DIALOG", hWnd, (DLGPROC)Dlgt_PlayDialogStart, fRet);
					}
					PaintUpdate();		// 先手後手の名前を更新するため全画面書き換え。
					break;

				case IDM_GNUGO_SETTING:
					DialogBox(ghInstance, "GTP_SETTING", hWnd, (DLGPROC)GnuGoSettingProc);
					break;

				case IDM_CLIPBOARD:
					hyouji_clipboard();
					break;

				case IDM_FROM_CLIPBOARD:
					clipboard_to_board();	// クリップボードから盤面を読み込む
					break;

				case IDM_PRT_TO_CLIP:
					PRT_to_clipboard();		// PRTの内容をクリップボードにコピー
					break;

				case IDM_SGF_CLIPOUT:
					SGF_clipout();			// SGFで棋譜をクリップボードへ
					break;

				case IDM_DEADSTONE:			// ツールバーのボタンが押された場合
				case IDM_VIEW_LIFEDEATH:	// 終局処理、死に石を表示
				case IDM_VIEW_FIGURE:		// 図形を表示
				case IDM_VIEW_NUMBER:		// 数値を表示
					{
						int id = LOWORD(wParam);
						int ids[3] = {IDM_VIEW_LIFEDEATH, IDM_VIEW_FIGURE, IDM_VIEW_NUMBER };  

						hMenu = GetMenu(hWnd);
						for (i=0;i<3;i++) CheckMenuItem(hMenu, ids[i], MF_UNCHECKED);	// メニューを全部OFF
						k = 0;
						if ( id==IDM_DEADSTONE ) {
							if ( endgame_board_type==0 ) id = ids[0];
							else k = endgame_board_type;
						}
						if ( id==IDM_VIEW_LIFEDEATH ) k = ENDGAME_STATUS;
						if ( id==IDM_VIEW_FIGURE    ) k = ENDGAME_DRAW_FIGURE;
						if ( id==IDM_VIEW_NUMBER    ) k = ENDGAME_DRAW_NUMBER;
						if ( k == endgame_board_type ) {	// 同じものが再度選択
							endgame_board_type = 0;
							UpdateLifeDethButton(0);
						} else {
							endgame_board_type = k;
							UpdateLifeDethButton(1);
							CheckMenuItem(hMenu, id, MF_CHECKED);
							if ( endgame_board_type == ENDGAME_STATUS && fGnugoLifeDeathView ) {
								FinalStatusGTP(endgame_board);
							} else {
								ThinkingDLL(endgame_board_type);
							}
						}
						PaintUpdate();
					}
					break;

				case IDM_PRINT:
					hMenu = GetMenu(hWnd);
					if ( hPrint==NULL || IsWindow(hPrint)==FALSE ) {
						CheckMenuItem(hMenu, IDM_PRINT, MF_CHECKED);
						OpenPrintWindow(hWnd);
					} else {
						DestroyWindow(hPrint);
						CheckMenuItem(hMenu, IDM_PRINT, MF_UNCHECKED);
					}
					fInfoWindow = (hPrint != NULL);
					break;

				case IDM_SOUND:
					SoundDrop = (SoundDrop == 0);	// 着手音フラグ
					CheckMenuItem(GetMenu(hWnd), IDM_SOUND, SoundDrop ? MF_CHECKED : MF_UNCHECKED);
					break;

				case ID_BAN_1:
				case ID_BAN_2:
				case ID_BAN_3:
				case ID_BAN_4:
				case ID_BAN_5:
				case ID_BAN_6:
				case ID_BAN_7:
				case ID_BAN_8:
					ID_NowBanColor = LOWORD(wParam);
					MenuColorCheck(hWnd);	// 盤面色のメニューにチェックを
					PaintUpdate();
					break;

				case IDM_BREAK:	// 対局中断
					fNowPlaying = 0;
					MenuEnablePlayMode(0);	// 対局中は棋譜移動メニューなどを禁止する
					break;

				case IDM_CHINESE_RULE:
					fChineseRule = (fChineseRule == 0);
					CheckMenuItem(GetMenu(hWnd), IDM_CHINESE_RULE, fChineseRule ? MF_CHECKED : MF_UNCHECKED);
					PaintUpdate();
					break;

				case IDM_ATVIEW:
					fATView = (fATView == 0);
					CheckMenuItem(GetMenu(hWnd), IDM_ATVIEW, fATView ? MF_CHECKED : MF_UNCHECKED);
					PaintUpdate();
					break;

				case IDM_BAN_CLEAR:
					for (i=0;i<BANMAX;i++) {
						if ( ban[i] != 3 ) ban[i]=0;
						ban_start[i] = ban[i];
					}
					reconstruct_ban();
					PaintUpdate();
					break;

				case IDM_SET_BOARD:
					all_tesuu = 0;
					reconstruct_ban();
					PaintUpdate();
					break;

				case IDM_BACK1:
					move_tesuu(tesuu - 1);		// 任意の手数の局面に盤面を移動させる
					PaintUpdate();
					break;

				case IDM_BACK10:
					move_tesuu(tesuu - 10);		// 任意の手数の局面に盤面を移動させる
					PaintUpdate();
					break;

				case IDM_BACKFIRST:
					move_tesuu( 0 );			// 任意の手数の局面に盤面を移動させる
					PaintUpdate();
					break;

				case IDM_FORTH1:
					move_tesuu(tesuu + 1);		// 任意の手数の局面に盤面を移動させる
					PaintUpdate();
					break;

				case IDM_FORTH10:
					move_tesuu(tesuu + 10);		// 任意の手数の局面に盤面を移動させる
					PaintUpdate();
					break;

				case IDM_FORTHEND:
					move_tesuu(all_tesuu);		// 任意の手数の局面に盤面を移動させる
					PaintUpdate();
					break;

				case IDM_KIFU_SAVE:
					KifuSave();
					break;

				case IDM_KIFU_OPEN:
					KifuOpen();
					break;

				case IDM_FILE_WRITE:
					fFileWrite = (fFileWrite == 0);
					hMenu = GetMenu(hWnd);
					CheckMenuItem(hMenu, IDM_FILE_WRITE, (fFileWrite ? MF_CHECKED : MF_UNCHECKED) );
					break;

				case IDM_ABOUT:
					DialogBox( ghInstance, "ABOUT_DIALOG", hWnd, (DLGPROC)AboutDlgProc);
					break;
			}

			// RecentFileが選択された場合
			if ( IDM_RECENT_FILE <= wParam && wParam < IDM_RECENT_FILE+RECENT_FILE_MAX ) {
				int n =  wParam - IDM_RECENT_FILE;
				strcpy(cDragFile, sRecentFile[n]);
//				PRT("%s\n",InputDir);
				if ( KifuOpen() == FALSE ) {
					DeleteRecentFile(n);	// ファイルリストから削除。ファイルが消えたので。
					UpdateRecentFileMenu(hWnd);
				}
				cDragFile[0] = 0;
			}
        }
    }
}
