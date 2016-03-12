//
// /header/h680_suicide/h680_suicide.h
//

#pragma once

//--------------------------------------------------------------------------------
// クラス
//--------------------------------------------------------------------------------

//
// 自殺手を指さない仕組み。
//
class Suicide
{
public:
	// 敵石を取ったフラグ
	int flgCapture = 0;

public:
	Suicide();

	// 自殺手になる状況でないか調査。
	void Research(
		int invColor,
		int adjColor
	);

	// この交点には打ち込まないなら真。
	bool DontHit(
		int		color,
		int		node
	);
};
