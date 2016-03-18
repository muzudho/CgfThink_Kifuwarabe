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
	Core			core			,
	int				color			,
	Board*			pBoard			,
	LibertyOfNodes*	pLibertyOfNodes
)
{
#ifdef CHECK_LOG
	core.PRT( _T("Bestmove�J�n���I \n"));
#endif

	int maxScore;	// ���܂œǂ񂾎�ň�ԍ��������]���l
	int bestmoveNode;

	// ���s���邽�тɈႤ�l��������悤�Ɍ��݂̎����ŗ�����������
	srand((unsigned)clock());

	//----------------------------------------
	// �΂�u���ʒu�P���A���̎�̕]���l���Z�o���܂��B
	//----------------------------------------
	maxScore = -1;
	bestmoveNode = 0; // 0 �Ȃ�p�X�B

	pBoard->ForeachAllNodesWithoutWaku([color,&maxScore,&bestmoveNode,&pBoard,&pLibertyOfNodes, &core](int node, bool& isBreak) {
		//{
			//int x, y;
			//AbstractBoard::ConvertToXy(x, y, node);
			//core.PRT(_T("#(%d,%d) "), x, y);
		//}

		// ���̋ǖʂŁA�΂�u�����Ƃ��̕]���l
		int flgAbort = 0;
		int score;		// �ǂ�ł����̕]���l
		score = Evaluation::EvaluateAtNode(core, flgAbort, color, node, pBoard, pLibertyOfNodes);
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

	return bestmoveNode;
}
