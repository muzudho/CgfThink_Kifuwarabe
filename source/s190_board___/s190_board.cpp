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

	// ��A�E�A���A���@�Ɉړ�����̂Ɏg�������l
	int g_dir4[4] = {
		-0x100,	// ��
		+0x001,	// �E
		+0x100,	// ��
		-0x001	// ��
	};// �I���W�i����cgfthink�ł͉E�A���A���A��̏��������B

	// �Ֆʂ̃T�C�Y�B19�H�Ղł�19�A9�H�Ղł�9
	int g_boardSize;

	// ���ɃR�E�ɂȂ�ʒu�B������� 0�B
	int g_kouNode = 0;

	// �n�}�B������΂̐��̂��ƁB[0]... ����������΂̐�, [1]...����������΂̐�
	int g_hama[2];


	//--------------------------------------------------------------------------------
	// �֐�
	//--------------------------------------------------------------------------------

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
