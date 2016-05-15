#pragma once

#include <windows.h>
#include "../n190_board___/n190_100_board.h"




class BoardView {
public:
	// 現在の盤面を表示
	static void PrintBoard(Core core, Board* pBoard);
};
