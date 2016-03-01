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
		#ifdef CGF_DEV
					UseDefaultScreenSize = 1;	// 固定の画面サイズを使う(公開時OFF)
		#endif
					// 描画用Windowの生成
					hWndDraw = CreateWindowEx(WS_EX_CLIENTEDGE,
						"DrawWndClass", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
						hWnd, NULL, ghInstance, NULL);
					if ( !hWndDraw ) return FALSE;

					CreateTBar(hWnd);				// ツールバーを生成
		//		    DragAcceptFiles(hWnd, TRUE);	// ドラッグ＆ドロップされるファイルを受け付ける。

					init_ban();

					// レジストリから情報（画面サイズと位置）を読み込む
					if ( LoadMainWindowPlacement(hWnd,1) == 0 ) PRT("Registory Error (If first time, it's OK)\n");
					// レジストリから情報を読み込む
					if ( LoadRegCgfInfo() == 0 ) PRT("Registory Error... something new (If first time, it's OK)\n");
					UpdateRecentFileMenu(hWnd);	// メニューを変更する

					if ( fInfoWindow ) SendMessage(hWnd,WM_COMMAND,IDM_PRINT,0L);		// 情報表示Windowの表示
					else CheckMenuItem( GetMenu(hWnd), IDM_PRINT, MF_UNCHECKED);
					MenuColorCheck(hWnd);	// 盤面色のメニューにチェックを

					GlobalMemoryStatus(&MemStat);
					RealMem_MB = MemStat.dwTotalPhys/(1024*1024);	// 積んでいる実メモリ
					LoadRegCpuInfo();	// レジストリからCPUの情報を読み込む
					PRT("物理メモリ= %d MB, ",MemStat.dwTotalPhys/(1024*1024));
					PRT("空物理=%dMB, 割合=%d%%\n",MemStat.dwAvailPhys/(1024*1024), MemStat.dwMemoryLoad);

					if ( SoundDrop ) { SoundDrop = 0; SendMessage(hWnd,WM_COMMAND,IDM_SOUND,0L); } // 効果音をオン
					if ( fATView   ) { fATView   = 0; SendMessage(hWnd,WM_COMMAND,IDM_ATVIEW,0L); }

					if ( UseDefaultScreenSize == 0 ) {
						SendMessage(hWnd,WM_COMMAND,IDM_BAN_CLEAR,0L);	// 画面消去(後、上のレジストリ登録をONに!!!）
					}

					sprintf( sPlayerName[0],"You");
					sprintf( sPlayerName[1],sDllName);

		//			PRT("--------->%s\n",GetCommandLine());
					GetCurrentDirectory(MAX_PATH, sCgfDir);	// 現在のディレクトリを取得

					SendMessage(hWnd,WM_COMMAND,IDM_BAN_CLEAR,0L);	// 画面消去

					SetFocus(hWnd);	// 先にhPrintが書かれるとFocusがそっちに行ってしまうので。

					if ( strlen(cDragFile) ) PostMessage(hWnd,WM_COMMAND,IDM_KIFU_OPEN,0L);	// 棋譜を開く
		//			SendMessage(hWnd,WM_COMMAND,IDM_THINKING,0L); DestroyWindow(hWnd);	// Profile用のテスト

					// DLLを読み込む
					ret = InitCgfGuiDLL();
					if ( ret != 0 ) {
						char sTmp[TMP_BUF_LEN];
						if ( ret == 1 ) sprintf(sTmp,"DLL is not found.");
						else            sprintf(sTmp,"Fail GetProcAddress() ... %d",ret);
						MessageBox(NULL,sTmp,"DLL initialize Error",MB_OK); 
						PRT("not able to use DLL.\n");
						return FALSE;
					} else {
						// DLLを初期化する。
						cgfgui_thinking_init(&thinking_utikiri_flag);
					}

					SendMessage(hWnd,WM_COMMAND,IDM_BREAK,0L);	// 対局中断状態に
		/*
					PRT("fGtpClient = %d\n",fGtpClient);
					if ( fGtpClient ) {
						ShowWindow( hWnd, SW_HIDE );
						if ( hPrint && IsWindow(hPrint)==TRUE ) ShowWindow( hPrint, SW_HIDE );
						{
							extern void test_gtp_client(void);
							test_gtp_client();
						}
						DestroyWindow(hWnd);	// GTP経由の場合はすぐに終了する。
		//				PRT_to_clipboard();		// PRTの内容をクリップボードにコピー
					}
		*/
        }
    }
}
