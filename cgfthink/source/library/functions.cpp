
extern "C" {

	#include "../../header/cgfthink.h"

	//--------------------------------------------------------------------------------
	// �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	extern int board[BOARD_MAX];

	// ���E�A�㉺�Ɉړ�����ꍇ�̓�����
	extern int dir4[4];


	// �΂�����
	void del_stone(int tz, int col)
	{
		int z, i;

		board[tz] = 0;
		for (i = 0; i < 4; i++) {
			z = tz + dir4[i];
			if (board[z] == col) del_stone(z, col);
		}
	}
}