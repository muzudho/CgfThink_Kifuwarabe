#pragma once

#include "../h190_board___/n190_100_board.h"




class BoardView {
public:
	// 現在の盤面を表示
	static void PrintBoard(HANDLE hConsoleWindow, Board* pBoard);
};
