using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P495____WinMain
{
    class cgf_win_WinMain
    {
        int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int cmdShow )
        {
	        MSG      msg;
	        WNDCLASS wc;
	        static char szAppName[] = "CGFGUI";
	        static HBRUSH hBrush = NULL;

	        ghInstance = hInstance;

        #ifdef CGF_E 
	        SetThreadLocale(LANG_ENGLISH);	// 無理やり場所を英語に。リソースが英語に変わる。テスト用。
        #endif

	        if ( !hPrevInstance ) {
		        wc.lpszClassName = szAppName;
		        wc.hInstance     = hInstance;
		        wc.lpfnWndProc   = WndProc;
		        wc.hCursor       = LoadCursor( NULL, IDC_ARROW);
		        wc.hIcon         = LoadIcon( hInstance, CGF_ICON );
		        wc.lpszMenuName  = szAppName;
        //		hBrush = CreateSolidBrush( RGB( 0,128,0 ) );	// 背景を緑に(wc.中のオブジェクトは終了時に自動的に削除される）
        //		wc.hbrBackground = hBrush;//	GetStockObject(WHITE_BRUSH);
	            wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH); 
		        wc.style         = CS_HREDRAW | CS_VREDRAW;
		        wc.cbClsExtra    = 0;
		        wc.cbWndExtra    = 0;
		        if (!RegisterClass( &wc )) return FALSE;		/* クラスの登録 */


	            wc.style         = CS_HREDRAW | CS_VREDRAW;
	            wc.lpfnWndProc   = DrawWndProc;
	            wc.cbClsExtra    = 0;
	            wc.cbWndExtra    = 4;
	            wc.hInstance     = hInstance;
	            wc.hIcon         = NULL;
	            wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		        hBrush = CreateSolidBrush( rgbBack );	// 背景色(wc.中のオブジェクトは終了時に自動的に削除される）
		        wc.hbrBackground = hBrush;//	GetStockObject(WHITE_BRUSH);
        //		wc.hbrBackground = GetStockObject(WHITE_BRUSH);
	            wc.lpszMenuName  = NULL;
	            wc.lpszClassName = "DrawWndClass";
	            if (! RegisterClass (&wc)) return FALSE;
	        }
	        else {
        //		MessageBox(NULL,"２つ起動しようとしています\n終了させて下さい","注意",IDOK);
        //		return FALSE;
	        }

	        // この行をCreateWindowの次に置くとだめ！。WM_CREATEを発行してしまうから！
	        if ( lpszCmdLine != NULL ) {
		        if ( strstr(lpszCmdLine,"--mode gtp") ) fGtpClient = 1;	// GTPの傀儡として動く
		        else strcpy(cDragFile,lpszCmdLine);	// ダブルクリックされた開くべきファイル名
	        }
	        /********** ウインドウの作成 ********/
	        hWndMain = CreateWindow(
						        szAppName,						/* Class name.   */                                  
        //						lpszCmdLine,        			/* 起動時 Pram.  */
						        szTitleName,					/* Title         */		       					
						        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,	/* Style bits.   */
						          0, 20,						/* X,Y - default.*/
						        640,580,						/* CX, CY        */
						        NULL,							/* 親はなし      */
						        NULL,							/* Class Menu.   */
						        hInstance,						/* Creator       */
						        NULL							/* Params.       */
					           );

	        ghWindow = hWndMain;

	        ShowWindow( hWndMain, cmdShow );
	        UpdateWindow(hWndMain);

	        hAccel = LoadAccelerators(hInstance, "CGF_ACCEL");	// アクセラレータキーのロード

	        while( GetMessage( &msg, 0, 0, 0) ) {
		        if ( IsDlgDoneMsg(msg) ) {	// アクセラレータキーやダイアログの処理
			        TranslateMessage( &msg );					// keyboard input.
			        DispatchMessage( &msg );
                }
	        }
	        return msg.wParam;
        }
    }
}
