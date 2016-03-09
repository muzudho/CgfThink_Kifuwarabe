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
	#include "../header/core/core_ui.h"
	#include "../header/core/core_board.h"
	#include "../header/core/core_explain.h"
	#include "../header/core/core_endgame.h"
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


	//--------------------------------------------------------------------------------
	// �O���O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	//
	// extern �Ƃ́A�ϐ���֐����@�ʂ̃t�@�C���Ŋ��� ��`���Ă���Ƃ��ɁA
	// �������̃t�@�C���Œ�`���Ă���̂ł�����g���܂��A�Ƃ����Ӗ��Œ�`�̓��ɕt����A
	// ���܂��Ȃ��������I
	// �R���p�C�����ǂ݂ɍs���t�@�C���̂ǂ����� 1�� extern �̂��Ă��Ȃ���`��
	// �����OK�Ȃ񂾂����I ������`���Ă���܂��A�Ƃ����R���p�C���E�G���[��h���邺���I
	//

	// ������ header/functions.cpp ���Q�Ƃ��Ă��������B
	extern int g_board[BOARD_MAX];
	extern int g_dir4[4];
	extern int g_checkedBoard[BOARD_MAX];
	extern int g_boardSize;
	extern int g_ishi;
	extern int g_dame;
	extern int g_kouNode;
	extern int g_hama[2];
	extern HANDLE g_hConsoleWindow;

}//extern "C"

// ������ cgfthink.h �̊֐��v���g�^�C�v�錾���Q�Ƃ��Ă��������B
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

// ������ cgfthink.h �̊֐��v���g�^�C�v�錾���Q�Ƃ��Ă��������B
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
	g_hama	     [BLACK]	= g_hama	   [WHITE]	= 0;
	g_thoughtTime[BLACK]	= g_thoughtTime[WHITE]	= 0;	// �݌v�v�l���Ԃ�������
	g_kouNode				= 0;

	// ������i�߂Ă�������
	for (iTesuu =0; iTesuu<curTesuu; iTesuu++) {
		node   = kifu[iTesuu][0];	// ���W�Ay*256 + x �̌`�œ����Ă���
		color = kifu[iTesuu][1];	// �΂̐F
		time   = kifu[iTesuu][2];	// �����
		g_thoughtTime[iTesuu & 1] += time;
		if (move_one(node, color) != MOVE_SUCCESS) {
			break;
		}
	}
	 
#if 0	// ���f����������ꍇ�̃T���v���B0��1�ɂ���΃R���p�C������܂��B
	for (i=0;i<300;i++) {				// 300*10ms = 3000ms = 3�b�҂��܂��B
		PassWindowsSystem();			// �ꎞ�I��Windows�ɐ����n���܂��B
		if ( *g_pThinkStop != 0 ) break;	// ���f�{�^���������ꂽ�ꍇ�B
		Sleep(10);						// 10ms(0.01�b)��~�B
	}
#endif

	// �I�Ǐ����A�}�`�A���l��\������ꍇ
	switch (endgameType)
	{
	case GAME_END_STATUS:
		return endgame_status(endgameBoard);
	case GAME_DRAW_FIGURE:
		return endgame_draw_figure(endgameBoard);
	case GAME_DRAW_NUMBER:
		return endgame_draw_number(endgameBoard);
	default:
		break;
	}

	//--------------------------------------------------------------------------------
	// �v�l���[�`��
	//--------------------------------------------------------------------------------

	// �����_���ɂP��w���܂��B
	if (blackTurn) {
		color = BLACK;
	} else {
		color = WHITE;
	}
	bestmoveNode = think_sample(color);

	PRT(_T("�v�l���ԁF���=%d�b�A���=%d�b\n"), g_thoughtTime[0], g_thoughtTime[1]);
	PRT(_T("����=(%2d,%2d)(%04x), �萔=%d,���=%d,��size=%d,komi=%.1f\n"),(bestmoveNode&0xff),(bestmoveNode>>8),bestmoveNode, curTesuu,blackTurn,boardSize,komi);
//	print_board();
	return bestmoveNode;
}

// �΋ǏI�����Ɉ�x�����Ă΂�܂��B
// �������̉���Ȃǂ��K�v�ȏꍇ�ɂ����ɋL�q���Ă��������B
DLL_EXPORT void cgfgui_thinking_close(void)
{
	// ���O�F �����Ă��邱�Ƃ̊m�F�B
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking_close") << endl;

	FreeConsole();
	// ���̉��ɁA�������̉���ȂǕK�v�ȏꍇ�̃R�[�h���L�q���Ă��������B
}