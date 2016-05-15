#include "../../header/n190_board___/n190_100_board.h"
#include "../../header/n190_board___/n190_250_markingBoard.h"

void MarkingBoard::Initialize(Board * pBoard)
{
	this->SetSize(pBoard->GetSize());

	// 枠と、枠内全てを 0 に初期化。
	this->ForeachAllNodesWithWaku([this](int node, bool& isBreak) {
		this->SetValue(node, 0);
	});
}
