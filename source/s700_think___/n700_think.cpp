//
// /source/s700_think___/s700_think.cpp
//

// muzudho: ���O�o�͂̂��߂ɁB
#include <iostream>
#include <fstream>
using namespace std;

extern "C" {

	#include <time.h>		// clock() ���g�p���邽�߂ɁB
	#include <tchar.h>		// Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB
	#include "../../header/h090_core____/n090_100_core.h"
	#include "../../header/h190_board___/n190_100_board.h"
	#include "../../header/h300_move____/n300_100_move.h"
	#include "../../header/h390_explain_/n390_100_explain.h"
	#include "../../header/h490_endgame_/n490_100_endgame.h"
	#include "../../header/h690_evaluate/n690_100_evaluation.h"
	#include "../../header/h700_think___/n700_100_think.h"


	//--------------------------------------------------------------------------------
	// �֐�
	//--------------------------------------------------------------------------------

	int Bestmove(
		int		color,
		Board*	pBoard
	)
	{
		PRT(_T("Bestmove�J�n���I \n"));
		//PRT(_T("color=%d invClr=%d \n", color, invClr));

		int x;
		int y;
		int node;
		int score;		// �ǂ�ł����̕]���l
		int maxScore;	// ���܂œǂ񂾎�ň�ԍ��������]���l
		int bestmoveNode;

		// ���s���邽�тɈႤ�l��������悤�Ɍ��݂̎����ŗ�����������
		srand((unsigned)clock());

		//----------------------------------------
		// �΂�u���ʒu�P���A���̎�̕]���l���Z�o���܂��B
		//----------------------------------------
		maxScore = -1;
		bestmoveNode = 0; // 0 �Ȃ�p�X�B

		for (y = 0; y < pBoard->size; y++) {
			for (x = 0; x < pBoard->size; x++) {
				node = Board::ConvertToNode(x, y);
				//PRT(_T("node=%d \n"));

				// ���̋ǖʂŁA�΂�u�����Ƃ��̕]���l
				int flgAbort = 0;
				score = Evaluate( flgAbort, color, node, pBoard);
				if (flgAbort)
				{
					continue;
				}

				// �x�X�g���[�u���X�V���܂��B
				// PRT("x,y=(%d,%d)=%d\n",x,y,score);
				if (maxScore < score) {
					maxScore = score;
					bestmoveNode = node;
				}
			}
		}

		return bestmoveNode;
	}
}