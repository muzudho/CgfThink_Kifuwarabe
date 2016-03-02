using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P295_Sgf
{
    class cgf_win_Sgf
    {

		unsigned long nSgfBufSize;	// バッファにロードしたサイズ
		unsigned long nSgfBufNum;	// バッファの先頭位置を示す
		char SgfBuf[SGF_BUF_MAX];	// バッファ

		// SGF用のバッファから1バイト読み出す。
		char GetSgfBuf(int *RetByte)
		{
			*RetByte = 0;
			if ( nSgfBufNum >= nSgfBufSize ) return 0;
			*RetByte = 1;
			return SgfBuf[nSgfBufNum++];
		}

		// 一行読み込む
		int ReadOneLine( string lpLine ) 
		{
			int i,RetByte;
			char c;

			i = 0;
			for ( ;; ) {
				c = GetSgfBuf(&RetByte);
				if ( RetByte == 0 ) break;	// EOF;
				if ( c == '\r' ) c = ' ';	// 小細工。たいした意味なし
				lpLine[i] = c;
				i++;
				if ( i >= 255 ) break;
				if ( c == '\r' || c == '\n' ) break;	// 改行はNULLに
			}
			lpLine[i] = 0;	// 最後にNULLをいれること！
			return (i);
		}


		// SGFを読み込む
		int ReadSGF(void)
		{
			int RetByte,nLen;
			int i,x,y,z;
			char c;
			char sCommand[256];	// SGF 用のコマンドを記憶
			BOOL fKanji;
			BOOL fAddBlack = 0;		// 置き石フラグ 


			nSgfBufNum = 0;

			for ( ;; ) {
				c = GetSgfBuf( &RetByte );
				if ( RetByte == 0 ) break;	// EOF;
				if ( c == '\r' || c == '\n' ) continue;	// 改行コードはパス
				if ( c == '(' ) break;
			}
			if ( c != '(' )	{
				PRT("SGFファイルの読み込み失敗。'(' が見つからない\n");
		//		break;
			}

			nLen = 0; 
			fKanji = 0 ;
			for ( ;; ) {
				c = GetSgfBuf( &RetByte );
				if ( RetByte == 0 ) break;	// EOF;
				if ( fKanji ) {		// 漢字の場合は無条件に2文字目を読む。
					sCommand[nLen] = c;
					nLen++;
					fKanji = 0;
					continue;
				}
				if ( c == '\r' || c == '\n' ) continue;	// 改行コードはパス
				if ( 0 <= c && c < 0x20 ) continue;	// 空白以下のコード（TAB含む）もパス。
				if ( c == ';' ) {
					fAddBlack = 0;		// 置き石フラグオフ
					nLen = 0;			// 初期化してみる。
					continue;			// 区切り、現在は無視。
				}
				if ( c == ')' && nLen == 0 ) break;		// SGF File の終了。
	
				if ( fAddBlack != 0 && ( !(nLen==0 && c != '[') ) ) {	// 置き石の時の処理
		//			PRT(":: %c ::\n",c);
					sCommand[nLen] = c;
					if ( nLen == 0 && c == ']' ) continue;
		//			if ( nLen == 0 && c != '[' ) pass;
					nLen++;
					if ( nLen == 4 ) {
						nLen = 0;
						c = sCommand[1];
						x = c - 'a' + 1;
						c = sCommand[2];
						y = c - 'a' + 1;
						if ( x <= 0 || x >= 20 || y <= 0 || y >= 20 ) {
							PRT("置き石範囲エラー\n");
							break;
						}
						z = y * 256 + x;
						ban_start[z] = fAddBlack;
						PRT("A%c[%c%c]",'B'*(fAddBlack==1)+'W'*(fAddBlack==2), sCommand[1],sCommand[2]);
					}
					continue;
				}

				
				if ( nLen == 0 ) {	// 最初の一文字目。
					if ( c=='[' || c==']' ) {
						PRT("[]?,");
		//				break;
						continue;
					}
		//			if ( 'a' <= c && c <= 'z' ) continue;	// 小文字は無視。
					if ( 'A' <= c && c <= 'Z' ) {			// 大文字。コマンド。
						sCommand[nLen] = c;
						nLen++;
						fAddBlack = 0;		// 置き石フラグオフ
					}
					continue;
				}		
				if ( nLen == 1 ) {
					if ( 'A' <= c && c <= 'Z' ) {			// 大文字。コマンド。
						sCommand[nLen] = c;
						nLen++;

						if ( strncmp(sCommand,"AB",2)==0 ) {	// 置き石（黒を置く）
							// 次に大文字、または ";" が来るまで置き石を探す。
							// AB[jp][jd][pj][dj][jj][dp][pd][pp][dd]  ;W[qf];B[md];W[qn];B[mp];W[nj];
							fAddBlack = 1;
							nLen = 0;
		//					PRT("ABを発見！\n");
						}
						if ( strncmp(sCommand,"AW",2)==0 ) {	// 置き石（白を置く）
							fAddBlack = 2;
							nLen = 0;
						}

						continue;
					}
					if ( c == '[' ) {	// B,W,C （黒、白、コメント）の時。
						sCommand[nLen] = c;
						nLen++;
					}
					continue;
				}
				if ( nLen >= 2 ) {
					if ( nLen == 2 && sCommand[1] != '[' && c != '[' ) continue;
					sCommand[nLen] = c;
					if ( nLen < 128 ) nLen++;	// 80文字以上は無視する
					if ( IsKanji( (unsigned char)c ) ) fKanji = 1;
		//			PRT("%d:%d,",fKanji,(unsigned char)c );

					if ( c == ']' ) {	// データ終わり。
						if ( sCommand[1] != '[' || sCommand[0] == 'C' ) {	// 移動手以外は無視
							sCommand[nLen] = 0;

							if ( strncmp(sCommand,"WL",2)==0 || strncmp(sCommand,"BL",2)==0 ) {	// 白、黒の残り時間
								static int left_time[2];
								int et,diff,wb = tesuu & 1;
								sCommand[nLen-1] = 0;
								et = atoi( sCommand+3 );
								diff = left_time[wb] - et;
								left_time[wb] = et;
								if ( diff >= 0 ) kifu[tesuu][2] = diff;
								nLen = 0;
								continue;
							}

							if ( sCommand[0] == 'C' && tesuu > 0 ) PRT("%3d:",tesuu);	// 途中のコメント
							PRT("%s\n",sCommand);
																				
							if ( strncmp(sCommand,"PB",2)==0 ) {	// 対局者名を取り込む
								sCommand[nLen-1] = 0;
								strcpy( sPlayerName[0], &sCommand[3] );
							}
							if ( strncmp(sCommand,"PW",2)==0 ) {
								sCommand[nLen-1] = 0;
								strcpy( sPlayerName[1], &sCommand[3] );
							}

							if ( strncmp(sCommand,"SZ",2)==0 ) {	// 盤の大きさ
								sCommand[nLen-1] = 0;
								ban_size = atoi( &sCommand[3] );
		//						if ( ban_size == BAN_19 || ban_size == BAN_13 || ban_size == BAN_9 ) ;
		//						else ban_size = 19;
								init_ban();
								// 盤面を初期化 ---> 置き碁のため
								for (i=0;i<BANMAX;i++) {
									ban_start[i] = ban[i];	// 開始局面を設定。初期状態は全て０
									if ( ban_start[i] != 3 ) ban_start[i] = 0;
								}
								SendMessage(hWndMain,WM_COMMAND,IDM_BAN_CLEAR,0L);	// オープン版では盤面初期化
							}

							if ( strncmp(sCommand,"RE",2)==0 ) {	// 結果
								sCommand[nLen-1] = 0;
								strcpy( sResult, &sCommand[3] );
							}
							if ( strncmp(sCommand,"PC",2)==0 ) {	// 場所
								sCommand[nLen-1] = 0;
								strcpy( sPlace, &sCommand[3] );
							}
							if ( strncmp(sCommand,"GN",2)==0 ) {	// ラウンド
								sCommand[nLen-1] = 0;
								strcpy( sRound, &sCommand[3] );
							}
							if ( strncmp(sCommand,"EV",2)==0 ) {	// イベント名
								sCommand[nLen-1] = 0;
								strcpy( sEvent, &sCommand[3] );
							}
							if ( strncmp(sCommand,"DT",2)==0 ) {	// 日付
								sCommand[nLen-1] = 0;
								strcpy( sDate, &sCommand[3] );
							}
							if ( strncmp(sCommand,"KM",2)==0 ) {	// コミ
								sCommand[nLen-1] = 0;
								komi = atof(&sCommand[3] );
							}
							if ( strncmp(sCommand,"HA",2)==0 ) {	// 置石の数
								int n;
								
								sCommand[nLen-1] = 0;
								n = atoi( &sCommand[3] );
								nHandicap = n;
								if ( n == 13 ) n = 10;
								if ( n == 25 ) n = 11;
		//						PRT("nHandicap=%d,n=%d\n",nHandicap,n);
								if ( n >= 2 ) SetHandicapStones();
							}

							nLen = 0;
							continue;
						}
						// 移動手以外は無視
						if ( sCommand[0] == 'V' || sCommand[0] == 'N' ) {	// V[B+5]は評価値？
							PRT("%s",sCommand);
							nLen = 0;
							continue;
						}
						
						if ( sCommand[0] == 'T' && sCommand[1] =='[' ) {	// 消費時間
							sCommand[nLen-1] = 0;
							kifu[tesuu][2] = atoi( sCommand+2 );
							nLen = 0;
							continue;
						}
						
						if ( sCommand[0] != 'B' && sCommand[0] != 'W' ) {	// 移動手以外は無視
							sCommand[nLen] = 0;
							PRT("%s ---> コマンドエラー！！！\n",sCommand);
		//					nLen = 0;
							break;
						}
		//				if ( tesuu == 0 && sCommand[0] == 'W' ) fWhiteFirst = 1;	// 白から打つ
						c = sCommand[2];
						if ( c >= 'A' && c<= 'Z' ) c += 32;	// ugf対応
						x = c - 'a' + 1;
						c = sCommand[3];
						if ( c >= 'A' && c<= 'Z' ) { c += 32; c = 'a'-2 + 'u' - c; }
						y = c - 'a' + 1;
						if ( x == 20 || y == 20 ) {
							x = 0;
							y = 0;
						} else if ( nLen != 5 || x < 1 || x > ban_size || y < 1 || y > ban_size ) {
							sCommand[nLen] = 0;
							PRT("%s:  移動場所エラー\n",sCommand);
		//					nLen = 0;
		//					break;
							PRT("不正な場所だが、パスとみなして続行\n");
							x = 0; y = 0;
						}
						z = y * 256 + x;
						tesuu++;	all_tesuu = tesuu;
						kifu[tesuu][0] = z;
						kifu[tesuu][1] = 2 - (sCommand[0] == 'B');	// 石の色
						kifu[tesuu][2] = 0;	// 思考時間、この後あれば設定される。
						sCommand[nLen] = 0;
		//				PRT("%s, z=%04x: ",sCommand,z);
						nLen = 0;
					}

					if ( nLen >= 255 ) {
						PRT("データの内容が255文字以上。ちと長すぎ\n");
						break;
					}
				}
			}

		//	for (i=0;i<2;i++) {	if ( strstr( sPlayerName[i], "Dai") != 0 ) sprintf(sPlayerName[i],"GnuGo3.5.4");}
		//	komi = 6;

			return 0;
		}

		// SGFのバッファに書き出す
		void SGF_OUT(string format, ...)
		{
			va_list ap;
			int n;
			char text[TMP_BUF_LEN];

			va_start(ap, format);
			vsprintf( text , format, ap );
			va_end(ap);

			n = strlen(text);
			if ( nSgfBufNum + n >= SGF_BUF_MAX ) { PRT("SgfBuf over\n"); return; }

			strncpy(SgfBuf+nSgfBufNum,text,n);
			nSgfBufNum += n;
			SgfBuf[nSgfBufNum] = 0;
			nSgfBufSize = nSgfBufNum;	// 一応代入
		}

		// SGFを書き出す
		void SaveSGF(void)
		{
			int i,j,x,y,z,m0,m1,s0,s1;
			int fSgfTime,fAdd;
			time_t timer;
			struct tm *tb;

			nSgfBufNum = 0;

			// 時刻の取得
			timer = time(NULL);
			tb = localtime(&timer);   /* 日付／時刻を構造体に変換する */

			SGF_OUT("(;\r\n"); 
			SGF_OUT("GM[1]SZ[%d]\r\n",ban_size); 
			SGF_OUT("PB[%s]\r\n",sPlayerName[0]); 
			SGF_OUT("PW[%s]\r\n",sPlayerName[1]); 
			if ( sDate[0] ) SGF_OUT("DT[%s]\r\n",sDate);
			else            SGF_OUT("DT[%4d-%02d-%02d]\r\n",tb->tm_year+1900,tb->tm_mon+1,tb->tm_mday); 
		//	SGF_OUT("DT[%4d-%02d-%02d %02d:%02d:%02d]\r\n",year, month, day, hour, minute, second); 
			double score = LatestScore - komi;
			if ( score > 0 ) SGF_OUT("RE[B+%.1f]\r\n", score);
			else             SGF_OUT("RE[W+%.1f]\r\n",-score);

			if ( nHandicap ) SGF_OUT("HA[%d]",nHandicap); 
			SGF_OUT("KM[%.1f]TM[]",komi); 
			if ( fChineseRule ) SGF_OUT("RU[Chinese]"); 
			else                SGF_OUT("RU[Japanese]"); 
			SGF_OUT("PC[%s]",sPlace); 
			SGF_OUT("EV[%s]",sEvent); 
			SGF_OUT("GN[%s]",sRound); 
			m0 = total_time[0] / 60;		
			s0 = total_time[0] -(m0 * 60);
			m1 = total_time[1] / 60;
			s1 = total_time[1] -(m1 * 60);
			SGF_OUT("AP[%s]\r\n",sVersion);
			fSgfTime = 0;
			for ( i=1;i<tesuu+1; i++ ) if ( kifu[i][2] ) fSgfTime = 1;
			if ( fSgfTime ) {
				SGF_OUT("C[Time %d:%02d,%d:%02d]\r\n",m0,s0,m1,s1); 
				SGF_OUT("C[%s]\r\n",sYourCPU);
				SGF_OUT("C['T' means thinking time per a move.]\r\n");
			}
		//	SGF_OUT("WS[0]",sEvent);
		//	SGF_OUT("BS[7]",sEvent);
		//	SGF_OUT("NS[1]",sEvent);

			// 盤面だけを書き込む
			fAdd = 0;
			for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
				z = (y+1)*256+x+1;
				if ( ban_start[z] != 1 ) continue;
				if ( fAdd == 0 ) { SGF_OUT("AB"); fAdd = 1; }
				SGF_OUT("[%c%c]",x+'a', y+'a' );
			}
			if ( fAdd ) SGF_OUT("\r\n");
			fAdd = 0;
			for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
				z = (y+1)*256+x+1;
				if ( ban_start[z] != 2 ) continue;
				if ( fAdd == 0 ) { SGF_OUT("AW"); fAdd = 1; }
				SGF_OUT("[%c%c]",x+'a', y+'a' );
			}
			if ( fAdd ) SGF_OUT("\r\n");
			// 無理矢理2回パス
		//	SGF_OUT("\nB[tt]W[tt]\n"); WriteFile( hFile,(void *)lpSend, strlen(lpSend), &RetByte, NULL );


			j = 0;
			for ( i=1;i<tesuu+1; i++ ) {
				z = kifu[i][0];
				if ( z != 0 ) {
					x = z & 0x00ff;
					y = (z & 0xff00) >> 8;
				} else {
					x = 20;
					y = 20;
				}
				if ( kifu[i][1] == 1 ) SGF_OUT(";B[%c%c]",x+'a'-1, y+'a'-1 );
				else                   SGF_OUT(";W[%c%c]",x+'a'-1, y+'a'-1 );
				// 棋譜に消費時間を保存。独自拡張。
				if ( fSgfTime ) { 
					SGF_OUT("T[%2d]",kifu[i][2]);
				}

				j++;
				if ( j==10 - 4*fSgfTime ) {	// 10手ごとに改行する。
					SGF_OUT("\r\n");
					j = 0;
				}
			}
			SGF_OUT(")\r\n");
		}


		// SGFをロードして盤面を構成する
		void LoadSetSGF(int fSgf)
		{
			int i;

			tesuu = all_tesuu = 0;
			nHandicap = 0;

			// 盤面を初期化 ---> 置き碁のため
			for (i=0;i<BANMAX;i++) {
				ban_start[i] = ban[i];	// 開始局面を設定。初期状態は全て０
				if ( ban_start[i] != 3 ) ban_start[i] = 0;
			}

			if ( fSgf ) ReadSGF();	// SGFを読み込む
			else        ReadIG1();	// ig1ファイルを読む

		#ifdef CGF_E
			PRT("Move Numbers=%d\n",tesuu);
		#else
			PRT("読み込んだ手数=%d\n",tesuu);
		#endif
			// 盤面を初期盤面に
			for (i=0;i<BANMAX;i++) ban[i] = ban_start[i];
		//	PRT("ban_size=%d\n",ban_size);
			reconstruct_ban();
			tesuu = all_tesuu;

			for (i=1; i<=tesuu; i++) {
				int z   = kifu[i][0];
				int col = kifu[i][1];
				if ( make_move(z,col) != MOVE_SUCCESS ) {
					PRT("棋譜読込エラー z=%x,col=%d,tesuu=%d \n",z,col,i);
					break;
				}
			}

			PaintUpdate();
		}


    }
}
