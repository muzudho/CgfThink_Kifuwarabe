namespace CgfthinkCs.P400____Nngs.L400_Analysis
{
    /// <summary>
    /// 旧、定数。
    /// NNGS_LOGIN_TOTYU
    /// NNGS_LOGIN_END
    /// NNGS_GAME_END
    /// NNGS_UNKNOWN
    /// </summary>
    public enum Result_NngsConnection
    {
        /// <summary>
        /// login処理の途中
        /// </summary>
        LOGIN_TOTYU = -3,

        /// <summary>
        /// login処理が終了した
        /// </summary>
        LOGIN_END   = -2,

        /// <summary>
        /// 対局終了
        /// </summary>
        GAME_END    = -1,

        /// <summary>
        /// 無意味なデータ。無視する。
        /// </summary>
        UNKNOWN     = -4
    }
}
