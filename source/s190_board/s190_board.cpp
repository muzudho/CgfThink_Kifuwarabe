//
// /source/s190_board/s190_board.cpp
//
extern "C" {

	#include "../../header/h090_core/h090_core.h"
	#include "../../header/h190_board/h190_board.h"


	//--------------------------------------------------------------------------------
	// �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	int g_board[BOARD_MAX];
	int g_boardSize;


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
		int i;

		g_checkedBoard[tNode] = 1;				// ���̐΂͌����ς�	
		g_kakondaIshi++;								// ���鑊��̐΂̐�
		for (i = 0; i < 4; i++) {
			adjNode = tNode + g_dir4[i];
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

	void DeleteStone(
		int tNode,
		int color
	)
	{
		int adjNode;	// �㉺���E�ɗאڂ����_
		int i;

		g_board[tNode] = 0;
		for (i = 0; i < 4; i++) {
			adjNode = tNode + g_dir4[i];
			if (g_board[adjNode] == color) {
				DeleteStone(adjNode, color);
			}
		}
	}

}
