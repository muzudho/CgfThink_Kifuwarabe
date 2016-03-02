using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P493____WinProc.L400_WindowsMessage
{
    public abstract class Wm_Create
    {
        public static void Perform()
        {
			if ( wParam == VK_RIGHT ) PostMessage(hWnd, WM_COMMAND, IDM_FORTH1,  0L);
			if ( wParam == VK_LEFT  ) PostMessage(hWnd, WM_COMMAND, IDM_BACK1,   0L);
			if ( wParam == VK_UP    ) PostMessage(hWnd, WM_COMMAND, IDM_BACK10,  0L);
			if ( wParam == VK_DOWN  ) PostMessage(hWnd, WM_COMMAND, IDM_FORTH10, 0L);
   			if ( wParam == VK_PRIOR ) PostMessage(hWnd, WM_COMMAND, IDM_BACK10,  0L);
			if ( wParam == VK_NEXT  ) PostMessage(hWnd, WM_COMMAND, IDM_FORTH10, 0L);
			if ( wParam == VK_HOME  ) PostMessage(hWnd, WM_COMMAND, IDM_BACKFIRST, 0L);
			if ( wParam == VK_END   ) PostMessage(hWnd, WM_COMMAND, IDM_FORTHEND, 0L);

			if ( wParam == 'E' ) PostMessage(hWnd, WM_COMMAND, IDM_VIEW_LIFEDEATH, 0L);
			if ( wParam == 'F' ) PostMessage(hWnd, WM_COMMAND, IDM_VIEW_FIGURE, 0L);
			if ( wParam == 'N' ) PostMessage(hWnd, WM_COMMAND, IDM_VIEW_NUMBER, 0L);
			if ( wParam == 'P' ) PostMessage(hWnd, WM_COMMAND, IDM_PASS, 0L);


			if ( wParam == '1' ) PostMessage(hWnd, WM_COMMAND, IDM_VIEW_LIFEDEATH, 0L);
			if ( wParam == '2' ) PostMessage(hWnd, WM_COMMAND, IDM_VIEW_FIGURE, 0L);
			if ( wParam == '3' ) PostMessage(hWnd, WM_COMMAND, IDM_VIEW_NUMBER, 0L);
        }
    }
}
