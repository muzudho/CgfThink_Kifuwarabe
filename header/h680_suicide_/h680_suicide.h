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
	void IsThis(
		int adjColor,
		int invClr
	);

	void Judge(
		int&	flgAbort,
		int		color,
		int		node
	);
};
