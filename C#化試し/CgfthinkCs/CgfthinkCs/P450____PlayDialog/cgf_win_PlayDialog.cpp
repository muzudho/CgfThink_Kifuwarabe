using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P450____PlayDialog
{
    public abstract class cgf_win_PlayDialog
    {

		bool NEAR OpenConnection()
		{            
			DCB dcb;	// 通信情報構造体
			COMMTIMEOUTS CommTimeOuts;	// 通信タイムアウトの構造体
			char szCommNumber[6];

			BitH = 0;	// 自分のシーケンスビット
			BitY = 0;	// 相手のシーケンスビット

			BitH = BitY = BitHH = BitYY = 0;	// 全てのシーケンスビットを初期化

			// 通信ポートのオープン
			sprintf( szCommNumber, "COM%d",nCommNumber);
			PRT("%sで通信を試みます。\n",szCommNumber);
			hCommDev = CreateFile( szCommNumber, GENERIC_READ | GENERIC_WRITE,
								 0,                    // exclusive access
								 NULL,                 // no security attrs
								 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
			if ( hCommDev == INVALID_HANDLE_VALUE ) return(FALSE);	// 通信ポートオープン失敗

			// 通信タイムアウトの設定
			CommTimeOuts.ReadIntervalTimeout         = MAXDWORD;
			CommTimeOuts.ReadTotalTimeoutMultiplier  =    0;
			CommTimeOuts.ReadTotalTimeoutConstant    = 1000;
			CommTimeOuts.WriteTotalTimeoutMultiplier =    0;
			CommTimeOuts.WriteTotalTimeoutConstant   = 1000;
			if ( !SetCommTimeouts( hCommDev, &CommTimeOuts ) ) {
				CloseHandle(hCommDev);
				return (FALSE); // SetCommTimeouts error
			}

			GetCommState( hCommDev, &dcb );	// 通信情報の取得
			dcb.BaudRate = 2400;			// 2400ボーに設定
			dcb.ByteSize = 8;				// 8データビット
			dcb.Parity = NOPARITY;			// パリティなし
			dcb.StopBits = ONESTOPBIT;		// 1ストップビット
			SetCommState( hCommDev, &dcb );	// 通信情報の設定
			return ( TRUE ) ;
		}

		///
		/// QUERY 質問を送る。
		///
		public static int SendQuery(int qqq)
		{
			BitH = ( BitH == 0 );	// 論理演算	0 -> 1, 1 -> 0
			StartByte = (BitYY << 1) + BitH;

			CommHI = 0xb0;
			CommLO = 0x80 + qqq;

			CheckSum = StartByte + ( CommHI & 0x7f ) + ( CommLO & 0x7f );
			CheckSum |= 0x80;

			lpSend[0] = StartByte;
			lpSend[1] = CheckSum;
			lpSend[2] = CommHI;
			lpSend[3] = CommLO;
			Delay(500);						/* 一定時間(500ms)待つ */
			PRT("Send Data    -> %02x:%02x:%02x:%02x :",StartByte,CheckSum,CommHI,CommLO);
			PRT("Send Query = %d \n",qqq);
			Send4Byte(lpSend);
		//	WriteFile( hCommDev,(void *)lpSend, 4, &RetByte, NULL );
		//	PRT("Send Query End\n");

			// 受信文字列、ANSWER を待つ
			if ( DialogBox(ghInstance, "COMM_DIALOG", ghWindow, (DLGPROC)CommProc) == FALSE ) {
				PRT("QUERYの中断処理\n");
				return(FALSE);
			}
			if ( RetComm != 4 ) {
				PRT("QUERY を送ったのに ANSWER が返ってこない・・・。\n");
				return(FALSE);
			}
		//	ReturnOK();		// 取りあえずANSWERにはOKを返しましょう
			return( CommLO & 0x7f );
		}


		
		// NewGameを送る
		public static int SendNewGame(void)
		{
			BitH = BitY = BitHH = BitYY = 0;	// 念のためここでもシーケンスビットを初期化

			BitH = ( BitH == 0 );	// 論理演算	0 -> 1, 1 -> 0
			StartByte = (BitYY << 1) + BitH;

			CommHI = 0xa0;
			CommLO = 0x80;

			CheckSum = StartByte + ( CommHI & 0x7f ) + ( CommLO & 0x7f );
			CheckSum |= 0x80;

			lpSend[0] = StartByte;
			lpSend[1] = CheckSum;
			lpSend[2] = CommHI;
			lpSend[3] = CommLO;
			PRT("Send Data    -> %02x:%02x:%02x:%02x :",StartByte,CheckSum,CommHI,CommLO);
			PRT("NewGame を送信\n");
			Send4Byte(lpSend);
		//	WriteFile( hCommDev,(void *)lpSend, 4, &RetByte, NULL );

			// 受信文字列、OK を待つ
			if ( DialogBox(ghInstance, "COMM_DIALOG", ghWindow, (DLGPROC)CommProc) == FALSE ) {
				PRT("NewGameの中断処理\n");
				return(FALSE);
			}
			if ( RetComm != 2 ) {
				PRT("NEWGAME を送ったのに OK が帰ってこないよ。\n");
				return(FALSE);
			}
			return( TRUE );
		}
		
		bool NEAR CloseConnection()
		{
			if ( fUseNngs ) { cgf_wsk_nngs.close();	return TRUE; }
			return ( CloseHandle( hCommDev ) );
		}


		/// init_comm で登録される。
		///
		/// nngsからの情報を待つ
		public static bool CALLBACK Dlgt_NngsWait(
			HWND hDlg,
			UINT msg,
			WPARAM wParam,
			Int64 lParam
			)
		{
			switch(msg) {
				case WM_INITDIALOG:
					hNngsWaitDlg = hDlg;
					if ( lParam == 0 ) {	// 最初は接続してログインする
						if ( cgf_wsk_nngs.connectNngs() == 0 ) EndDialog(hDlg,FALSE);	// ログイン失敗
					}
					return TRUE;

				case WM_COMMAND:
					switch ( LOWORD(wParam) ) {
						case IDCANCEL:
							EndDialog(hDlg,FALSE);			// 中断で終了
							break;
						case IDM_NNGS_MES:
							EndDialog(hDlg,TRUE);			// 何らかのメッセージが来たので終了
							break;
					}
					return TRUE;

				case WM_DESTROY:
					hNngsWaitDlg = NULL;
					break;
			}
			return FALSE;
		}

		// Wm_Create.Perform() から呼び出される。
		//
        // 通信対戦の初期化処理。失敗した場合は0を返す
        public static int init_comm(HWND hWnd)
        {
	        if ( fUseNngs )
			{
		        // 最初にWinSockを初期化してサーバに接続まで。
		        if ( DialogBoxParam(ghInstance, "NNGS_WAIT", hWnd, (DLGPROC)Dlgt_NngsWait, 0) == FALSE ) {
			        PRT("nngs接続失敗\n");
			        return 0;
		        }
		        PRT("Loginまで正常終了\n");
		        return 1;
	        }

	        if ( cgf_win_PlayDialog.OpenConnection() == FALSE ) {
		        PRT("通信ポートオープンの失敗！\n");
		        return 0;
	        }

	        PRT("棋譜自動セーブ=%d\n",fAutoSave);

	        if ( turn_player[1]==PLAYER_SGMP ) {	// こちらが先手（黒）の時は NEWGAME を送る
		        if ( cgf_win_PlayDialog.SendNewGame() == FALSE ) {	// NewGame を送る
			        PRT("メインループに戻ります\n");
			        cgf_win_PlayDialog.CloseConnection();
			        return 0;
		        }
		        // 先手で打つ
		        return 1;

	        } else {		// DLLが後手番の時
		        if ( DialogBox(ghInstance, "COMM_DIALOG", hWnd, (DLGPROC)CommProc) == FALSE ) {
			        PRT("メインループに戻ります\n");
			        cgf_win_PlayDialog.CloseConnection();
			        return 0;
		        }
		        if ( RetComm != 3 ) {
			        PRT("NewGame が来ないね・・・RetComm=%d\n",RetComm);
			        cgf_win_PlayDialog.CloseConnection();
			        return 0;
		        }
        //		PRT("NewGame が来た来た！\n");

		        int k = cgf_win_PlayDialog.SendQuery(11);	// 石の色を質問
		        if ( k == 1 ) {
			        PRT("先手なのに白なの？\n");
			        cgf_win_PlayDialog.CloseConnection();
			        return 0;
		        }
		        k = cgf_win_PlayDialog.SendQuery(8);	// 互い先かどうかを質問
		        if ( k >= 2 ) {
			        PRT("置き碁はしませんよ k=%d だけど続行\n",k);
        //			cgf_win_PlayDialog.CloseConnection();
        //			return 0;
		        }
		        ReturnOK();	// 条件を満たしたのでOKを送り、指し手を待つ
		        return 1;
	        }
        //	return 0;

        }
    }
}
