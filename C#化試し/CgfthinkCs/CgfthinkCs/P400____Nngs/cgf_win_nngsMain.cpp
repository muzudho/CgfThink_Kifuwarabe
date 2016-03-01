using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P450____Nngs
{
    public abstract class cgf_win_nngsMain
    {

        /// cgf_win.WndProc から呼び出される？
        ///
        /// 非同期のSelectを受け取る。nngs用
        /// 旧名：OnNngsSelect
        public static void OnReceivedNngs(HWND /*hWnd*/, WPARAM wParam, Int64 lParam)
        {
	        static string buf;
	        static int buf_size = 0;

	        if ( GlobalTusin.GetAsyncError(lParam) != 0 ) {
		        PRT("ソケットイベントの通知でエラー=%d\n", GlobalTusin.GetSelectError(lParam));
        //		WSAECONNABORTED  (10053)
		        GlobalTusin.So_ck.CloseSo_cket();		// ソケットを破棄
		        GlobalTusin.So_ck = So_cket2Impl.InvalidSo_cket;
		        return;
	        }

	        if ( GlobalTusin.So_ck != (So_cket2)wParam)
			{
					return;	// 処理すべきソケットか判定
			}

	        switch (GlobalTusin.GetSelectEvent(lParam)) {
		        case SelectEventFD.CONNECT:	// 接続に成功	
			        PRT("サーバに接続\n");
			        buf_size = 0;
			        break;

		        case SelectEventFD.CLOSE:		// 接続を切られた
			        GlobalTusin.So_ck.CloseSo_cket();
			        GlobalTusin.So_ck = So_cket2Impl.InvalidSo_cket;
			        PRT("サーバから切断された\n");
			        break;

		        case SelectEventFD.WRITE:		// 送信バッファに空きができた
			        PRT("SelectEventFD.WRITE\n");
			        break;

		        case SelectEventFD.READ:		// データを受信する
        //			PRT("SelectEventFD.READ size=%d\n",buf_size);
			        int nRecv = recv( GlobalTusin.So_ck, buf + buf_size, 1, 0);
			        if ( nRecv == GlobalTusin.SO_CKET_ERROR ) {		// 多分ブロックしただけ？なので何もしない
				        PRT("Error=%d recv()\n", GlobalTusin.GetLastError());	
				        break;
			        }
			        buf_size += nRecv;
			        if ( buf_size >= TMP_BUF_LEN ) {
				        PRT("buf over! size=%d\n",buf_size);
				        buf_size = 0;
				        break;
			        }
			        if ( buf_size <= 0 ) break;
			        buf[buf_size] = 0;

			        if ( strcmp(buf,"Login: ") == 0 ) {
				        PRT("<---%s",buf);
				        buf_size = 0;
				        cgf_wsk_nngs.nngs(dll_col-1);	// loginする
				        break;
			        }

			        if ( buf[buf_size-1] == '\n' ) {	// 1行受け取った
				        buf_size = 0;
				        PRT("<---%s",buf);
				        if ( fPassWindows ) { PRT("思考中はnngsからの情報は無視\n"); break; }
				        // 構文解析ルーチンへ飛ぶ
				        int ret = cgf_wsk_nngs.Analysis(buf,dll_col-1);	// nngsの内容を解析
        //				PRT("ret=%d\n",ret);
				        if ( ret == Result_NngsConnection.LOGIN_TOTYU ) break;
				        if ( ret == Result_NngsConnection.UNKNOWN     ) break;
				        RetZ = ret;
				        if ( hNngsWaitDlg == NULL ) {
					        PRT("nngs待機ダイアログが出ていない\n");
					        break;
				        }
				        PostMessage(hNngsWaitDlg,WM_COMMAND,IDM_NNGS_MES,0);	// 通信待ちダイアログを終了させる。
			        }
			        break;
	        }
        }
    }
}
