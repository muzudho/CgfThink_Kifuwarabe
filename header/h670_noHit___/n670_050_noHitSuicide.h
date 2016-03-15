//
// /header/h680_suicide/h680_suicide.h
//

#pragma once


#include "../../header/h190_board___/n190_150_liberty.h"
#include "../../header/h670_noHit___/n670_000_noHit.h"


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
		int		color,
		int		node,
		Liberty liberties[4]
	);
};
