extern "C" {
	#include "../../header/h090_core____/n090_100_core.h"
	#include "../../header/h190_board___/n190_100_board.h"
	#include "../../header/h670_noHit___/n670_120_noHitHasinohoBocchi.h"
}




NoHitHasinohoBocchi::NoHitHasinohoBocchi()
{
	this->isBocchi		= false;
	this->isSoto		= false;
	this->isEdge		= false;
	this->isCorner		= false;
}





void NoHitHasinohoBocchi::Research(
	int		node,
	Board*	pBoard
	) {

	this->isSoto		= false;
	this->isEdge		= false;
	this->isCorner		= false;



	this->isBocchi = true;
	for (int iDir = 0; iDir < 4; iDir++) {		// 上隣 → 右隣 → 下隣 → 左隣
		int adjNode = node + pBoard->dir4[iDir];		// 隣接(adjacent)する交点と、
		int adjColor = pBoard->table[adjNode];		// その色

		if (adjColor==BLACK || adjColor==WHITE)
		{
			// ぼっちではない。
			this->isBocchi = false;
			break;
		}
	}



	int x, y;
	Board::ConvertToXy(x, y, node);

	if (x < 1 || pBoard->size < x ||
		y < 1 || pBoard->size < y
		) {
		// 盤外
		//PRT(_T("(%d,%d) ban=%d ; Soto \n"), x, y, boardSize);
		this->isSoto = true;
		goto gt_EndMethod;
	}

	if (x == 1 || pBoard->size == x ||
		y == 1 || pBoard->size == y
	) {
		// 辺
		//PRT(_T("(%d,%d) ban=%d ; EDGE \n"), x, y, boardSize);
		this->isEdge = true;
	}
	else
	{
		//PRT(_T("(%d,%d) ban=%d ; ------ \n"), x, y, boardSize);
		goto gt_EndMethod;
	}

	if ( (x == 1 || pBoard->size == x) &&
		(y == 1 || pBoard->size == y)
	) {
		// 角
		//PRT(_T("(%d,%d) ban=%d ; CORNER \n"), x, y, boardSize);
		this->isCorner = true;
	}

gt_EndMethod:
	;
}




// 評価値を出します。
int NoHitHasinohoBocchi::Evaluate(
	) {
	int score = 100;

	if (!this->isBocchi)
	{
		// ぼっち石でなければ、気にせず　端でも角でも置きます。
		goto gt_EndMethod;
	}

	if (this->isCorner)
	{
		// 角に　ぼっち石　を置きたくない。
		score -= 50;
		goto gt_EndMethod;
	}

	if (this->isEdge)
	{
		// 辺に　ぼっち石　を置きたくない。
		score -= 33;
		goto gt_EndMethod;
	}

gt_EndMethod:
	return score;
}
