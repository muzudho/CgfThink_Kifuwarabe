#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_250_markingBoard.h"

void MarkingBoard::Initialize(Board * pBoard)
{
	this->SetSize(pBoard->GetSize());

	// ˜g‚ÆA˜g“à‘S‚Ä‚ð 0 ‚É‰Šú‰»B
	this->ForeachAllNodesWithWaku([this](int node, bool& isBreak) {
		this->SetValue(node, 0);
	});
}
