
namespace CgfthinkCs.P100____TusinInfra
{
    public class So_cket2Impl : So_cket2
    {
        
        /// <summary>
        /// 旧名：INVALID_SOCKET
        /// </summary>
        public static So_cket2Impl InvalidSo_cket { get; set; }

        static So_cket2Impl()
        {
            So_cket2Impl.InvalidSo_cket = new So_cket2Impl();
        }

        public void CloseSo_cket()
        {
            // TODO: ソケットを閉じて下さい。
        }

        /// <summary>
        /// 旧名：connect
        /// </summary>
        public void ConnectSo_ck()
        {
        }

    }
}
