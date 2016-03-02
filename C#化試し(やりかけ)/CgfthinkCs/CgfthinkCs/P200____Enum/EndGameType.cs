using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P200____Enum
{
    /// <summary>
    /// 現在局面で何をするか、を指定
    /// </summary>
    public enum EndGameType
    {
        /// <summary>
        /// 通常の手
        /// </summary>
        ENDGAME_MOVE,

        /// <summary>
        /// 終局処理
        /// </summary>
        ENDGAME_STATUS,

        /// <summary>
        /// 図形を描く
        /// </summary>
        ENDGAME_DRAW_FIGURE,

        /// <summary>
        /// 数値を書く
        /// </summary>
        ENDGAME_DRAW_NUMBER
    }
}
