using SocketServer.P900_Etc;
using System;
using System.Windows.Forms;

namespace SocketServer
{
    /// <summary>
    /// https://msdn.microsoft.com/ja-jp/library/fx6588te(v=vs.110).aspx
    /// </summary>
    public partial class Uc_Main : UserControl
    {
        public Uc_Main()
        {
            InitializeComponent();
        }

        /// <summary>
        /// [サーバー起動]ボタン押下。
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            AsynchronousSocketListener.StartListening();
        }
    }
}
