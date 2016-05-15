#pragma once

#include <windows.h>
#include "../n190_board___/n190_100_board.h"
#include "../n190_board___/n190_200_libertyOfNodes.h"




class LibertyOfNodesView {
public:
	// 現在の盤面を表示
	static void PrintBoard(Core core, LibertyOfNodes* pLibertyOfNodes);
};
