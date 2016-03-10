//
// /source/think.cpp
//

// muzudho: ���O�o�͂̂��߂ɁB
#include <iostream>
#include <fstream>
using namespace std;

extern "C" {

	#include <time.h>		// clock() ���g�p���邽�߂ɁB
	#include <windows.h>	// rand() �����g�p���邽�߂ɁB
	#include <tchar.h>		// Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB
	#include "../../header/h090_core/h090_core.h"
	#include "../../header/h300_move/h300_move.h"
	#include "../../header/h400_core/h400_board.h"
	#include "../../header/h400_core/h400_explain.h"
	#include "../../header/h400_core/h400_endgame.h"
	#include "../../header/h700_think/h700_think.h"


	//--------------------------------------------------------------------------------
	// �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	// �E�A���A���A��Ɉړ�����̂Ɏg�������l
	int g_dir4[4] = {
		+0x001,	// �E
		-0x001,	// ��
		+0x100,	// ��
		-0x100	// ��
	};

	// ���ɂ��̐΂����������ꍇ��1
	int g_checkedBoard[BOARD_MAX];

	// �͂�Ŏ��鑊��̐΂̐�(�ċA�֐��Ŏg��)
	int g_kakondaIshi = 0;

	// �A�̃��o�e�B�i�΂̌ċz�_�j�̐�(�ċA�֐��Ŏg��)
	int g_liberty = 0;

	// ���ɃR�E�ɂȂ�ʒu�B������� 0�B
	int g_kouNode = 0;

	// �n�}�B������΂̐��̂��ƁB[0]... ����������΂̐�, [1]...����������΂̐�
	int g_hama[2];

	// �R���\�[���ɏo�͂��邽�߂̃n���h��
	HANDLE g_hConsoleWindow = INVALID_HANDLE_VALUE; //static HANDLE hOutput;

	//--------------------------------------------------------------------------------
	// �֐�
	//--------------------------------------------------------------------------------

	// �����̓w�b�_�[�t�@�C�������Ă��������B
	int Bestmove(
		int color
	)
	{
		PRT(_T("Bestmove�J�n���I \n"));

		int bestmoveNode;
		int x;
		int y;
		int node;
		int i;
		int iDir;
		int maxScore;	// ���܂œǂ񂾎�ň�ԍ��������]���l
		int score;		// �ǂ�ł����̕]���l
		int flgCapture;	// �G�΂�������t���O
		int flgMove;	// �ړ����ʂ̎��
		int safe;
		int adjNode;	// �㉺���E��(adjacent)�̌�_
		int adjColor;	// �㉺���E��(adjacent)�̐΂̐F
		int invClr = INVCLR(color);//�������]

		PRT(_T("color=%d invClr=%d \n", color, invClr));


		// ���O�F �����Ă��邱�Ƃ̊m�F�B
		ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
		outputfile << _T("Bestmove: (^q^)") << endl;


		// ���s���邽�тɈႤ�l��������悤�Ɍ��݂̎����ŗ�����������
		srand((unsigned)clock());

		maxScore = -1;
		bestmoveNode = 0; // 0 �Ȃ�p�X�B

		PRT(_T("g_boardSize=%d \n", g_boardSize));

		for (y = 0; y < g_boardSize; y++) {
			for (x = 0; x < g_boardSize; x++) {
				node = ConvertNode(x, y);
				PRT(_T("node=%d \n"));

				if (g_board[node]) {
					// �΂����邩�A�g�Ȃ�
					PRT(_T("�΂����邩�A�g�B \n"));
					continue;
				}
				if (node == g_kouNode) {
					// �R�E�ɂȂ�ʒu�Ȃ�
					PRT(_T("�R�E�B \n"));
					continue;
				}

				score		= rand() % 100; // 0 �` 99 �̃����_���ȕ]���l��^����B
				flgCapture	= 0;
				safe		= 0;
				for (iDir = 0; iDir < 4; iDir++) {
					adjNode		= node + g_dir4[iDir];
					adjColor	= g_board[adjNode];
					if (adjColor == WAKU) {
						// �g�Ȃ�
						PRT(_T("�g�B \n"));
						safe++;
					}
					if (adjColor == 0 || adjColor == WAKU) {
						// ����ۂ��A�g�Ȃ�B
						PRT(_T("����ۂ��A�g�B \n"));
						continue;
					}

					// �ׂ̐΂̃��o�e�B�i�ċz�_�j�̐��𐔂��܂��B
					CountLiberty(adjNode);

					// �G�΂ŁA�ċz�_�̐��� 1 �Ȃ�A�����ɐ΂�u���Ǝ�邱�Ƃ��ł��܂��B
					if (adjColor == invClr && g_liberty == 1) {
						PRT(_T("�G�΂�������B \n"));
						flgCapture = 1; 	// �G�΂��A������t���O�B
					}

					// �����ɐ΂�u���Ă��A�ċz�_�� 1 �ȏ�c��i�����S�ȁj�����ɂȂ���܂��B
					if (adjColor == color && 2 <= g_liberty ) {
						safe++;
					}

					// �]���l�̌v�Z
					score +=
						(adjColor == invClr)		// �ׂ̐΂́A����̐΂Ȃ� 1�B
						* g_kakondaIshi				// �͂�Ŏ���΂̐��B
						* (10 / (g_liberty + 1));	// �A�̌ċz�_�̌���
													//		0 �Ȃ� 10�_
													//		1 �Ȃ� 5�_
													//		2 �Ȃ� 3.3333...�_
													//		3 �Ȃ� 2.5�_
													//		4 �Ȃ� 2�_
													//		...
					PRT(_T("�X�R�A=%d \n", score));
				}
				if (safe == 4) { // �l�����@�����̐΂�A�ǂɁ@�͂܂�Ă���ꏊ�i��j�ɂȂ�Ȃ�
					PRT(_T("��ɂ͑ł��Ȃ��B \n"));
					continue;	// ��ɂ͑ł��Ȃ��B
				}
				if (flgCapture == 0) {		// �΂����Ȃ��ꍇ
					// ���ۂɒu���Ă݂ā@���E�肩�ǂ�������
					int temp_kouNode	= g_kouNode;				// �R�E�̈ʒu��ޔ�
					flgMove				= MoveOne(node, color);		// �R�E�̈ʒu���ς�邩���B
					g_board[node]		= 0;
					g_kouNode			= temp_kouNode;
					if (flgMove == MOVE_SUICIDE) {	// ���E��Ȃ�
						PRT(_T("���E��͑ł��Ȃ��B \n"));
						continue;	// �x�X�g���[�u�ɂ͂Ȃ肦�Ȃ�
					}
				}

				// �x�X�g���[�u���X�V���܂��B
				// PRT("x,y=(%d,%d)=%d\n",x,y,value);
				if (maxScore < score) {
					maxScore = score;
					bestmoveNode = node;
				}
			}
		}


		return bestmoveNode;
		//return 1*256 + 1;
	}
}