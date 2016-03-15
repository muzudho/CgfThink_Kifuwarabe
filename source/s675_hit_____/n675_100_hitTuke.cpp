extern "C" {

	#include "../../header/h190_board___/n190_100_board.h"
	#include "../../header/h190_board___/n190_150_Liberty.h"
	#include "../../header/h675_hit_____/n675_100_hitTuke.h"

}




HitTuke::HitTuke()
{
};




int HitTuke::Evaluate(
	int		invColor,
	int		node,
	Liberty liberties[4],
	Board*	pBoard
	)
{
	int score = 0;
	int iDir;
	int adjNode;
	int adjColor;

	for (iDir = 0; iDir < 4; iDir++) {		// ��� �� �E�� �� ���� �� ����
		adjNode	 = node + pBoard->dir4[iDir];	// �א�(adjacent)�����_�ƁA
		adjColor = pBoard->table[adjNode];		// ���̐F

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