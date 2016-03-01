using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P200____Enum
{
    /// <summary>
    /// 死活情報でセットする値
    /// その位置の石が「活」か「死」か。不明な場合は「活」で。
    /// その位置の点が「黒地」「白地」「ダメ」か。
    /// </summary>
    public enum GtpStatusType
    {
        /// <summary>
        /// 活
        /// </summary>
	    GTP_ALIVE,

        /// <summary>
        /// 死
        /// </summary>
	    GTP_DEAD,

        /// <summary>
        /// セキで活（未使用、「活」で代用して下さい）
        /// </summary>
	    GTP_ALIVE_IN_SEKI,

        /// <summary>
        /// 白地
        /// </summary>
	    GTP_WHITE_TERRITORY,

        /// <summary>
        /// 黒地
        /// </summary>
	    GTP_BLACK_TERRITORY,

        /// <summary>
        /// ダメ
        /// </summary>
	    GTP_DAME
    }
}
