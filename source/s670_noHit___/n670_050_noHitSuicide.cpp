
extern "C" {

	#include "../../header/h190_board___/n190_100_board.h"
	#include "../../header/h190_board___/n190_150_liberty.h"
	#include "../../header/h300_move____/n300_100_move.h"
	#include "../../header/h670_noHit___/n670_050_noHitSuicide.h"
}




NoHitSuicide::NoHitSuicide() {
	this->flgCapture = 0;
}




// 自殺手になる状況でないか調査。
bool NoHitSuicide::IsThis(
	int		color		,
	int		node		,
	Liberty liberties[4],
	Board*	pBoard
)
{
	bool result = false;
	int invColor = INVCLR(color);	//白黒反転
	int iDir;
	int adjNode;	// 上下左右隣(adjacent)の交点
	int adjColor;	// 上下左右隣(adjacent)の石の色

	for (iDir = 0; iDir < 4; iDir++) {		// 上隣 → 右隣 → 下隣 → 左隣
		adjNode = node + pBoard->dir4[iDir];	// 隣接(adjacent)する交点と、
		adjColor = pBoard->table[adjNode];		// その色

		// 隣に、呼吸点が 1 個の相手の石があれば、それは取ることができます。
		if (adjColor == invColor && liberties[iDir].liberty == 1) {
			//PRT(_T("敵石を取った。 \n"));
			this->flgCapture = 1; 	// 敵石を、取ったフラグ。
		}
	}

	int flgMove;	// 移動結果の種類

	if (this->flgCapture == 0) {					// 石が取れない場合
													// 実際に置いてみて　自殺手かどうか判定
		int temp_kouNode = pBoard->kouNode;		// コウの位置を退避

		flgMove = MoveOne(node, color, pBoard);		// 石を置きます。コウの位置が変わるかも。

											// 石を置く前の状態に戻します。
		pBoard->table[node] = 0;					// 置いた石を消します。
		pBoard->kouNode = temp_kouNode;			// コウの位置を元に戻します。

		if (flgMove == MOVE_SUICIDE) {		// 自殺手なら
											//PRT(_T("自殺手は打たない。 \n"));
											// ベストムーブにはなりえない
			result = true;
			goto gt_EndMethod;
		}
	}

gt_EndMethod:
	return result;
}
