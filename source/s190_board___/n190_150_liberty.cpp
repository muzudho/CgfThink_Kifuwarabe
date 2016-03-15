//
// /source/s600_research/s600_100_adjLiberty.cpp
//

#include "../../header/h190_board___/n190_150_liberty.h"




Liberty::Liberty()
{
	//this->checkedBoard[BOARD_MAX];
	this->liberty = 0;
	this->renIshi = 0;
};




void Liberty::Count(int node)
{
	int thisColor;	// �㉺���E��(adjacent)�̐΂̐F

	thisColor = g_board[node];		// ���̐F

									// ��ɑł����܂Ȃ����A���̒��ɑł����܂Ȃ����A�̏����̂��Ƃ�
	if (thisColor == 0 || thisColor == WAKU) {
		// ����ۂ��A�g�Ȃ�B
		//PRT(_T("����ۂ��A�g�B \n"));
		goto gt_EndMethod;
	}

	int i;

	this->liberty = 0;
	this->renIshi = 0;
	for (i = 0; i < BOARD_MAX; i++) {
		this->checkedBoard[i] = 0;
	}

	this->CountElement(node, g_board[node]);

gt_EndMethod:
	return;
}




void Liberty::CountElement(int tNode, int color)
{
	int adjNode;
	int iDir;

	this->checkedBoard[tNode] = 1;					// ���̐΂͌����ς�	
	this->renIshi++;								// �ċz�_�𐔂��Ă���i�P�܂��͘A�́j
													// �΂̐�
	for (iDir = 0; iDir < 4; iDir++) {				// �אڂ���l����
		adjNode = tNode + g_dir4[iDir];
		if (this->checkedBoard[adjNode]) {
			continue;
		}
		if (g_board[adjNode] == 0) {				// ��_
			this->checkedBoard[adjNode] = 1;			// ���̋�_�͌����ς݂Ƃ���
			this->liberty++;							// ���o�e�B�̐�
		}
		if (g_board[adjNode] == color) {
			this->CountElement(adjNode, color);	// ���T���̎����̐�
		}
	}
}
