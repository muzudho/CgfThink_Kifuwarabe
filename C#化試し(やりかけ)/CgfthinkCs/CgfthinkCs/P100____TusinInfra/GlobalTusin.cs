using System;

namespace CgfthinkCs.P100____TusinInfra
{
    /// <summary>
    /// 旧名：
    /// FD_CONNECT
    /// FD_CLOSE
    /// FD_WRITE
    /// FD_READ
    /// </summary>
    public enum SelectEventFD
    {
        /// <summary>
        /// FIXME: うそ値。
        /// </summary>
        USO,

        /// <summary>
        /// 接続に成功
        /// </summary>
        CONNECT,

        /// <summary>
        /// 接続を切られた
        /// </summary>
        CLOSE,

        /// <summary>
        /// 送信バッファに空きができた
        /// </summary>
        WRITE,

        /// <summary>
        /// データを受信する
        /// </summary>
        READ,
    }

    public abstract class GlobalTusin
    {
        /// <summary>
        /// 旧名：SOCKET_ERROR
        /// </summary>
        public static int SO_CKET_ERROR = -1;//FIXME:数字はうそ。

        /// <summary>
        /// 旧名：WSAEWOULDBLOCK
        /// </summary>
        public static int EWOULDBLOCK = -2;//FIXME:数字はうそ。

        /// <summary>
        /// 
        /// </summary>
        public static int INADDR_NONE = -3;//FIXME:数字はうそ。

        /// <summary>
        /// 旧名 Sock
        /// </summary>
        public static So_cket2 So_ck { get; set; }

        static GlobalTusin()
        {
            GlobalTusin.So_ck = So_cket2Impl.InvalidSo_cket;
        }

        /// <summary>
        /// 旧名：WSAStartup
        /// </summary>
        public static int Startup()
        {
            //WSADATA WsaData;

            return 0;// WSAStartup(0x0101, &WsaData);
        }

        /// <summary>
        /// 旧名：socket
        /// </summary>
        public static So_cket2 CreateSo_cket()
        {
            // socket( PF_INET, SOCK_STREAM, 0 ) で、
            // ソケットを作れるらしい。
            return new So_cket2Impl();
        }

        /// <summary>
        /// 旧名：WSAGetLastError
        /// </summary>
        /// <returns></returns>
        public static int GetLastError()
        {
            return 0;
        }

        /// <summary>
        /// 旧名：WSAAsyncSelect
        /// </summary>
        /// <returns></returns>
        public static int AsyncSelect()
        {
            return 0;
                //WSAAsyncSelect(
                //         GlobalTusin.So_ck,
                //         ghWindow,
                //         WM_USER_ASYNC_SELECT,
            //             SelectEventFD.CONNECT | SelectEventFD.WRITE | SelectEventFD.READ | SelectEventFD.CLOSE
                //     );
        }

        /// <summary>
        /// 旧名：WSACleanup
        /// </summary>
        public static void Cleanup()
        {
        }

        /// <summary>
        /// 旧名：WSAGETASYNCERROR
        /// </summary>
        public static int GetAsyncError(Int64 lParam)
        {
            return 0;//FIXME:うそ値。
        }

        /// <summary>
        /// 旧名：WSAGETSELECTERROR
        /// </summary>
        public static int GetSelectError(Int64 lParam)
        {
            return 0;//FIXME:うそ値。
        }

        /// <summary>
        /// 旧名：WSAGETSELECTEVENT
        /// </summary>
        /// <returns></returns>
        public static SelectEventFD GetSelectEvent(Int64 lParam)
        {
            return SelectEventFD.USO;//FIXME:うそ値。
        }

        /// <summary>
        /// 旧名：inet_addr
        /// IPアドレスを数値に変換。
        /// </summary>
        /// <returns></returns>
        public static long inet_addr(string ipAddress_str)
        {
            return 0;//FIXME:うそ値。
        }

        /// <summary>
        /// 旧名：gethostbyname
        /// </summary>
        /// <returns></returns>
        public static string GetHostByName(string ipAddress_str)
        {
            return "";//FIXME:うそ値。
        }


        /// <summary>
        /// 旧名：send( GlobalTusin.So_ck, message, message.Length, 0)
        /// </summary>
        /// <param name="so_ck"></param>
        /// <param name="message"></param>
        /// <param name="num"></param>
        /// <returns></returns>
        public static int Send( So_cket2 so_ck, string message)
        {
            int num = 0;

            return 0;//FIXME:うそ値。nByteSend
        }
    }
}
