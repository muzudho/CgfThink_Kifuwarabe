//
// �R���s���[�^�[�͌�\�t�g�w���ӂ��ׁx�̎v�l�G���W��
//
// CgfGoban.exe�p�̎v�l���[�`���̃T���v��
//
// �w2005/06/04 - 2005/07/15 �R�� �G�x�ł����ɉ����B
// �����Ŏ��Ԃ������ł��B

// muzudho: ���O�o�͂̂��߂ɁB
#include <iostream>
#include <fstream>
using namespace std;


#include <windows.h> // �R���\�[���ւ̏o�͓�
#include <tchar.h> // Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB

#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h300_move____/n300_100_move.h"
#include "../../header/h390_explain_/n390_100_explain.h"
#include "../../header/h480_view____/n480_100_boardView.h"
#include "../../header/h490_endgame_/n490_100_endgame.h"
#include "../../header/h700_think___/n700_200_think.h"
//#include "../../header/h800_cgfthink/n800_100_cppBoard.h"
#include "../../header/h800_cgfthink/n800_200_cgfthink.h"



//--------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------

// �v�l���f�t���O�B0�ŏ���������Ă��܂��B
// GUI�́u�v�l���f�{�^���v�������ꂽ�ꍇ��1�ɂȂ�܂��B
Cgfthink g_cgfthink;


//--------------------------------------------------------------------------------
// �֐��錾�܂Ƃ߁@�ʂ̃A�v���P�[�V��������Ăяo��������
//--------------------------------------------------------------------------------

// �΋ǊJ�n���Ɉ�x�����Ă΂�܂��B
//extern "C"
DLL_EXPORT void cgfgui_thinking_init(
	int* pThinkStoped	// ���i��0�B���~�{�^���������ꂽ�Ƃ��� 1 �ɂȂ�܂��B���̒l��1�ɂȂ����ꍇ�͎v�l���I�����Ă��������B
)
{
	// ���O�F �����Ă��邱�Ƃ̊m�F�B
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"));
	outputfile << _T("called: cgfgui_thinking_init") << endl;

	//--------------------
	// �|�C���^�̎󂯓n��
	//--------------------
	g_cgfthink.pThinkStoped = pThinkStoped;

	// PRT()����\�����邽�߂̃R���\�[�����N������B
	AllocConsole();		// ���̍s���R�����g�A�E�g����΃R���\�[���͕\������܂���B
	SetConsoleTitle(_T("CgfgobanDLL Infomation Window"));

	// �R���\�[���ɏo�͂��邽�߂̃n���h��
	//HANDLE hConsoleWindow = INVALID_HANDLE_VALUE; //static HANDLE hOutput;
	HANDLE hConsoleWindow = GetStdHandle(STD_OUTPUT_HANDLE);
	Core::PRT(hConsoleWindow, _T("�f�o�b�O�p�̑��������i�O���O�j�@PRT()�֐��ŏo�͂ł���񂾂���\n"));

	// ���̉��ɁA�������̊m�ۂȂǕK�v�ȏꍇ�̃R�[�h���L�q���Ă��������B
}

