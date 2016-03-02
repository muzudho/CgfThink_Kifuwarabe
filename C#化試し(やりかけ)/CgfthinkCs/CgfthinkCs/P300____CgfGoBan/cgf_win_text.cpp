using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P400____CgfGoBan
{
    class cgf_win_text
    {
        
        /*** printf の代用 ***/
        void PRT(string format, ...)
        {
	        va_list ap;
	        int i,len,x,y,nRet = 1;
        //	char c,text[256],sOut[MAX_PATH];
	        char c;
			string text;
			string sOut;
	        static FILE *fp;
	        RECT rc;
	        HDC hDC;

	        va_start(ap, format);
	        vsprintf( text , format, ap );

	        //// FILE書出用 ////
	        if ( fFileWrite == 1 ) {
		        sOut = sCgfDir;
		        sOut += "\\output";	// 吐き出すファイル名
		        fp = fopen(sOut,"a");
		        fprintf(fp,text);
		        fclose(fp);
	        }
	        ////////////////////

        //	PRT_Scroll();

	        len = (int)strlen(text);
	        for (i=0;   i<len; i++) {
		        c = text[i];
		        if ( c == '\n' ) {nRet++;PRT_Scroll();}
		        else {
			        cPrintBuff[PMaxY-1][PRT_x] = c;
			        if ( PRT_x == PRetX - 1 ) {nRet++;PRT_Scroll();}	// 一列は100行
			        else PRT_x++;
		        }
	        }
	        if ( hPrint != NULL && IsWindow(hPrint) ) {
		        if ( nRet == 1 ) {	// スクロールしない場合を高速描画
        //			rc.left  = (PRT_x-len)*nWidth;
        //			rc.right =  PRT_x     *nWidth;
			        hDC = GetDC(hPrint);
			        x = PRT_x - len;
			        y = PMaxY -1;
			        TextOut(hDC,x*nWidth,(y-PRT_y)*(nHeight),&cPrintBuff[y][x],strlen(&cPrintBuff[y][x]));
			        ReleaseDC(hPrint,hDC);
		        } else {
			        ScrollWindow( hPrint, 0, -nHeight*(nRet-1), NULL, NULL );
			        GetClientRect( hPrint, &rc);
			        rc.top    = (PMaxY-PRT_y - nRet)*nHeight;
			        rc.bottom = (PMaxY-PRT_y       )*nHeight;
			        InvalidateRect(hPrint, &rc, TRUE);
			        UpdateWindow(hPrint);
		        }
	        }
        //	MessageBox( ghWindow, text, "Printf Debug", MB_OK);
	        va_end(ap);
        }



        void PRT_Scroll(void)
        {
	        int i,j;

	        for (i=1;i<PMaxY;i++) {
		        for (j=0;j<PRetX;j++) cPrintBuff[i-1][j] = cPrintBuff[i][j];
	        }
	        for (i=0; i<PRetX; i++) cPrintBuff[PMaxY-1][i] = 0;
	        PRT_x = 0;
        }

        // PRTの内容をクリップボードにコピー
        void PRT_to_clipboard(void)
        {
	        int i;
	        HGLOBAL hMem;
	        LPSTR lpStr;
	        string sTmp;

	        hMem = GlobalAlloc(GHND, PMaxY*PMaxX);
	        lpStr = (LPSTR)GlobalLock(hMem);

	        lstrcpy( lpStr, "");

	        for (i=0;i<PMaxY;i++) {
		        if ( strlen(cPrintBuff[i]) == 0 ) continue;
		        sTmp = String.Format("%s\n",cPrintBuff[i]);
		        lstrcat(lpStr,sTmp);
	        }

	        GlobalUnlock(hMem);
	        if ( OpenClipboard(ghWindow) ) {
		        EmptyClipboard();
		        SetClipboardData( CF_TEXT, hMem );
		        CloseClipboard();
        //		PRT("クリップボードにPRTを出力しました\n");
	        }

        }

    }
}
