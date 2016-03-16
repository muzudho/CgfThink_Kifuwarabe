#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_150_liberty.h"
#include "../../header/h190_board___/n190_200_libertyOfNodes.h"

void LibertyOfNodes::Initialize( Board* pBoard)
{
	pBoard->ForeachAllNodesWithoutWaku([this,&pBoard](int node) {
		Liberty liberty;
		liberty.Count(node, pBoard);

		// 呼吸点を覚えておく碁盤です。
		this->table[node] = liberty.liberty;
	});
	/*
	for (int x = 1; x < pBoard->size+1; x++)
	{
		for (int y = 1; y < pBoard->size+1; y++)
		{
			int node = Board::ConvertToNode(x, y);

			Liberty liberty;
			liberty.Count(node, pBoard);

			// 呼吸点を覚えておく碁盤です。
			this->table[node] = liberty.liberty;
		}
	}
	*/
}
