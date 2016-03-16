// muzudho: ���O�o�͂̂��߂ɁB
#include <iostream>
#include <fstream>
using namespace std;

#include <time.h>		// clock() ���g�p���邽�߂ɁB
#include <tchar.h>		// Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB
#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_200_libertyOfNodes.h"
#include "../../header/h300_move____/n300_100_move.h"
#include "../../header/h390_explain_/n390_100_explain.h"
#include "../../header/h490_endgame_/n490_100_endgame.h"
#include "../../header/h700_think___/n700_100_evaluation.h"
#include "../../header/h700_think___/n700_200_think.h"


int Think::Bestmove(
	HANDLE			hConsoleWindow	,
	int				color			,
	Board*			pBoard			,
	LibertyOfNodes*	pLibertyOfNodes
)
{
	Core::PRT(hConsoleWindow, _T("Bestmove�J�n���I \n"));

	int maxScore;	// ���܂œǂ񂾎�ň�ԍ��������]���l
	int bestmoveNode;

	// ���s���邽�тɈႤ�l��������悤�Ɍ��݂̎����ŗ�����������
	srand((unsigned)clock());

	//----------------------------------------
	// �΂�u���ʒu�P���A���̎�̕]���l���Z�o���܂��B
	//----------------------------------------
	maxScore = -1;
	bestmoveNode = 0; // 0 �Ȃ�p�X�B

	Core::PRT(hConsoleWindow, _T("aa \n"));

	pBoard->ForeachAllNodesWithoutWaku([color,&maxScore,&bestmoveNode,&pBoard,&pLibertyOfNodes, &hConsoleWindow](int node, bool& isBreak) {
		//PRT(_T("node=%d \n"));

		Core::PRT(hConsoleWindow, _T("bb \n"));

		// ���̋ǖʂŁA�΂�u�����Ƃ��̕]���l
		int flgAbort = 0;
		int score;		// �ǂ�ł����̕]���l
		score = Evaluation::Evaluate(hConsoleWindow, flgAbort, color, node, pBoard, pLibertyOfNodes);
		if (flgAbort)
		{
			goto gt_Next;
		}

		// �x�X�g���[�u���X�V���܂��B
		// PRT("x,y=(%d,%d)=%d\n",x,y,score);
		if (maxScore < score) {
			maxScore = score;
			bestmoveNode = node;
		}
	gt_Next:
		;
	});

	Core::PRT(hConsoleWindow, _T("cc \n"));

	return bestmoveNode;
}
