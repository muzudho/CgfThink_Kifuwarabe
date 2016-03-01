using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P493____WinProc
{
    class cgf_win_WinProc
    {
		/// ************* ウィンドウ手続き ***************
		LRESULT CALLBACK WndProc(
			HWND hWnd,
			UINT msg,
			WPARAM wParam,
			Int64 lParam
		)
		{
			HMENU hMenu;
			int i,k,z,ret;
			MEMORYSTATUS MemStat;	// 物理メモリ測定用

			if ( fPassWindows )
			{
				for (;;) {	// 思考中はすべてカットする。
					if ( msg == WM_CLOSE ) { PRT("\n\n現在思考中...別のボタンを。\n\n"); return 0; }
					if ( msg == WM_COMMAND && (LOWORD(wParam) == IDM_DLL_BREAK || LOWORD(wParam) == IDM_BREAK) ) {
						PRT("\n\n現在思考中...無理やり停止。\n\n");
						thinking_utikiri_flag = 1;
						return 0;
					}
					if ( msg == WM_USER_ASYNC_SELECT ) break;	// 通信情報は受け取る
					if ( msg == WM_COMMAND && LOWORD(wParam) == IDM_KIFU_SAVE ) break;
					return( DefWindowProc( hWnd, msg, wParam, lParam) );
				}
			}

			switch( msg ) {
				case WM_CREATE:
					// 初期設定
					Wm_Create.Perform();
					break;

				case WM_COMMAND:
					Wm_Command.Perform();
					return 0;

				case WM_KEYDOWN:
					Wm_KeyDown.Perform();
					break;

				case WM_USER_ASYNC_SELECT:	// 非同期のソケット通知(nngsサーバとの通信用)
					Wm_UserAsyncSelect.Perform();
					break;

				case WM_NOTIFY:
					Wm_Notify.Perform();
					break;

				case WM_SIZE:
					Wm_Size.Perform();
					break;

				case WM_DESTROY:
					Wm_Destroy.Perform();
					break;
		
				default:
					return( DefWindowProc( hWnd, msg, wParam, lParam) );
			}
			return 0L;
		}
    }
}
