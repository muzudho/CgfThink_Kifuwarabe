using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P450____Nngs.L400_Analysis
{
    public abstract class cgf_wsk_analysis
    {
		string[] stone_str = new string{ "B","W" };

        /// cgf_win_nngsMain.OnReceivedNngs から呼び出される。
        ///
        /// 0...PASS,
        /// 1以上...手。
        /// -1以下...エラーやログイン処理
        ///
        /// nngsの内容を解析
        ///
        ///int fTurn;	// 黒番の時 0、白番の時 1
        ///
        public static int Analysis(
            string str,
            int fTurn
        )
		{
			string tmp;
			/*
				if ( strstr(str,"Login:") ) {	// 改行コードで1行を判断してるので失敗する
					sprintf(tmp,"%s\n",nngs_player_name[fTurn]);	// ユーザ名でログイン
					cgf_wsk_nngs.Send(tmp);	
					return Result_NngsConnection.LOGIN_TOTYU;
				}
			*/
			if ( 0!=str.IndexOf("No Name Go Server (NNGS) version") ) {
				cgf_wsk_nngs.Send("set client TRUE\n");	// シンプルな通信モードに
				return Result_NngsConnection.LOGIN_TOTYU;
			}

			if ( 0!=str.IndexOf("Set client to be True") ) {
				if ( fTurn == 0 ) {
					tmp = String.Format("match %s B %d %d 0\n",nngs_player_name[1-fTurn],ban_size,nngs_minutes);
					cgf_wsk_nngs.Send(tmp);	// 黒番の場合に、試合を申し込む
				}
				return Result_NngsConnection.LOGIN_TOTYU;
			}	

			tmp = String.Format("Match [%dx%d]",ban_size,ban_size);	// "Match [19x19]"
			if ( 0!=str.IndexOf(tmp) ) {
				tmp = String.Format("match %s W %d %d 0\n",nngs_player_name[1-fTurn],ban_size,nngs_minutes);
				cgf_wsk_nngs.Send(tmp);	// 白番の場合に、試合を受ける。
				return Result_NngsConnection.LOGIN_END;
			}

			if ( fTurn == 0 && 0!= str.IndexOf("accepted.") ) {	// 白が応じたので初手を送れる。
				return Result_NngsConnection.LOGIN_END;
			}	

			if ( 0!= str.IndexOf("Illegal") ) return Result_NngsConnection.GAME_END;	// Errorの場合

			tmp = String.Format("(%s): ",stone_str[1-fTurn]);
			if(0!=str.IndexOf(tmp))
			{
				string p = tmp;
				int x,y;

				p += 5;
				if ( 0!=p.IndexOf("Pass") ) {
					PRT("相手 -> PASS!\n");
					return 0;
				} else {
					x = *p;
					y = atoi(p+1);
					PRT("相手 -> %c%d\n",x,y);
					x = x  - 'A' - (x > 'I') + 1;	// 'I'を超えたら -1	
					return (ban_size +1 - y)*256 + x;
				}	
			}

			// Passが連続した場合に来る(PASSした後の相手からのPASSは来ない）
			if ( 0!= str.IndexOf("You can check your score") ) {
				tmp = String.Format(,"done\n");	// 地を計算するモードを終えるために "done" を送る
										// この後、"resigns." 文字列が来る。これでサーバに正式な棋譜として保存される。
				cgf_wsk_nngs.Send(tmp);
				return 0;	// PASSを返す
			}

			// 終局処理が終わった。"9 {Game 1: test2 vs test1 : Black resigns. W 10.5 B 6.0}"
			if ( 0!=str.IndexOf("9 {Game") && 0!= str.IndexOf("resigns.") ) {
				tmp = String.Format("%s vs %s",nngs_player_name[1],nngs_player_name[0]);
				if ( 0!=str.IndexOf(tmp) ) return Result_NngsConnection.GAME_END;	// game end
			}

			tmp = String.Format("{%s has disconnected}",nngs_player_name[1-fTurn]);
			if ( 0!=str.IndexOf(tmp) ) return Result_NngsConnection.GAME_END;	// 対戦相手が接続を切った。

			// どちらかの時間が切れた場合
			if ( 0!= str.IndexOf("forfeits on time") &&
				 0!= str.IndexOf(nngs_player_name[0]) && 0!= str.IndexOf(nngs_player_name[1]) ) {
				return Result_NngsConnection.GAME_END;
			}
			return Result_NngsConnection.UNKNOWN;	// 無意味なデータ
		}
    }
}
