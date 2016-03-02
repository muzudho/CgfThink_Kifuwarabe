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
	#include "../header/cgfthink.h"
	#include "../header/library/functions.h"

	//--------------------------------------------------------------------------------
	// �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	// �v�l���f�t���O�B0�ŏ���������Ă��܂��B
	// GUI�́u�v�l���f�{�^���v�������ꂽ�ꍇ��1�ɂȂ�܂��B
	int* g_pThinkStop = NULL;

	// �݌v�v�l���� [0]��� [1]���
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
	extern int g_board[BOARD_MAX];

	// ���E�A�㉺�Ɉړ�����ꍇ�̓�����
	extern int g_dir4[4];

	// ���ɂ��̐΂����������ꍇ��1
	extern int g_checkedBoard[BOARD_MAX];

	// �Ֆʂ̃T�C�Y�B19�H�Ղł�19�A9�H�Ղł�9
	extern int g_boardSize;

	// ������΂̐�(�ċA�֐��Ŏg��)
	extern int g_ishi;

	// �A�̃_���̐�(�ċA�֐��Ŏg��)
	extern int g_dame;

	// ���ɃR�E�ɂȂ�ʒu
	extern int g_kouZ;

	// [0]... ����������΂̐�, [1]...����������΂̐�
	extern int g_hama[2];

	// �R���\�[���ɏo�͂��邽�߂̃n���h��
	//static extern HANDLE hOutput;
	extern HANDLE g_hConsoleWindow;

}//extern "C" {

// �΋ǊJ�n���Ɉ�x�����Ă΂�܂��B
DLL_EXPORT void cgfgui_thinking_init( int* ptr_stop_thinking)
{
	// muzudho: 
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"));
	outputfile << _T("called: cgfgui_thinking_init") << endl;

	// ���f�t���O�ւ̃|�C���^�ϐ��B
	// ���̒l��1�ɂȂ����ꍇ�͎v�l���I�����Ă��������B
	g_pThinkStop = ptr_stop_thinking;

	// PRT()����\�����邽�߂̃R���\�[�����N������B
	AllocConsole();		// ���̍s���R�����g�A�E�g����΃R���\�[���͕\������܂���B
	SetConsoleTitle(_T("CgfgobanDLL Infomation Window"));
	g_hConsoleWindow = GetStdHandle(STD_OUTPUT_HANDLE);
	PRT(_T("�f�o�b�O�p�̑��ł��BPRT()�֐��ŏo�͂ł��܂��B\n"));

	// ���̉��ɁA�������̊m�ۂȂǕK�v�ȏꍇ�̃R�[�h���L�q���Ă��������B
}


// �΋ǏI�����Ɉ�x�����Ă΂�܂��B
// �������̉���Ȃǂ��K�v�ȏꍇ�ɂ����ɋL�q���Ă��������B
DLL_EXPORT void cgfgui_thinking_close(void)
{
	// muzudho: 
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking_close") << endl;

	FreeConsole();
	// ���̉��ɁA�������̉���ȂǕK�v�ȏꍇ�̃R�[�h���L�q���Ă��������B
}

// �v�l���[�`���B����1���Ԃ��B
// �{�̂��珉���ՖʁA�����A�萔�A��ԁA�Ղ̃T�C�Y�A�R�~�A����������ԂŌĂ΂��B
DLL_EXPORT int cgfgui_thinking(
	int initBoard[],	// �����Ֆ�
	int kifu[][3],		// ����  [][0]...���W�A[][1]...�΂̐F�A[][2]...����ԁi�b)
	int tesuu,			// �萔
	int blackTurn,		// ���(����...1�A����...0)
	int boardSize,		// �Ֆʂ̃T�C�Y
	double komi,		// �R�~
	int endgameType,	// 0...�ʏ�̎v�l�A1...�I�Ǐ����A2...�}�`��\���A3...���l��\���B
	int endgameBoard[]	// �I�Ǐ����̌��ʂ�������B
)
{
	// muzudho: 
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking") << endl;

	int z,col,t,i,ret_z;

	// ���݋ǖʂ������Ə����Ֆʂ�����
	for (i = 0; i < BOARD_MAX; i++) {
		g_board[i] = initBoard[i];	// �����Ֆʂ��R�s�[
	}
	g_boardSize = boardSize;
	g_hama[0] = g_hama[1] = 0;
	g_thoughtTime[0] = g_thoughtTime[1] = 0;	// �݌v�v�l���Ԃ�������
	g_kouZ = 0;

	// ������i�߂Ă�������
	for (i=0;i<tesuu;i++) {
		z   = kifu[i][0];	// ���W�Ay*256 + x �̌`�œ����Ă���
		col = kifu[i][1];	// �΂̐F
		t   = kifu[i][2];	// �����
		g_thoughtTime[i&1] += t;
		if (move_one(z, col) != MOVE_SUCCESS) {
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
		col = BLACK;
	} else {
		col = WHITE;
	}
	ret_z = think_sample(col);

	PRT(_T("�v�l���ԁF���=%d�b�A���=%d�b\n"), g_thoughtTime[0], g_thoughtTime[1]);
	PRT(_T("����=(%2d,%2d)(%04x), �萔=%d,���=%d,��size=%d,komi=%.1f\n"),(ret_z&0xff),(ret_z>>8),ret_z, tesuu,blackTurn,boardSize,komi);
//	print_board();
	return ret_z;
}
