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

extern "C" {

	#include <windows.h> // �R���\�[���ւ̏o�͓�
	#include <tchar.h> // Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB
	#include "../header/h090_core/h090_core.h"
	#include "../header/h300_move/h300_move.h"
	#include "../header/h400_core/h400_board.h"
	#include "../header/h400_core/h400_explain.h"
	#include "../header/h400_core/h400_endgame.h"
	#include "../header/cgfthink.h"
	#include "../header/think.h"

	//--------------------------------------------------------------------------------
	// �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	// �v�l���f�t���O�B0�ŏ���������Ă��܂��B
	// GUI�́u�v�l���f�{�^���v�������ꂽ�ꍇ��1�ɂȂ�܂��B
	int* g_pThinkStoped = NULL;

	// �݌v�v�l���� [0]��� [1]���B�@�����Ƃ͌���Ȃ��H
	int g_thoughtTime[2];

}//extern "C"

DLL_EXPORT void cgfgui_thinking_init(
	int* pThinkStoped
)
{
	// ���O�F �����Ă��邱�Ƃ̊m�F�B
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"));
	outputfile << _T("called: cgfgui_thinking_init") << endl;

	//--------------------
	// �|�C���^�̎󂯓n��
	//--------------------
	g_pThinkStoped = pThinkStoped;

	// PRT()����\�����邽�߂̃R���\�[�����N������B
	AllocConsole();		// ���̍s���R�����g�A�E�g����΃R���\�[���͕\������܂���B
	SetConsoleTitle(_T("CgfgobanDLL Infomation Window"));
	g_hConsoleWindow = GetStdHandle(STD_OUTPUT_HANDLE);
	PRT(_T("�f�o�b�O�p�̑��ł��BPRT()�֐��ŏo�͂ł��܂��B\n"));

	// ���̉��ɁA�������̊m�ۂȂǕK�v�ȏꍇ�̃R�[�h���L�q���Ă��������B
}

DLL_EXPORT int cgfgui_thinking(
	int		initBoard[]		,
	int		kifu[][3]		,
	int		curTesuu		,
	int		blackTurn		,
	int		boardSize		,
	double	komi			,
	int		endgameType		,
	int		endgameBoard[]
)
{
	// ���O�F �����Ă��邱�Ƃ̊m�F�B
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking") << endl;

	PRT(_T("cgfgui_thinking �J�n���I boardSize=%d \n", boardSize));

	int node;			// �͌�Տ�̌�_�i�����Ղł����}�X�ځj
	int bestmoveNode;	// �R���s���[�^�[���ł�_�B
	int color;			// �΂̐F
	int time;			// �����
	int i;
	int iNode;
	int iTesuu;

	// ���݋ǖʂ������Ə����Ֆʂ�����
	for (iNode = 0; iNode < BOARD_MAX; iNode++) {
		g_board[iNode] = initBoard[iNode];	// �����Ֆʂ��R�s�[
	}

	//--------------------
	// ���H��
	//--------------------
	g_boardSize				= boardSize;

	//--------------------
	// ������
	//--------------------
	g_hama[BLACK]			= 0;	// ������΂̐�
	g_hama[WHITE]			= 0;
	g_thoughtTime[BLACK]	= 0;	// �݌v�v�l����
	g_thoughtTime[WHITE]	= 0;	
	g_kouNode				= 0;	// �R�E�ɂȂ�ʒu�B

	// ������i�߂Ă�������
	for (iTesuu =0; iTesuu<curTesuu; iTesuu++) {
		node	= kifu[iTesuu][0];	// ���W�Ay*256 + x �̌`�œ����Ă���
		color	= kifu[iTesuu][1];	// �΂̐F
		time	= kifu[iTesuu][2];	// �����
		g_thoughtTime[iTesuu & 1] += time; // �萔�̉�1�������� [0]���A[1]���B
		if (MoveOne(node, color) != MOVE_SUCCESS) {
			// �������Ȃ���΂����Ŏ~�߂�B
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
							return EndgameStatus		(endgameBoard);
	// �u�}�`��`���v�Ȃ�
	case GAME_DRAW_FIGURE:
							return EndgameDrawFigure	(endgameBoard);
	// �u���l�������v�Ȃ�
	case GAME_DRAW_NUMBER:	
							return EndgameDrawNumber	(endgameBoard);
	// �ʏ�̎w����
	default:				
							break;
	}

	//--------------------------------------------------------------------------------
	// �v�l���[�`��
	//--------------------------------------------------------------------------------

	if (blackTurn) {
		color = BLACK;
	} else {
		color = WHITE;
	}
	// �P��w���܂��B
	bestmoveNode = Bestmove(color);

	PRT(_T("�v�l���ԁF���=%d�b�A���=%d�b\n"), g_thoughtTime[0], g_thoughtTime[1]);
	PRT(_T("����=(%2d,%2d)(%04x), �萔=%d,���=%d,��size=%d,komi=%.1f\n"),(bestmoveNode&0xff),(bestmoveNode>>8),bestmoveNode, curTesuu,blackTurn,boardSize,komi);
//	PrintBoard();
	return bestmoveNode;
}

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