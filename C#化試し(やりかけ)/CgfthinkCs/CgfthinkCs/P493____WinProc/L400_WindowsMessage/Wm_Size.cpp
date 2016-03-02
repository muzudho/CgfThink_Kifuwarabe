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
			SendMessage(hWndToolBar,msg,wParam,lParam);
			if (wParam != SIZE_MINIMIZED) MoveDrawWindow(hWnd);
        }
    }
}
