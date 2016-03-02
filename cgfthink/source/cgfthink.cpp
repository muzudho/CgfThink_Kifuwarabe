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

	int board[BOARD_MAX];

	// ���E�A�㉺�Ɉړ�����ꍇ�̓�����
	int dir4[4] = { +0x001,-0x001,+0x100,-0x100 };

	// �v�l���f�t���O�B0�ŏ���������Ă��܂��B
	// GUI�́u�v�l���f�{�^���v�������ꂽ�ꍇ��1�ɂȂ�܂��B
	int* pThinkStop = NULL;


	// ���ɂ��̐΂����������ꍇ��1
	int check_board[BOARD_MAX];		

	// �Ֆʂ̃T�C�Y�B19�H�Ղł�19�A9�H�Ղł�9
	int board_size;	

	// ������΂̐�(�ċA�֐��Ŏg��)
	int ishi = 0;	

	// �A�̃_���̐�(�ċA�֐��Ŏg��)
	int dame = 0;	
	
	// ���ɃR�E�ɂȂ�ʒu
	int kou_z = 0;	

	// [0]... ����������΂̐�, [1]...����������΂̐�
	int hama[2];

	// �݌v�v�l����
	int sg_time[2];	

	// �R���\�[���ɏo�͂��邽�߂̃n���h��
	//static HANDLE hOutput = INVALID_HANDLE_VALUE;	
	HANDLE hOutput = INVALID_HANDLE_VALUE;

}//extern "C" {

// �΋ǊJ�n���Ɉ�x�����Ă΂�܂��B
DLL_EXPORT void cgfgui_thinking_init(int *ptr_stop_thinking)
{
	// muzudho: 
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"));
	outputfile << _T("called: cgfgui_thinking_init") << endl;

	// ���f�t���O�ւ̃|�C���^�ϐ��B
	// ���̒l��1�ɂȂ����ꍇ�͎v�l���I�����Ă��������B
	pThinkStop = ptr_stop_thinking;

	// PRT()����\�����邽�߂̃R���\�[�����N������B
	AllocConsole();		// ���̍s���R�����g�A�E�g����΃R���\�[���͕\������܂���B
	SetConsoleTitle(_T("CgfgobanDLL Infomation Window"));
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
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
	int dll_init_board[],	// �����Ֆ�
	int dll_kifu[][3],		// ����  [][0]...���W�A[][1]...�΂̐F�A[][2]...����ԁi�b)
	int dll_tesuu,			// �萔
	int dll_black_turn,		// ���(����...1�A����...0)
	int dll_board_size,		// �Ֆʂ̃T�C�Y
	double dll_komi,		// �R�~
	int dll_endgame_type,	// 0...�ʏ�̎v�l�A1...�I�Ǐ����A2...�}�`��\���A3...���l��\���B
	int dll_endgame_board[]	// �I�Ǐ����̌��ʂ�������B
)
{
	// muzudho: 
	ofstream outputfile(_T("muzudho_cgfthink_log.txt"), ios::app);
	outputfile << _T("called: cgfgui_thinking") << endl;

	int z,col,t,i,ret_z;

	// ���݋ǖʂ������Ə����Ֆʂ�����
	for (i=0;i<BOARD_MAX;i++) board[i] = dll_init_board[i];	// �����Ֆʂ��R�s�[
	board_size = dll_board_size;
	hama[0] = hama[1] = 0;
	sg_time[0] = sg_time[1] = 0;	// �݌v�v�l���Ԃ�������
	kou_z = 0;
		
	for (i=0;i<dll_tesuu;i++) {
		z   = dll_kifu[i][0];	// ���W�Ay*256 + x �̌`�œ����Ă���
		col = dll_kifu[i][1];	// �΂̐F
		t   = dll_kifu[i][2];	// �����
		sg_time[i&1] += t;
		if ( move_one(z,col) != MOVE_SUCCESS ) break;
	}
	 
#if 0	// ���f����������ꍇ�̃T���v���B0��1�ɂ���΃R���p�C������܂��B
	for (i=0;i<300;i++) {				// 300*10ms = 3000ms = 3�b�҂��܂��B
		PassWindowsSystem();			// �ꎞ�I��Windows�ɐ����n���܂��B
		if ( *pThinkStop != 0 ) break;	// ���f�{�^���������ꂽ�ꍇ�B
		Sleep(10);						// 10ms(0.01�b)��~�B
	}
#endif

	// �I�Ǐ����A�}�`�A���l��\������ꍇ
	if ( dll_endgame_type == GAME_END_STATUS  ) return endgame_status(dll_endgame_board);
	if ( dll_endgame_type == GAME_DRAW_FIGURE ) return endgame_draw_figure(dll_endgame_board);
	if ( dll_endgame_type == GAME_DRAW_NUMBER ) return endgame_draw_number(dll_endgame_board);

	// �T���v���̎v�l���[�`�����Ă�
	if ( dll_black_turn ) col = BLACK;
	else                  col = WHITE;
	ret_z = think_sample(col);

	PRT(_T("�v�l���ԁF���=%d�b�A���=%d�b\n"),sg_time[0],sg_time[1]);
	PRT(_T("����=(%2d,%2d)(%04x), �萔=%d,���=%d,��size=%d,komi=%.1f\n"),(ret_z&0xff),(ret_z>>8),ret_z, dll_tesuu,dll_black_turn,dll_board_size,dll_komi);
//	print_board();
	return ret_z;
}
