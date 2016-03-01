using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SocketClient.P900____Etc;

namespace SocketClient
{
    /// <summary>
    /// https://msdn.microsoft.com/ja-jp/library/bew39x2a(v=vs.110).aspx
    /// </summary>
    public partial class Uc_Main : UserControl
    {
        public Uc_Main()
        {
            InitializeComponent();
        }

        /// <summary>
        /// [クライアント起動]ボタン押下。
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            AsynchronousClient.StartClient();
        }
    }
}
