//
// /source/s675_hit_____/s675_100_hitTuke.cpp
//

extern "C" {

	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h675_hit_____/h675_100_hitTuke.h"

}




HitTuke::HitTuke()
{
	this->unuse = 0;
};




int HitTuke::Evaluate_AdjNode(
	int invColor,
	int adjColor
	)
{
	int score = 0;

	// �]���l�̌v�Z�i�S�������J��Ԃ��j
	score +=
		(adjColor == invColor)		// �ׂ̐�
									//		����̐�: 1
									//		����ȊO: 0
									//   �~
		* g_renIshi					// �A�̐΂̐�
									//   �~
		* (10 / (g_liberty + 1));	// �A�̌ċz�_�̌�
									//		0��: 10�_
									//		1��:  5�_
									//		2��:  3.3333...�_
									//		3��:  2.5�_
									//		4��:  2�_
									//		...

	return score;
}