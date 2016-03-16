#include "..\..\header\h675_hit_____\n675_150_hitAte.h"

int HitAte::Evaluate(int color, int node, Board* pBoard, LibertyOfNodes* libertyOfNodes)
{
	int score		= 0;
	int opponent	= INVCLR(color);

	int libertyOfRen = libertyOfNodes->ValueOf(node);
	if (pBoard->ValueOf(node) == opponent && libertyOfRen<4)
	{
		// 相手の石（または連）で、呼吸点が 3 箇所以下の物を選びます。

		std::vector<int> openNodes = pBoard->GetOpenNodesOfStone(node, libertyOfRen);
		if(!openNodes.empty())
		{
			// この石（連ではなく）の開いている方向（１方向～３方向）がある場合。

			if (openNodes.size()==1)
			{
				// アタリ　の状態です。
				if (score < 120)
				{
					// 他の指し手に　これといった手がないようなら、アテにいきましょう。
					score = 120;
				}
			} else {
				// アタリ　ではない場合。
			}
		}
	}

	return score;
}
