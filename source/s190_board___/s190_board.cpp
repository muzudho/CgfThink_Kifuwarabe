//
// /source/s190_board/s190_board.cpp
//
extern "C" {

	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h190_board___/h190_board.h"


	//--------------------------------------------------------------------------------
	// �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	// �Տ�̐΂̐F�B
	int g_board[BOARD_MAX];

	// ���ɂ��̐΂����������ꍇ��1
	int g_checkedBoard[BOARD_MAX];

	// ��A�E�A���A���@�Ɉړ�����̂Ɏg�������l
	int g_dir4[4] = {
		-0x100,	// ��
		+0x001,	// �E
		+0x100,	// ��
		-0x001	// ��
	};// �I���W�i����cgfthink�ł͉E�A���A���A��̏��������B

	// �Ֆʂ̃T�C�Y�B19�H�Ղł�19�A9�H�Ղł�9
	int g_boardSize;

	// �͂�Ŏ��鑊��̐΂̐�(�ċA�֐��Ŏg��)
	int g_kakondaIshi = 0;

	// �A�̃��o�e�B�i�΂̌ċz�_�j�̐�(�ċA�֐��Ŏg��)
	int g_liberty = 0;

	// ���ɃR�E�ɂȂ�ʒu�B������� 0�B
	int g_kouNode = 0;

	// �n�}�B������΂̐��̂��ƁB[0]... ����������΂̐�, [1]...����������΂̐�
	int g_hama[2];


	//--------------------------------------------------------------------------------
	// �֐�
	//--------------------------------------------------------------------------------

	void CountLiberty(int tNode)
	{
		int i;

		g_liberty = g_kakondaIshi = 0;
		for (i = 0; i < BOARD_MAX; i++) {
			g_checkedBoard[i] = 0; 
		}
		CountLibertyElement(tNode, g_board[tNode]);
	}

	void CountLibertyElement(int tNode, int color)
	{
		int adjNode;
		int iDir;

		g_checkedBoard[tNode] = 1;						// ���̐΂͌����ς�	
		g_kakondaIshi++;								// �ċz�_�𐔂��Ă���i����������ƘA�́j
														// �΂̐�
		for (iDir = 0; iDir < 4; iDir++) {				// �אڂ���l����
			adjNode = tNode + g_dir4[iDir];
			if (g_checkedBoard[adjNode]) {
				continue;
			}
			if (g_board[adjNode] == 0) {				// ��_
				g_checkedBoard[adjNode] = 1;			// ���̋�_�͌����ς݂Ƃ���
				g_liberty++;							// ���o�e�B�̐�
			}
			if (g_board[adjNode] == color) {
				CountLibertyElement(adjNode, color);	// ���T���̎����̐�
			}
		}
	}

	int ConvertNode(int x, int y)
	{
		return (y + 1) * 256 + (x + 1);
	}

	void DeleteRenStones(
		int tNode,
		int color
	)
	{
		int adjNode;	// �㉺���E�ɗאڂ����_
		int i;

		// �w�肵���ʒu�̐΂��폜�B
		g_board[tNode] = 0;

		// �S�����̐΂ɂ������������s���܂��B
		for (i = 0; i < 4; i++) {
			adjNode = tNode + g_dir4[i];
			if (g_board[adjNode] == color) {
				DeleteRenStones(adjNode, color);
			}
		}
	}

}
