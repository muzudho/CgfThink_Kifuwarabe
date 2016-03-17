#include <tchar.h> // Unicode対応の _T() 関数を使用するために。
#include "../../header/h090_core____/n090_100_core.h"
#include "..\..\header\h675_hit_____\n675_150_hitAte.h"

int HitAte::Evaluate(Core core, int color, int node, Board* pBoard, LibertyOfNodes* pLibertyOfNodes)
{
	int score		= 0;
	int opponent	= INVCLR(color);

	// 上右下左に、相手の石がないか探します。
	pBoard->ForeachArroundNodes(node, [&core,&pBoard,&pLibertyOfNodes,&score, opponent](int adjNode, bool& isBreak) {
		int libertyOfRen = pLibertyOfNodes->ValueOf(adjNode);
		int x, y;
		AbstractBoard::ConvertToXy(x, y, adjNode);
		//core.PRT(_T("adj(%d,%d)LibRen=%d"), x, y, libertyOfRen);

		if (pBoard->ValueOf(adjNode) == opponent && libertyOfRen<4)
		{
			// 相手の石（または連）で、呼吸点が 3 箇所以下の物を選びます。

			std::vector<int> openNodes = pBoard->GetOpenNodesOfStone(core, adjNode, libertyOfRen);
			//core.PRT(_T("開%d"), openNodes.size());
			if (!openNodes.empty())
			{
				// この石（連ではなく）の開いている方向（１方向～３方向）がある場合。

				if (openNodes.size() == 1)
				{
					//core.PRT(_T("Ate!"));

					// アタリ　の状態です。
					if (score < 120)
					{
						// 他の指し手に　これといった手がないようなら、アテにいきましょう。
						score += 120;
					}
				}
				else {
					// アタリ　ではない場合。
				}
			}
		}

		//core.PRT(_T(";"));
	});


	return score;
}
