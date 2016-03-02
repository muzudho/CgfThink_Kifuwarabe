using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P295_Sgf
{
    class cgf_win_Ig1
    {
        // ig1ファイルを読む
        int ReadIG1(void)
        {
	        int i,x=0,y,yl,yh=0,z, xFlag,yFlag=0, nLen;
	        int RetByte;		// 読み込まれたバイト数
	        char c;
	        char  lpLine[256];	// １分を読む
	
	        nSgfBufNum = 0;
	
	        for ( i=0;i<255;i++) {
		        sPlayerName[0][i] = 0;
		        sPlayerName[1][i] = 0;
	        }

	        // 一行をロードする
	        // "INT" が見つかるまでロード
	        for ( ;; ) {
		        nLen = ReadOneLine( lpLine );
		        if ( nLen == 0 ) {
			        PRT("EOF - 読み込み失敗。MOV が見つからない\n");
			        break;
		        }
		        lpLine[nLen] = 0;
		        PRT(lpLine);
		        if ( lpLine[0] == 'M' && lpLine[1] == 'O' && lpLine[2] == 'V' ) break;
		        if ( strstr( lpLine, "PLAYER_BLACK" ) ) strncpy( sPlayerName[0], &lpLine[13], nLen-13-1);
		        if ( strstr( lpLine, "PLAYER_WHITE" ) ) strncpy( sPlayerName[1], &lpLine[13], nLen-13-1);
		        if ( strstr( lpLine, "黒：" ) ) strncpy( sPlayerName[0], &lpLine[4], nLen-4-1);
		        if ( strstr( lpLine, "白：" ) ) strncpy( sPlayerName[1], &lpLine[4], nLen-4-1);
	        }

	        if ( nLen ) {	
		        tesuu = 0;
		        xFlag = 1;
		        for ( ;; ) {
			        c = GetSgfBuf( &RetByte );
			        if ( RetByte == 0 ) break;	// EOF;
			        if ( c == '\r' || c == '\n' ) continue;	// 改行コードはパス
			        if ( c <= 0x20 ) continue;	// 空白以下のコード（TAB含む）は区切り
			        if ( c == '-' ) break;
			        if ( xFlag == 1 && 'A' <= c && c <= 'T'	) {
				        x = c  - 'A' - (c > 'I') + 1;	// 'I'を超えたら-1	
				        xFlag = 0;
				        yFlag = 1;
			        }
			        else if ( yFlag == 1 && '0'	<= c && c <= '9' ) {
				        yh = c - '0';
				        yFlag = 2;
			        }
			        else if ( yFlag == 2 && '0'	<= c && c <= '9' ) {
				        yl = c - '0';
				        yFlag = 1;
				        xFlag = 1;
				        y = 10*yh + yl;
				        if ( x==1 && y==0 ) z = 0;
				        else z = ( y<<8 ) + x;
				        tesuu++;	all_tesuu = tesuu;
				        kifu[tesuu][0] = z;
				        kifu[tesuu][1] = 2 - ( tesuu & 1 );	// 先手が黒
			        }
		        }
	        }
	        return 0;
        }

        // ig1ファイルを書き出す
        void SaveIG1(void)
        {
	        int i,j,x,y,z;
	        time_t timer;
	        struct tm *tb;

	        nSgfBufNum = 0;

	        // 時刻の取得
	        timer = time(NULL);
	        tb = localtime(&timer);   /* 日付／時刻を構造体に変換する */

	        // \n だけだと 0x0a のみが入り好ましくないので 0x0d(\r)を追加 
	        SGF_OUT("#COMMENT\r\n");
	        SGF_OUT("#--- CgfGoBan 棋譜ファイル（GOTERMと互換性あり）---\r\n");
	        SGF_OUT("#対局日 %4d-%02d-%02d\r\n",tb->tm_year+1900,tb->tm_mon+1,tb->tm_mday); 
	        SGF_OUT("PLAYER_BLACK %s\r\n",sPlayerName[0]);
	        SGF_OUT("PLAYER_WHITE %s\r\n",sPlayerName[1]);

	        double score = LatestScore - komi;
	        if ( score > 0 ) SGF_OUT("RESULT B+%.1f\r\n", score);
	        else             SGF_OUT("RESULT W+%.1f\r\n",-score);
        //	SGF_OUT("PLACE %s\r\n",sPlace);
        //	SGF_OUT("EVENT %s\r\n",sEvent);
        //	SGF_OUT("GAME_NAME Round %s\r\n",sRound);

	        SGF_OUT("INT %d 0\r\n",ban_size);
	        SGF_OUT("MOV\r\n");

	        j = 0;
	        for ( i=1;i<tesuu+1; i++ ) {
		        z = kifu[i][0];
		        if ( z != 0 ) {
			        x = z & 0x00ff;
			        y = (z & 0xff00) >> 8;
		        } else {
			        x = 1;
			        y = 0;
		        }
		        SGF_OUT(" %c%02d",x+64+(x>8),y );
 		        j++;
		        if ( j==19 ) {
			        SGF_OUT("\r\n");
			        j = 0;
		        }
	        }
	        SGF_OUT(" -\r\n");
        }

    }
}
