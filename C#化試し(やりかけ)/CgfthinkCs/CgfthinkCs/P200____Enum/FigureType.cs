using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CgfthinkCs.P200____Enum
{
    /// <summary>
    /// 図形を描く時の指定。盤面、石の上に記号を書ける。
    /// (形 | 色) で指定する。黒で四角を書く場合は (FIGURE_SQUARE | FIGURE_BLACK)
    /// </summary>
    public enum FigureType
    {
        /// <summary>
        /// 何も書かない
        /// </summary>
	    FIGURE_NONE,

        /// <summary>
        /// 三角形
        /// </summary>
	    FIGURE_TRIANGLE,

        /// <summary>
        /// 四角
        /// </summary>
	    FIGURE_SQUARE,

        /// <summary>
        /// 円
        /// </summary>
	    FIGURE_CIRCLE,

        /// <summary>
        /// ×
        /// </summary>
	    FIGURE_CROSS,

        /// <summary>
        /// "？"の記号	
        /// </summary>
	    FIGURE_QUESTION,

        /// <summary>
        /// 横線
        /// </summary>
	    FIGURE_HORIZON,

        /// <summary>
        /// 縦線
        /// </summary>
	    FIGURE_VERTICAL,

        /// <summary>
        /// 斜め、左上から右下
        /// </summary>
	    FIGURE_LINE_LEFTUP,
        
        /// <summary>
        /// 斜め、左下から右上
        /// </summary>
	    FIGURE_LINE_RIGHTUP,

        /// <summary>
        /// 黒で書く（色指定)
        /// </summary>
	    FIGURE_BLACK = 0x1000,

        /// <summary>
        /// 白で書く	(色指定）
        /// </summary>
	    FIGURE_WHITE = 0x2000,
    }
}
