//
// /source/s670_noHit___/s670_050_suicide.cpp
//

extern "C" {

	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h300_move____/h300_move.h"
	#include "../../header/h670_noHit___/h670_050_noHitSuicide.h"
}




NoHitSuicide::NoHitSuicide() {
	this->flgCapture = 0;
}




// 自殺手になる状況でないか調査。
void NoHitSuicide::Research(
	int invColor,
	int node
)
{
	int iDir;
	int adjNode;	// 上下左右隣(adjacent)の交点
	int adjColor;	// 上下左右隣(adjacent)の石の色

	for (iDir = 0; iDir < 4; iDir++) {		// 上隣 → 右隣 → 下隣 → 左隣
		adjNode = node + g_dir4[iDir];	// 隣接(adjacent)する交点と、
		adjColor = g_board[adjNode];		// その色

		CountLiberty(adjNode);						// 隣の石（または連）の呼吸点　の数を数えます。

													// 隣に、呼吸点が 1 個の相手の石があれば、それは取ることができます。
		if (adjColor == invColor && g_liberty == 1) {
			//PRT(_T("敵石を取った。 \n"));
			this->flgCapture = 1; 	// 敵石を、取ったフラグ。
		}
	}

}




bool NoHitSuicide::DontHit(
	int		color,
	int		node
	)
{
	bool result = false;
	int flgMove;	// 移動結果の種類

	if (this->flgCapture == 0) {					// 石が取れない場合
											// 実際に置いてみて　自殺手かどうか判定
		int temp_kouNode = g_kouNode;		// コウの位置を退避

		flgMove = MoveOne(node, color);		// 石を置きます。コウの位置が変わるかも。

											// 石を置く前の状態に戻します。
		g_board[node] = 0;					// 置いた石を消します。
		g_kouNode = temp_kouNode;			// コウの位置を元に戻します。

		if (flgMove == MOVE_SUICIDE) {		// 自殺手なら
											//PRT(_T("自殺手は打たない。 \n"));
											// ベストムーブにはなりえない
			result = true;
			goto gt_EndMethod;
		}

	gt_EndMethod:
		return result;
	}
}
