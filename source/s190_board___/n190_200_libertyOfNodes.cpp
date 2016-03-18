#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_150_liberty.h"
#include "../../header/h190_board___/n190_200_libertyOfNodes.h"

LibertyOfNodes::LibertyOfNodes()
{
}

LibertyOfNodes::~LibertyOfNodes()
{
}

void LibertyOfNodes::Initialize( Board* pBoard)
{
	this->SetSize( pBoard->GetSize());

	// 枠を 0 に初期化。
	this->ForeachAllNodesOfWaku([this](int node, bool& isBreak) {
		// 呼吸点の数を覚えておく碁盤です。
		this->SetValue( node, 0);
	});

	pBoard->ForeachAllNodesWithoutWaku([this,&pBoard](int node, bool& isBreak) {
		Liberty liberty;
		liberty.Count(node, pBoard->ValueOf(node), pBoard);

		// 呼吸点の数を覚えておく碁盤です。
		this->SetValue( node, liberty.liberty);
	});
}
