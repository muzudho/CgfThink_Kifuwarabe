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
	#include "../../header/h090_core____/n090_100_core.h"
	#include "../../header/h190_board___/n190_100_board.h"
	#include "../../header/h300_move____/n300_100_move.h"
	#include "../../header/h390_explain_/n390_100_explain.h"
	#include "../../header/h480_view____/n480_100_board.h"
	#include "../../header/h490_endgame_/n490_100_endgame.h"
	#include "../../header/h700_think___/n700_100_think.h"
	#include "../../header/h800_cgfthink/n800_100_cgfthink.h"

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

	PRT(_T("cgfgui_thinking �J�n���I boardSize=%d \n"), boardSize);

	/* for debug
	int tnode, tx, ty;
	for (int x = 0; x < 9; x++)
	{
		for (int y = 0; y < 9; y++)
		{
			tnode = ConvertToNode(x, y);
			ConvertToXy(tx, ty, tnode);
			PRT(_T("(%d,%d)= %d =(%d %d) \n"), x, y, tnode, tx, ty);
		}
	}
	// */

	int node;			// �͌�Տ�̌�_�i�����Ղł����}�X�ځj
	int bestmoveNode;	// �R���s���[�^�[���ł�_�B
	int color;			// �΂̐F
	int time;			// �����
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
		if (MoveOne(node, color, g_board) != MOVE_SUCCESS) {
			// �������Ȃ���΂����Ŏ~�߂�B�i�G���[���������H�H�j
			PRT(_T("������i�߂��Ȃ������̂Ŏ~�߂��� \n"));
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
							return EndgameStatus		(endgameBoard, g_board, g_boardSize );
	// �u�}�`��`���v�Ȃ�
	case GAME_DRAW_FIGURE:
							return EndgameDrawFigure	(endgameBoard, g_boardSize);
	// �u���l�������v�Ȃ�
	case GAME_DRAW_NUMBER:	
							return EndgameDrawNumber	(endgameBoard, g_boardSize);
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
	bestmoveNode = Bestmove(color, g_board, g_boardSize);

	PRT(_T("�v�l���ԁF���=%d�b�A���=%d�b\n"), g_thoughtTime[0], g_thoughtTime[1]);
	PRT(_T("����=(%2d,%2d)(%04x), �萔=%d,���=%d,��size=%d,komi=%.1f\n"),(bestmoveNode&0xff),(bestmoveNode>>8),bestmoveNode, curTesuu,blackTurn,boardSize,komi);
	//PrintBoard();
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