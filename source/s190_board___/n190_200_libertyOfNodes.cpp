#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_150_liberty.h"
#include "../../header/h190_board___/n190_200_libertyOfNodes.h"

void LibertyOfNodes::Initialize(int board[], int boardSize)
{
	for (int x = 0; x < boardSize; x++)
	{
		for (int y = 0; y < boardSize; y++)
		{
			int node = ConvertToNode(x, y);

			Liberty liberty;
			liberty.Count(node, board);

			this->board[node] = liberty.liberty;
		}
	}
}