// �v�l���[�`���B����1��̍��W��Ԃ��BPASS�̏ꍇ0�B
// GUI���猻�݂̋ǖʂ̏�񂪓n�����B
// �܂��A�I�Ǐ����̏ꍇ�́A�I�ǔ��f�̌��ʂ�Ԃ��B
//extern "C"
DLL_EXPORT int cgfgui_thinking(
	int		initBoard[]		,	// �����Ֆʁi�u��̏ꍇ�́A�����ɒu�΂�����j
	int		kifu[][3]		,	// ����	[�萔][0]...���W
								//		[�萔][1]...�΂̐F
								//		[�萔][2]...����ԁi�b)
								// �萔�� 0 ����n�܂�AcurTesuu ��1��O�܂ł���B
	int		curTesuu		,	// ���݂̎萔
	int		flgBlackTurn	,	// ����ԃt���O(����...1�A����...0)�B���������萔�ƈ���Ă�₱�����B
	int		boardSize		,	// �Ֆʂ̃T�C�Y
	double	komi			,	// �R�~
	int		endgameType		,	// 0...�ʏ�̎v�l�A1...�I�Ǐ����A2...�}�`��\���A3...���l��\���B
	int		endgameBoard[]		// �I�Ǐ����̌��ʂ�������B
)
{
	// �f�o�b�O�p�̑��ł��BPRT()�֐��ŏo�͂ł��܂��B
	HANDLE hConsoleWindow = GetStdHandle(STD_OUTPUT_HANDLE);

	// ���O�F �����Ă��邱�Ƃ̊m�F�B
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking") << endl;

	Core::PRT(hConsoleWindow, _T("cgfgui_thinking �J�n���I boardSize=%d \n"), boardSize);

	//--------------------
	// ���H��
	//--------------------
	Board* pBoard = new Board();
	pBoard->size = boardSize;

	// ���݋ǖʂ������Ə����Ֆʂ�����
	for (int iNode = 0; iNode < BOARD_MAX; iNode++) {
		pBoard->table[iNode] = initBoard[iNode];	// �����Ֆʂ��R�s�[
	}

	/*
	// [&hConsoleWindow]��t���Ă����ƁA�u���b�N�̊O���� hConsoleWindow �ϐ����Q�Ƃł��邺���i�O���O�j
	pBoard->ForeachAllNodesWithWaku( [&hConsoleWindow](int node) {
		int x, y;
		Board::ConvertToXy(x, y, node);
		Core::PRT(hConsoleWindow, _T("(^q^) %d =(%d %d) \n"), node, x, y);
	});
	// */


	//--------------------
	// ������
	//--------------------
	int node;			// �͌�Տ�̌�_�i�����Ղł����}�X�ځj
	int bestmoveNode;	// �R���s���[�^�[���ł�_�B
	int color;			// �΂̐F
	int time;			// �����
	int iTesuu;

	// �݌v�v�l���� [0]��� [1]���B�@�����Ƃ͌���Ȃ��H
	int thoughtTime[2] = { 0, 0 };

	// ������i�߂Ă�������
	for (iTesuu =0; iTesuu<curTesuu; iTesuu++) {
		node	= kifu[iTesuu][0];	// ���W�Ay*256 + x �̌`�œ����Ă���
		color	= kifu[iTesuu][1];	// �΂̐F
		time	= kifu[iTesuu][2];	// �����
		thoughtTime[iTesuu & 1] += time; // �萔�̉�1�������� [0]���A[1]���B
		if (Move::MoveOne(hConsoleWindow, node, color, pBoard) != MOVE_SUCCESS) {
			// �������Ȃ���΂����Ŏ~�߂�B�i�G���[���������H�H�j
			Core::PRT(hConsoleWindow, _T("������i�߂��Ȃ������̂Ŏ~�߂��� \n"));
			break;
		}
	}
	 
#if 0	// ���f����������ꍇ�̃T���v���B0��1�ɂ���΃R���p�C������܂��B
	for (i=0;i<300;i++) {				// 300*10ms = 3000ms = 3�b�҂��܂��B
		YieldWindowsSystem();			// �ꎞ�I��Windows�ɐ����n���܂��B
		if ( *g_pThinkStop != 0 ) break;	// ���f�{�^���������ꂽ�ꍇ�B
		Sleep(10);						// 10ms(0.01�b)��~�B
	}
#endif

	// ���[�h�ʑΉ�
	switch (endgameType)
	{
	// �u�I�Ǐ����v�Ȃ�
	case GAME_END_STATUS:
							return Endgame::EndgameStatus		(endgameBoard, pBoard);
	// �u�}�`��`���v�Ȃ�
	case GAME_DRAW_FIGURE:
							return Endgame::EndgameDrawFigure	(endgameBoard, pBoard);
	// �u���l�������v�Ȃ�
	case GAME_DRAW_NUMBER:	
							return Endgame::EndgameDrawNumber	(endgameBoard, pBoard);
	// �ʏ�̎w����
	default:				
							break;
	}

	//--------------------------------------------------------------------------------
	// �v�l���[�`��
	//--------------------------------------------------------------------------------

	if (flgBlackTurn) {
		color = BLACK;
	} else {
		color = WHITE;
	}
	// �P��w���܂��B
	bestmoveNode = Think::Bestmove(hConsoleWindow, color, pBoard);

	Core::PRT(hConsoleWindow, _T("�v�l���ԁF���=%d�b�A���=%d�b\n"), thoughtTime[0], thoughtTime[1]);
	Core::PRT(hConsoleWindow, _T("����=(%2d,%2d)(%04x), �萔=%d,���=%d,��size=%d,komi=%.1f\n"),(bestmoveNode&0xff),(bestmoveNode>>8),bestmoveNode, curTesuu,flgBlackTurn,boardSize,komi);
	//PrintBoard();

	//PRT(_T("a"));

	// FIXME: ���������[�N���Ă���͂����i�O���O�j
	//delete pBoard;

	//PRT(_T("b"));

	//int* pointer = new int;

	//PRT(_T("c"));

	//delete pointer;

	//PRT(_T("d"));

	return bestmoveNode;
}

// �΋ǏI�����Ɉ�x�����Ă΂�܂��B
// �������̉���Ȃǂ��K�v�ȏꍇ�ɂ����ɋL�q���Ă��������B
//extern "C"
DLL_EXPORT void cgfgui_thinking_close(
	void
)
{
	// ���O�F �����Ă��邱�Ƃ̊m�F�B
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking_close") << endl;

	FreeConsole();
	// ���̉��ɁA�������̉���ȂǕK�v�ȏꍇ�̃R�[�h���L�q���Ă��������B
}