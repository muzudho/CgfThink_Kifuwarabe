#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_150_liberty.h"
#include "../../header/h190_board___/n190_200_libertyOfNodes.h"

void LibertyOfNodes::Initialize( Board* pBoard)
{
	pBoard->ForeachAllNodesWithoutWaku([this,&pBoard](int node) {
		Liberty liberty;
		liberty.Count(node, pBoard);

		// �ċz�_���o���Ă�����Ղł��B
		this->table[node] = liberty.liberty;
	});
}
