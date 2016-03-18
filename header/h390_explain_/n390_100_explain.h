#pragma once

	//--------------------------------------------------------------------------------
	// enum まとめ
	//--------------------------------------------------------------------------------

	// コンピューター囲碁ソフト『彩』のスクリーンショットでも見たことのある人もいるかもしれない、
	// 盤面、石の上に描く記号
	// (形 | 色) で指定する。黒で四角を描く場合は (FIGURE_SQUARE | FIGURE_BLACK)
	enum FigureType {
		FIGURE_NONE,			// 何も描かない
		FIGURE_TRIANGLE,		// 三角形
		FIGURE_SQUARE,			// 四角
		FIGURE_CIRCLE,			// 円
		FIGURE_CROSS,			// ×
		FIGURE_QUESTION,		// "？"の記号	
		FIGURE_HORIZON,			// 横線
		FIGURE_VERTICAL,		// 縦線
		FIGURE_LINE_LEFTUP,		// 斜め、左上から右下
		FIGURE_LINE_RIGHTUP,	// 斜め、左下から右上
		FIGURE_BLACK = 0x1000,	// 黒で描く（色指定)
		FIGURE_WHITE = 0x2000,	// 白で描く	(色指定）
	};
