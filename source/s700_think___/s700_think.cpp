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
	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h300_move____/h300_move.h"
	#include "../../header/h390_explain_/h390_explain.h"
	#include "../../header/h400_endgame_/h400_endgame.h"
	#include "../../header/h690_evaluate/h690_evaluation.h"
	#include "../../header/h700_think___/h700_think.h"


	//--------------------------------------------------------------------------------
	// �֐�
	//--------------------------------------------------------------------------------

	int Bestmove(
		int color
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

		for (y = 0; y < g_boardSize; y++) {
			for (x = 0; x < g_boardSize; x++) {
				node = ConvertNode(x, y);
				//PRT(_T("node=%d \n"));

				// ���̋ǖʂŁA�΂�u�����Ƃ��̕]���l
				int flgAbort = 0;
				score = Evaluate( flgAbort, color, node);
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