#pragma once


#include <windows.h> // コンソールへの出力等
#include "../h090_core____/n090_100_core.h"
#include "../h190_board___/n190_150_liberty.h"
#include "../h670_noHit___/n670_000_noHit.h"


//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------

//
// 自殺手を指さない仕組み。
//
class NoHitSuicide : NoHit
{
public:
	// 敵石を取ったフラグ
	int flgCapture;

public:
	NoHitSuicide();

	// 自殺手になる状況でないか調査。
	bool IsThis(
		Core	core		,
		int		color		,
		int		node		,
		Liberty liberties[4],
		Board*	pBoard
	);
};
