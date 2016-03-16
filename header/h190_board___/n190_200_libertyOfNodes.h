#pragma once


#include "../h190_board___/n190_050_abstractBoard.h"
#include "../h190_board___/n190_100_board.h"



// 自分の位置を含む石、または連の総呼吸点の数を、各交点に格納したテーブルです。
class LibertyOfNodes : public AbstractBoard{
public:
	LibertyOfNodes();
	~LibertyOfNodes();

	void Initialize( Board* pBoard);

	// 枠も含めて碁盤を全走査
	// .cpp に本体を書くとなんかエラーが出たので、.h に書いているんだぜ☆（＾ｑ＾）
	template<typename Func> void ForeachAllXyWithWaku(Func func)
	{
		for (int y = 0; y < this->size + 2; y++)
		{
			for (int x = 0; x < this->size + 2; x++)
			{
				bool isBreak = false;
				func(x, y, isBreak);
				if (isBreak)
				{
					break;
				}
			}
		}
	}
};