using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P400____CgfGoBan
{
    public abstract class cgf_wsk_nngs
    {
		// データ送信
		public void static Send(string message)
		{
			int nByteSend = GlobalTusin.Send( GlobalTusin.So_ck, message);
			if ( nByteSend == GlobalTusin.SO_CKET_ERROR ) {
				PRT("センド() Err=%d, %s\n", GlobalTusin.GetLastError(), message);
				return;
			}
			PRT("--->%s",message);
		}

		// cgf_wsk.Dlgt_NngsWait から呼び出される。
		//
		// サーバと接続する。失敗すれば0を返す
		public static int connectNngs()
		{
			SOCKADDR_IN Addr;
			unsigned short nPort = 9696;	// サーバのポート番号
			unsigned long addrIP;			// IPが変換される数値
			PHOSTENT host = NULL;			// ホスト情報
			int i;
		//	char sIP[] = "192.168.0.1";		// GPW杯でのサーバのIP
		//	char sIP[] = "202.250.144.34";	// nngs.computer-go.jp 9696

			// WinSockの初期化
			if ( GlobalTusin.Startup() != 0 ) {
				PRT("Error!: スタートアップ(): %d\n", GlobalTusin.GetLastError());
				return 0;
			}

			// ソケットを作る
			GlobalTusin.So_ck = GlobalTusin.CreateSo_cket();
			if ( GlobalTusin.So_ck == So_cket2Impl.InvalidSo_cket ) {
				PRT("Error: クリエイト・ソケット(): %d\n", GlobalTusin.GetLastError());
				return 0;
			}
	
			// IPアドレスを数値に変換
			addrIP = GlobalTusin.inet_addr(sNngsIP[0]);
			if ( addrIP == GlobalTusin.INADDR_NONE ) {
				host = GlobalTusin.GetHostByName(sNngsIP[0]);		// ホスト名から情報を取得してIPを調べる
				if ( host == NULL ) {
					PRT("Error: GetHostByName(): \n");
					return 0;
				}
				addrIP = *((ulong *)((host->h_addr_list)[0]));
			} else {
		//		host = gethostbyaddr((const string )&addrIP, 4, AF_INET);	// Localな環境では失敗する
			}

			if ( host ) {	// IP情報などを表示
				PRT("公式名 = %s\n",host->h_name);
				for (i=0; host->h_aliases[i]; i++) PRT("別名 = %s\n",host->h_aliases[i]);
				for (i=0; host->h_addr_list[i]; i++) {
					IN_ADDR *ip;
					ip = (IN_ADDR *)host->h_addr_list[i];
					PRT("IP = %s\n",inet_ntoa(*ip));
				}
			} else PRT("host = NULL,sIP=%s\n",sNngsIP[0]);

			// 非同期で通信する場合
			// 接続、送信、受信、切断のイベントをウィンドウメッセージで通知されるように
			if (
					fUseNngs
					&&
					GlobalTusin.AsyncSelect == GlobalTusin.SO_CKET_ERROR
				) {
				PRT("ソケットイベント通知設定に失敗\n");
				return 0;
			}

			// サーバーのIPアドレスとポート番号の指定
			memset(&Addr, 0, sizeof(Addr));
			Addr.sin_family      = AF_INET;
			Addr.sin_port        = htons(nPort);
			Addr.sin_addr.s_addr = addrIP;

			// サーバーへ接続	 
			if (
				GlobalTusin.So_ck.connectSo_ck( (LPSOCKADDR)&Addr, sizeof(Addr) ) == GlobalTusin.SO_CKET_ERROR
			) {
				if ( GlobalTusin.GetLastError() != GlobalTusin.EWOULDBLOCK ) {
					PRT("Error: コネクト(): \n");
					return 0;
				}
			}

		//	cgf_wsk_nngs.close();	// 接続の切断
			return 1;
		}

		
		// Loginする('\n'を待たずにすぐ送る)
		public static void login(int fTurn)
		{
			string message;
			message = String.Format("%s\n",nngs_player_name[fTurn]);
			cgf_wsk_nngs.Send(message);	// すぐにユーザ名でログイン
		}
		
		// 接続の切断
		public static void close(void)
		{
			if ( GlobalTusin.So_ck != So_cket2Impl.InvalidSo_cket )
			{
				GlobalTusin.So_ck.CloseSo_cket();
			}
			GlobalTusin.So_ck = So_cket2Impl.InvalidSo_cket;
			GlobalTusin.Cleanup();
		}


    }
}
