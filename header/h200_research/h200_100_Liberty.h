//
// /header/h600_research/h600_100_adjLiberty.h
//

#pragma once


#include "../../header/h190_board___/h190_board.h"




class Liberty {
public:
	// 既にこの石を検索した場合は1
	int checkedBoard[BOARD_MAX];

	// 連のリバティ（石の呼吸点）の数(再帰関数で使う)
	int liberty;

	// 隣接する（１個あるいは連の）石の数(再帰関数で使う)
	int renIshi;

public:
	Liberty();


	// 位置 tNode におけるリバティ（石の呼吸点）の数と石の数を計算。結果はグローバル変数に格納。
	void	Count(
		int node
		);

	// リバティ（石の呼吸点）と石の数える再帰関数
	// 4方向を調べて、空白だったら+1、自分の石なら再帰で。相手の石、壁ならそのまま。
	void	CountElement(
		int tNode,
		int color
		);
};