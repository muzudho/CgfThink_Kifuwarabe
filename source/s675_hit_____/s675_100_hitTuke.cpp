//
// /source/s675_hit_____/s675_100_hitTuke.cpp
//

extern "C" {

	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h200_research/h200_100_Liberty.h"
	#include "../../header/h675_hit_____/h675_100_hitTuke.h"

}




HitTuke::HitTuke()
{
};




int HitTuke::Evaluate(
	int invColor,
	int node,
	Liberty liberties[4]
	)
{
	int score = 0;
	int iDir;
	int adjNode;
	int adjColor;

	for (iDir = 0; iDir < 4; iDir++) {		// ��� �� �E�� �� ���� �� ����
		adjNode	 = node + g_dir4[iDir];	// �א�(adjacent)�����_�ƁA
		adjColor = g_board[adjNode];		// ���̐F

		// �]���l�̌v�Z�i�S�������J��Ԃ��j
		score +=
			(adjColor == invColor)		// �ׂ̐�
										//		����̐�: 1
										//		����ȊO: 0
										//   �~
			* liberties[iDir].renIshi	// �A�̐΂̐�
										//   �~
			* (10 / (liberties[iDir].liberty + 1));	// �A�̌ċz�_�̌�
													//		0��: 10�_
													//		1��:  5�_
													//		2��:  3.3333...�_
													//		3��:  2.5�_
													//		4��:  2�_
													//		...
	}


	return score;
}