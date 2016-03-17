#include "../../header/h190_board___/n190_150_liberty.h"
#include "..\..\header\h675_hit_____\n675_200_hitNobiSaver.h"

int HitNobiSaver::Evaluate(Core core, int color, int node, Board * pBoard, LibertyOfNodes * pLibertyOfNodes)
{
	int score = 0;

	/*
	// まず　呼吸点が１つ、
	// 次に　呼吸点が２つ、
	// と、呼吸点の少ない方から順に評価を付けていきます。
	// 盤面を４回探し回ることになります。
	//
	for (int iExpectedLiberty = 1; iExpectedLiberty < 4; iExpectedLiberty++)
	{
		pBoard->ForeachAllNodesWithoutWaku([&pBoard,&pLibertyOfNodes,&score, color, iExpectedLiberty](int node, bool& isBreak) {

			int currentLiberty = pLibertyOfNodes->ValueOf(node); // 石を置く前の呼吸点の数。

			if (
				pBoard->ValueOf(node) == color // 脅かされているかもしれない自分の石
				&&
				currentLiberty == iExpectedLiberty // 呼吸点の数
			)
			{
				// 上、右、下、左を調べます。
				pBoard->ForeachArroundNodes(node, [&pBoard,&score, color, node, currentLiberty](int adjNode, bool& isBreak) {
					if (pBoard->ValueOf(adjNode) == EMPTY)
					{
						// わたしの石の北隣にある空きスペースの位置。

						// 空きスペースに石を置いたと考えて、石を置いた局面のその自分の石（または連）の呼吸点を数えます。
						Liberty futureLiberty;
						futureLiberty.Count(adjNode, color, pBoard);

						// 評価値計算
						if (futureLiberty.liberty <= currentLiberty)
						{
							// ツケて　呼吸点が減っているようでは話しになりません。
							//score += 0;
						}
						else
						{
							// ツケて　呼吸点が増えているので、どれだけ増えたかを数えます。
							int upLiberty = futureLiberty - currentLiberty;

							score += 40  // 40を基本に。
								+
								(upLiberty - 1) * 50    // 呼吸点が２以上増えるなら、
														// 呼吸点が１増えるたびに 50 点のボーナス。
								/
								(currentLiberty * currentLiberty * currentLiberty)  // ツケる前の呼吸点の数が大きいほど、
																					// スコアが減る（緊急の関心を薄れさせる）仕掛け。
																					// 呼吸点 1 = 1／1 点
																					// 呼吸点 2 = 1／8 点
																					// 呼吸点 3 = 1／27 点
																					// 呼吸点 4 = 1／64 点
								;
						}

						//found = true;
					}
					else if
					(
						pBoard->ValueOf(adjNode) == color // 北隣がコンピューターの石で、
						&&
						taikyoku.MarkingBoard.CanDo_North(node) // 北隣のマーキングが 0 なら
					)
					{
						// 再帰呼び出し
						if (Util_SasiteNext.FindStone_LibertyWeak(out tryLocation, out tryScore, myStone_location.ToNorth(), currentLiberty, taikyoku))    // 再帰的に検索
						{
							//found = true;
						}
					}

				});
			}

		});
	}

	*/
	return score;
}
