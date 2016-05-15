#pragma once


#include "../n190_board___/n190_050_abstractBoard.h"
#include "../n190_board___/n190_100_board.h"



// 自分の位置を含む石、または連の総呼吸点の数を、各交点に格納したテーブルです。
class LibertyOfNodes : public AbstractBoard{
public:
	LibertyOfNodes();
	~LibertyOfNodes();

	void Initialize( Board* pBoard);
};