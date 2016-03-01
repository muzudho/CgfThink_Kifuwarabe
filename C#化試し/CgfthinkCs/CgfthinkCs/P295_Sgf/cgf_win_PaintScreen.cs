using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P295_Sgf
{
    class cgf_win_PaintScreen
    {
        // 実際に描画を行う
        void PaintScreen( HWND hWnd, HDC hDC )
        {
	        HBRUSH hBrush,hBrushWhite,hBrushBlack;
	        HPEN hPen,hPenOld;
	        HFONT hFont,hFontOld;
	        RECT rRect;
	
	        int dx,sx,sy,db;
	        int x,y,z,xx,yy,i,j,k,n,size;
	        char sNum[TMP_BUF_LEN];

	        GetScreenParam(	hWnd,&dx,&sx,&sy,&db );

	        hBrushBlack = CreateSolidBrush( rgbBlack );	// 黒石
	        hBrushWhite = CreateSolidBrush( rgbWhite );	// 白石

	        SetTextColor( hDC, rgbText );	//　文字色
        //	SetBkColor( hDC, RGB(0,128,0) );		//　背景を濃い緑に

	        // 背景をまず描画する。
	        RECT rcFull;
	        GetClientRect( hWnd, &rcFull );	// 現在の画面の大きさを取得。グラフの大きさをこのサイズに合わせる
	        hBrush = CreateSolidBrush(rgbBlack);
	        FillRect(hDC, &rcFull, hBrush );
	        DeleteObject(hBrush);

	        /* 盤面全体の表示 */
	        SetRect( &rRect , sx, sy, sx+dx*(ban_size-1)+db*2, sy+dx*(ban_size-1)+db*2 );
	        n = ID_NowBanColor - ID_BAN_1;
        //	PRT("n=%d\n",n);
	        if ( n >= RGB_BOARD_MAX || n < 0 ) { PRT("色がない n=%d\n",n); debug(); }
	        hBrush = CreateSolidBrush( rgbBoard[n] );
	        FillRect( hDC, &rRect, hBrush );
	        DeleteObject(hBrush);
			
   	        hPen = CreatePen(PS_SOLID, 1, rgbBlack );
	        hPenOld = (HPEN)SelectObject( hDC, hPen );
	        for (x=1;x<ban_size-1;x++) {
		        MoveToEx( hDC, x*dx+sx+db , sy+db, NULL );
                LineTo( hDC, x*dx+sx+db , sy+db+dx*(ban_size-1) );
		        MoveToEx( hDC, sx+db      , x*dx+sy+db, NULL );
                LineTo( hDC, sx+db+dx*(ban_size-1) , x*dx+sy+db );
	        }
	        SelectObject( hDC, hPenOld );
	        DeleteObject(hPen);

   	        hPen = CreatePen(PS_SOLID, 2, rgbBlack );
	        SelectObject( hDC, hPen );
	        MoveToEx( hDC, sx+db , sy+db, NULL );
	        LineTo( hDC, sx+db , sy+db+dx*(ban_size-1) );
	        LineTo( hDC, sx+db+dx*(ban_size-1), sy+db+dx*(ban_size-1) );
	        LineTo( hDC, sx+db+dx*(ban_size-1), sy+db );
	        LineTo( hDC, sx+db, sy+db );
	        SelectObject( hDC, hPenOld );
	        DeleteObject(hPen);

	        /* 星の点 */
	        SelectObject( hDC, hBrushBlack );
	        if ( ban_size == BAN_19 ) {
    	        for ( i=0;i<3;i++) {
    		        x =  sx+db+dx*3 + dx*6*i;
    		        for ( j=0;j<3;j++) {
		    	        y =  sy+db+dx*3 + dx*6*j;
		    	        Ellipse( hDC, x-2,y-2, x+3,y+3);
			        }
		        }
	        } else if (ban_size == BAN_13 ) {
   		        x =  sx+db+dx*6;
    	        y =  sy+db+dx*6;
    	        Ellipse( hDC, x-2,y-2, x+3,y+3);
    	        for ( i=0;i<2;i++) {
    		        x =  sx+db+dx*3 + dx*6*i;
    		        for ( j=0;j<2;j++) {
		    	        y =  sy+db+dx*3 + dx*6*j;
		    	        Ellipse( hDC, x-2,y-2, x+3,y+3);
			        }
		        }
	        } else {
   		        if ( ban_size & 1 ) {
			        n = ban_size / 2;
			        x =  sx+db+dx*n;
    		        y =  sy+db+dx*n;
    		        Ellipse( hDC, x-2,y-2, x+3,y+3);
		        }
	        }

	        SetBkMode( hDC, TRANSPARENT);	// TRANSPARENT ... 重ねる, OPAQUE...塗りつぶし

	        // 題字「CGF碁盤」の表示
	        x = sx+db+dx*(ban_size)+8;
	        y = 15;

        #ifdef CGF_E
	        hFont = CreateFont( 36,0, 0, 0, FW_BOLD, 0, 0, 0,
		        ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		        DEFAULT_PITCH | FF_SCRIPT, "Comic Sans MS");
        //		DEFAULT_PITCH | FF_SCRIPT, "Gungsuh");
	        // "Arial","Tahoma","Times New Roman","MS UI Gothic","Mattise ITC","Lucida Sans","Lucida Handwriting"
	        // Arial Black,Arial Narrow,Batang,Century,Comic Sans MS,Georgia,Gulim,Gungsuh,GungsuhChe,Impact
	        x += 5;
	        y += 5;
        #else
	        hFont = CreateFont( 36,0, 0, 0, FW_BOLD, 0, 0, 0,
		        SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		        DEFAULT_PITCH | FF_SCRIPT, "標準ゴシック");
	        y += 10;
        #endif
	        hFontOld = (HFONT)SelectObject( hDC, hFont );
	        SetTextColor( hDC, rgbBlack );		// 影を黒で
	        n = strlen(szTitleName);
	        TextOut( hDC, x+4, y+4, szTitleName,n );
	        SetTextColor( hDC, rgbWhite );	//　文字を白に
	        TextOut( hDC, x-1, y-1, szTitleName,n );
	        TextOut( hDC, x+1, y-1, szTitleName,n );
	        TextOut( hDC, x-1, y+1, szTitleName,n );
	        TextOut( hDC, x+1, y+1, szTitleName,n );
	        SetTextColor( hDC, RGB(128,0,128) );
	        TextOut( hDC,   x,   y, szTitleName,n );
	        x += 7;
        #ifdef CGF_E
	        x -= 5;
        #endif
   	        hPen = CreatePen(PS_SOLID, 2, RGB( 255,255,0 ) );
	        hPenOld = (HPEN)SelectObject( hDC, hPen );
	        MoveToEx( hDC, x -10, 70, NULL );
	        LineTo(   hDC, x+125, 70 );
	        MoveToEx( hDC, x -10, 15, NULL );
	        LineTo(   hDC, x+125, 15 );
	        SelectObject( hDC, hPenOld );
	        DeleteObject(hPen);
	        SelectObject( hDC, hFontOld );
	        DeleteObject(hFont);


	        /* 数字 */
	        SetTextColor( hDC, rgbText );
	        if ( dx > 16 ) {
		        k = 18;
		        n = 5;
		        xx = 0;
		        yy = 0;
	        } else {
		        k = 12;
		        n = 2;
		        xx = 4;
		        yy = 4;
	        }
	
	        hFont = CreateFont( k,0, 0, 0, FW_NORMAL, 0, 0, 0,
		        SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		        DEFAULT_PITCH | FF_SCRIPT, FontName);
	        hFontOld = (HFONT)SelectObject( hDC, hFont );
	        for (i=0;i<ban_size;i++) {
		        if ( fATView ) {
			        // チェス式座標系
			        sprintf( sNum,"%d",ban_size-i);	
			        TextOut( hDC, xx + (ban_size-i<=9)*n , sy+db-8 + i*dx, sNum ,strlen(sNum) );
			        sprintf( sNum,"%c",i+'A'+(i>7) );	// A-T 表記
			        TextOut( hDC, sx+db-4 + i*dx, 0, sNum ,strlen(sNum) );
		        } else {
        #ifdef CGF_DEV
			        sprintf( sNum,"%x",i+1);
			        TextOut( hDC, xx + (i<15)*n , yy + sy+db-8 + i*dx, sNum ,strlen(sNum) );
			        sprintf( sNum,"%x",i+1 );
			        TextOut( hDC, sx+db-4 + i*dx - (i>14)*n, yy, sNum ,strlen(sNum) );
        #else
			        sprintf( sNum,"%d",i+1);
			        TextOut( hDC, xx + (i<9)*n , sy+db-8 + i*dx, sNum ,strlen(sNum) );
			        sprintf( sNum,"%d",i+1 );
			        TextOut( hDC, sx+db-4 + i*dx - (i>8)*n, yy, sNum ,strlen(sNum) );
        #endif
		        }
	        }
	        SelectObject( hDC, hFontOld );
	        DeleteObject(hFont);

	        /* 盤面にある石を表示 */
	        for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
		        z = ((y+1)<<8) + (x+1);
	   	        xx = sx+db+dx*x;
	   	        yy = sy+db+dx*y;
		        k = ban[z];
		        if ( k == 0 ) continue;
		        if ( k == 1 ) SelectObject( hDC, hBrushBlack );		// 黒石
		        else          SelectObject( hDC, hBrushWhite );		// 白石

        //	   	Ellipse( hDC, xx-dx/2,yy-dx/2, xx+dx/2+1,yy+dx/2+1);		// ディスプレイドライバによる？
	   	        Ellipse( hDC, xx-dx/2,yy-dx/2, xx+dx/2,  yy+dx/2  );		// Windows98 ViperV550 ではこちら。
        //		Chord( hDC,  xx-dx/2,yy-dx/2, xx+dx/2,yy+dx/2, xx-dx/2,yy-dx/2, xx-dx/2,yy-dx/2 );	// Ellipse は動作がおかしい（はみ出る）Win98 ViperV550では描画されないことあり。

		        if ( kifu[tesuu][0] == z ) {	// 直前に打たれた石に手数を表示
			        sprintf( sNum,"%d",tesuu);
			        n = strlen(sNum);
			        if ( n==1 ) size = dx*3/4;
			        else if ( n==2 ) size = dx*7/10;
			        else size = dx*7/12;
			        DrawValueNumber(hDC, size, k, xx, yy, sNum);
		        }
	        }

	
	        // 死活を表示
	        if ( endgame_board_type == ENDGAME_STATUS ) {
		        for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
			        z = ((y+1)<<8) + (x+1);
	   		        xx = sx+db+dx*x;
	   		        yy = sy+db+dx*y;
			
			        k = ban[z];
			        n = endgame_board[z];
			        if ( k == 0 ) {
				        // 確定地を表示
				        hBrush = NULL;
				        if ( n == GTP_WHITE_TERRITORY ) hBrush = hBrushWhite;
				        if ( n == GTP_BLACK_TERRITORY ) hBrush = hBrushBlack;
				        if ( hBrush == NULL ) continue;
				        SelectObject( hDC, hBrush );
				        Rectangle( hDC, xx-dx/4,yy-dx/4, xx+dx/4,yy+dx/4);
				        continue;
			        }
			        if ( n != GTP_DEAD ) continue;

		   	        // 死んでいる石に×をつける
			        if ( k == 1 ) hPen = CreatePen(PS_SOLID, 2, rgbWhite );
			        else          hPen = CreatePen(PS_SOLID, 2, rgbBlack );
			        hPenOld = (HPEN)SelectObject( hDC, hPen );
			        MoveToEx( hDC, xx-dx/6,yy-dx/6, NULL );
			        LineTo(   hDC, xx+dx/6,yy+dx/6 );
			        MoveToEx( hDC, xx-dx/6,yy+dx/6 , NULL );
			        LineTo(   hDC, xx+dx/6,yy-dx/6 );
			        SelectObject( hDC, hPenOld );
			        DeleteObject(hPen);
		        }
	        }
	        // 数値を表示
	        if ( endgame_board_type == ENDGAME_DRAW_NUMBER ) {
		        for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
			        z = ((y+1)<<8) + (x+1);
	   		        xx = sx+db+dx*x;
	   		        yy = sy+db+dx*y;
			        k = ban[z];
			        n = endgame_board[z];
			        if ( n == 0 ) continue;

			        sprintf( sNum,"%3d",n);
			        n = strlen(sNum);
			        size = dx*7/12;
			        if ( n == 4 ) size = dx*7/16; 
			        if ( n >= 5 ) size = dx*7/20; 
			        DrawValueNumber(hDC, size, k, xx, yy, sNum);
		        }
	        }
	        // 図形を表示
	        if ( endgame_board_type == ENDGAME_DRAW_FIGURE ) {
		        for (y=0;y<ban_size;y++) for (x=0;x<ban_size;x++) {
			        int m,d;
			        z = ((y+1)<<8) + (x+1);
	   		        xx = sx+db+dx*x;
	   		        yy = sy+db+dx*y;
        //			k = ban[z];
			        n = endgame_board[z];
			
			        m = n & ~(FIGURE_BLACK | FIGURE_WHITE);	// 色指定を消す
			        d = 2;	// ペンの太さ
			        if ( m == FIGURE_TRIANGLE || m == FIGURE_SQUARE	|| m == FIGURE_CIRCLE ) d = 1;
			        if ( n & FIGURE_WHITE ) hPen = CreatePen(PS_SOLID, d, rgbWhite );
			        else                    hPen = CreatePen(PS_SOLID, d, rgbBlack );
			        hPenOld = (HPEN)SelectObject( hDC, hPen );

			        if ( n & FIGURE_WHITE ) hBrush = hBrushWhite;
			        else                    hBrush = hBrushBlack;
			        SelectObject( hDC, hBrush );


			        if ( m == FIGURE_TRIANGLE ) {	// 三角形
				        int x1 = dx /  3;
				        int y1 = dx /  3;
				        int y2 = dx /  5;
				        MoveToEx( hDC, xx   ,yy-y1, NULL );
				        LineTo(   hDC, xx+x1,yy+y2 );
				        LineTo(   hDC, xx-x1,yy+y2 );
				        LineTo(   hDC, xx   ,yy-y1 );
			        }
			        if ( m == FIGURE_SQUARE ) {		// 四角
				        int x1 = dx / 4;
				        Rectangle( hDC, xx-x1,yy-x1, xx+x1,yy+x1);
			        }
			        if ( m == FIGURE_CIRCLE ) {		// 円
				        int x1 = dx / 5;
				        Chord( hDC, xx-x1,yy-x1, xx+x1,yy+x1, xx-x1,yy-x1, xx-x1,yy-x1 );
			        }
			        if ( m == FIGURE_CROSS ) {		// ×
				        int x1 = dx / 6;
				        MoveToEx( hDC, xx-x1,yy-x1, NULL );
				        LineTo(   hDC, xx+x1,yy+x1 );
				        MoveToEx( hDC, xx-x1,yy+x1, NULL );
				        LineTo(   hDC, xx+x1,yy-x1 );
			        }
			        if ( m == FIGURE_QUESTION ) {	// "？"の記号	
				        sprintf( sNum,"?" );
				        size = dx*10/12;
				        if ( n & FIGURE_BLACK ) k = 2;
				        else                    k = 1;
				        DrawValueNumber(hDC, size, k, xx, yy, sNum);
			        }
			        if ( m == FIGURE_HORIZON ) {	// 横線
				        int x1 = dx * 10 / 22;
				        MoveToEx( hDC, xx-x1,yy, NULL );
				        LineTo(   hDC, xx+x1,yy );
			        }
			        if ( m == FIGURE_VERTICAL ) {	// 縦線
				        int x1 = dx * 10 / 22;
				        MoveToEx( hDC, xx,yy-x1, NULL );
				        LineTo(   hDC, xx,yy+x1 );
			        }
			        if ( m == FIGURE_LINE_LEFTUP ) {	// 斜め、左上から右下
				        int x1 = dx * 10 / 22;
				        MoveToEx( hDC, xx-x1,yy-x1, NULL );
				        LineTo(   hDC, xx+x1,yy+x1 );
			        }
			        if ( m == FIGURE_LINE_RIGHTUP ) {	// 斜め、左下から右上
				        int x1 = dx * 10 / 22;
				        MoveToEx( hDC, xx-x1,yy+x1, NULL );
				        LineTo(   hDC, xx+x1,yy-x1 );
			        }

			        SelectObject( hDC, hPenOld );
			        DeleteObject(hPen);
		        }
	        }



	        SelectObject( hDC, hBrushBlack );		// 黒石
	        xx = sx+db+dx*(ban_size)+ban_size;
   	        yy =  135;
   	        hPen = CreatePen(PS_SOLID, 1, rgbWhite );
	        hPenOld = (HPEN)SelectObject( hDC, hPen );
        #ifdef CGF_E
	        n = 12;
        #else
	        n = 13;
        #endif
	        Chord( hDC,  xx-n,yy-n, xx+n,yy+n, xx-n,yy-n,xx-n,yy-n );
	        SelectObject( hDC, hPenOld );
	        DeleteObject(hPen);
            SelectObject( hDC, hBrushWhite );		// 白石
   	        yy =  225;
	        Chord( hDC,  xx-n,yy-n, xx+n,yy+n, xx-n,yy-n,xx-n,yy-n );	// Ellipse は動作がおかしい（はみ出る）
        // 	Ellipse( hDC, xx-n,yy-n, xx+n,yy+n);


	        /* その他の画面情報 */
	        hFont = CreateFont( 16,0, 0, 0, FW_NORMAL, 0, 0, 0,
		        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		        DEFAULT_PITCH | FF_SCRIPT, FontName);
	        hFontOld = (HFONT)SelectObject( hDC, hFont );
	        SetTextColor( hDC, rgbText );	// 文字色
        #ifdef CGF_E
	        n = 3;
        #else
	        n = 0;
        #endif
	        sprintf( sNum,info_str[0]);
	        TextOut( hDC, xx-8+n,128, sNum ,strlen(sNum) );
	        sprintf( sNum,"%s",sPlayerName[0]);
	        TextOut( hDC, xx+20,128, sNum ,strlen(sNum) );

	        SetTextColor( hDC, rgbBlack );	// 文字を黒に
	        sprintf( sNum,info_str[1]);
	        TextOut( hDC, xx-8+n,218, sNum ,strlen(sNum) );
	        SetTextColor( hDC, rgbWhite );	// 文字を白に
	        sprintf( sNum,"%s",sPlayerName[1]);
	        TextOut( hDC, xx+20,218, sNum ,strlen(sNum) );

	        sprintf( sNum,info_str[2],hama[0]);
	        TextOut( hDC, xx-16,152, sNum ,strlen(sNum) );
	        sprintf( sNum,info_str[2],hama[1]);
	        TextOut( hDC, xx-16,242, sNum ,strlen(sNum) );
	
	        sprintf( sNum,info_str[3],tesuu);
	        TextOut( hDC, xx-16,300, sNum ,strlen(sNum) );
	        k = kifu[tesuu][0];
	        if ( k==0 ) {
		        sprintf( sNum,"PASS");
	        } else {
		        if ( fATView ) {
			        sprintf( sNum,"%c -%2d",'A'+(k&0xff)+((k&0xff)>8)-1,ban_size-(k>>8)+1);
		        } else {
			        sprintf( sNum,"(%2d,%2d)",k&0xff,k>>8);
		        }
	        }
	        TextOut( hDC, xx-16+8*8,300, sNum ,strlen(sNum) );

	        if ( IsBlackTurn() ) sprintf( sNum,info_str[4]);
	        else                 sprintf( sNum,info_str[5]);
	        if ( fNowPlaying ) TextOut( hDC, xx-16,340, sNum ,strlen(sNum) );

	        if ( endgame_board_type == ENDGAME_STATUS ) {
		        endgame_score_str(sNum,0);
		        TextOut( hDC, xx-16,390, sNum ,strlen(sNum) );

		        sprintf( sNum,"Komi=%.1f",komi);
		        TextOut( hDC, xx-16,430, sNum ,strlen(sNum) );
		        if ( fChineseRule ) {
			        // 中国ルールでカウントしてみる。
			        // アゲハマは無視。盤上に残っている石の数と地の合計。
			        endgame_score_str(sNum,1);
			        TextOut( hDC, xx-16,450, sNum ,strlen(sNum) );
		        }
	        }

	        // 思考時間の表示
	        count_total_time();

	        for (i=0;i<2;i++) {
		        int m,s,mm,ss;

		        m = total_time[i] / 60;
		        s = total_time[i] -(m * 60);
		             if ( tesuu == 0    ) n = 0;
		        else if ( (tesuu+i) & 1 ) n = tesuu;
		        else                      n = tesuu - 1;
		        mm = kifu[n][2] / 60;
		        ss = kifu[n][2] - (mm * 60);
		        sprintf( sNum,"%02d:%02d / %02d:%02d",mm,ss,m,s);
		        int flag = i;
		        if ( tesuu > 0 && kifu[1][1]==2 ) flag = 1 - i;	// 白から打っている場合は表示を逆に。
		        TextOut( hDC, xx-16,174+90*flag, sNum ,strlen(sNum) );
	        }


	        SelectObject( hDC, hFontOld );
	        DeleteObject(hFont);

	        DeleteObject(hBrushWhite);
	        DeleteObject(hBrushBlack);
        }

    }
}
