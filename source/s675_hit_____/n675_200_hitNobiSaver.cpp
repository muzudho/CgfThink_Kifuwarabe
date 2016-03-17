#include "../../header/h190_board___/n190_150_liberty.h"
#include "..\..\header\h675_hit_____\n675_200_hitNobiSaver.h"

int HitNobiSaver::Evaluate(
	Core			core			,
	int				color			,
	int				node			,
	Board*			pBoard			,
	LibertyOfNodes* pLibertyOfNodes
)
{
	int score = 0;

	// 石を置く前の、上、右、下、左　にある自分の石（または連）の呼吸点の数の合計。
	int currentAdjLibertySum = 0;
	pLibertyOfNodes->ForeachArroundNodes(node, [&pBoard,&pLibertyOfNodes, &currentAdjLibertySum, color](int adjNode, bool& isBreak) {
		if (pBoard->ValueOf(adjNode)== color)//自分の石
		{
			currentAdjLibertySum += pLibertyOfNodes->ValueOf(adjNode);
		}
	});

	//----------------------------------------
	// 危機でも無いのに伸ばしてしまうということがないよう、呼吸点が 1〜3 のときを危機と定義します。
	//----------------------------------------
	if (!(0<currentAdjLibertySum && currentAdjLibertySum<4))
	{
		// 危機じゃなかった。
		goto gt_EndMethod;
	}
	// これで0による除算も回避☆

	// 呼吸点に自分の石を置いたと考えて、石を置いた局面のその自分の石（または連）の呼吸点を数えます。
	Liberty futureLiberty;
	futureLiberty.Count(node, color, pBoard);
	{
		int x, y;
		AbstractBoard::ConvertToXy(x, y, node);
		core.PRT(_T("futureLiberty(xy=%d,%d,color=%d) "), x,y, color);
	}

	// 評価値計算
	if (futureLiberty.liberty <= currentAdjLibertySum)
	{
		// ツケて　呼吸点が増えていないようでは話しになりません。
		//score += 0;
	}
	else
	{
		// ツケて　呼吸点が増えているので、どれだけ増えたかを数えます。
		int upLiberty = futureLiberty.liberty - currentAdjLibertySum;
		core.PRT(_T("futureLiberty.liberty=%d "), futureLiberty.liberty);
		core.PRT(_T("currentAdjLibertySum=%d "), currentAdjLibertySum);
		core.PRT(_T("upLiberty=%d "),upLiberty);

		//score += 40  // 40を基本に。
		score += 1  // 1 を基本に。
			+
			(upLiberty - 1) * 50    // 呼吸点が２以上増えるなら、
									// 呼吸点が１増えるたびに 50 点のボーナス。
			/
			(currentAdjLibertySum * currentAdjLibertySum * currentAdjLibertySum)
									// ツケる前の呼吸点の数が大きいほど、
									// スコアが減る（緊急の関心を薄れさせる）仕掛け。
									// 呼吸点 1 = 1／1 点
									// 呼吸点 2 = 1／8 点
									// 呼吸点 3 = 1／27 点
									// 呼吸点 4 = 1／64 点
			;
	}
	
gt_EndMethod:
	return score;
}
