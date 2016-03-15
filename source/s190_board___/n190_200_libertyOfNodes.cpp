#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_150_liberty.h"
#include "../../header/h190_board___/n190_200_libertyOfNodes.h"

void LibertyOfNodes::Initialize( Board* pBoard)
{
	for (int x = 0; x < pBoard->size; x++)
	{
		for (int y = 0; y < pBoard->size; y++)
		{
			int node = Board::ConvertToNode(x, y);

			Liberty liberty;
			liberty.Count(node, pBoard);

			// ŒÄ‹z“_‚ðŠo‚¦‚Ä‚¨‚­Œé”Õ‚Å‚·B
			this->table[node] = liberty.liberty;
		}
	}
}
