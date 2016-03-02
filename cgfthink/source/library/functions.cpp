
extern "C" {

	#include <time.h>		// clock() ���g�p���邽�߂ɁB
	#include <windows.h>	// rand() �����g�p���邽�߂ɁB
	#include <tchar.h> // Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB
	#include "../../header/cgfthink.h"

	//--------------------------------------------------------------------------------
	// �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	//
	// extern �Ƃ́A�ϐ���֐����@�ʂ̃t�@�C���Ŋ��� ��`���Ă���Ƃ��ɁA
	// �������̃t�@�C���Œ�`���Ă���̂ł�����g���܂��A�Ƃ����Ӗ��Œ�`�̓��ɕt����A
	// ���܂��Ȃ��������I
	// �R���p�C�����ǂ݂ɍs���t�@�C���̂ǂ����� 1�� extern �̂��Ă��Ȃ���`��
	// �����OK�Ȃ񂾂����I ������`���Ă���܂��A�Ƃ����R���p�C���E�G���[��h���邺���I
	//
	extern int board[BOARD_MAX];

	// ���E�A�㉺�Ɉړ�����ꍇ�̓�����
	extern int dir4[4];

	// ���ɂ��̐΂����������ꍇ��1
	extern int check_board[BOARD_MAX];

	// �Ֆʂ̃T�C�Y�B19�H�Ղł�19�A9�H�Ղł�9
	extern int board_size;

	// ������΂̐�(�ċA�֐��Ŏg��)
	extern int ishi;

	// �A�̃_���̐�(�ċA�֐��Ŏg��)
	extern int dame;

	// ���ɃR�E�ɂȂ�ʒu
	extern int kou_z;

	// [0]... ����������΂̐�, [1]...����������΂̐�
	extern int hama[2];

	//--------------------------------------------------------------------------------
	// �֐���`
	//--------------------------------------------------------------------------------

	// �ꎞ�I��Windows�ɐ����n���܂��B
	// �v�l���ɂ��̊֐����ĂԂƎv�l���f�{�^�����L���ɂȂ�܂��B
	// ���b30��ȏ�Ă΂��悤�ɂ���ƃX���[�Y�ɒ��f�ł��܂��B
	void PassWindowsSystem(void)
	{
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);						// keyboard input.
			DispatchMessage(&msg);
		}
	}

	#define PRT_LEN_MAX 256			// �ő�256�����܂ŏo�͉�

	// �R���\�[���ɏo�͂��邽�߂̃n���h��
	//static extern HANDLE hOutput;
	extern HANDLE hOutput;


	// printf()�̑�p�֐��B
	void PRT(const _TCHAR* fmt, ...)//const char *fmt
	{
		va_list ap;
		int len;
		//static char text[PRT_LEN_MAX];
		static _TCHAR text[PRT_LEN_MAX];
		DWORD nw;

		if (hOutput == INVALID_HANDLE_VALUE) return;
		va_start(ap, fmt);
		//len = _vsnprintf(text, PRT_LEN_MAX - 1, fmt, ap);
		len = _vsnwprintf(text, PRT_LEN_MAX - 1, fmt, ap);
		va_end(ap);

		if (len < 0 || len >= PRT_LEN_MAX) return;
		//WriteConsole(hOutput, text, (DWORD)strlen(text), &nw, NULL);
		WriteConsole(hOutput, text, (DWORD)wcslen(text), &nw, NULL);
	}

	// �����ɋ߂��]���֐��B�����΂����ɍs���悤�ɁB
	int think_sample(int col)
	{
		int max, ret_z;
		int x, y, z, i, value, capture, z1, flag, safe, k;
		int un_col = UNCOL(col);

		// ���s���邽�тɈႤ�l��������悤�Ɍ��݂̎����ŗ�����������
		srand((unsigned)clock());

		max = -1;
		ret_z = 0;
		for (y = 0; y<board_size; y++) for (x = 0; x<board_size; x++) {
			z = get_z(x, y);
			if (board[z]) continue;
			if (z == kou_z) continue;	// �R�E

			value = rand() % 100;
			capture = safe = 0;
			for (i = 0; i<4; i++) {
				z1 = z + dir4[i];
				k = board[z1];
				if (k == WAKU) safe++;
				if (k == 0 || k == WAKU) continue;
				count_dame(z1);
				if (k == un_col && dame == 1) capture = 1;	// �G�΂�����
				if (k == col && dame >= 2) safe++;			// ���S�Ȗ����Ɍq����
				value += (k == un_col) * ishi * (10 / (dame + 1));
			}
			if (safe == 4) continue;	// ��ɂ͑ł��Ȃ��B
			if (capture == 0) {		// �΂����Ȃ��ꍇ�͎��ۂɒu���Ă݂Ď��E�肩�ǂ�������
				int kz = kou_z;			// �R�E�̈ʒu��ޔ�
				flag = move_one(z, col);
				board[z] = 0;
				kou_z = kz;
				if (flag == MOVE_SUICIDE) continue;	// ���E��
			}
			//		PRT("x,y=(%d,%d)=%d\n",x,y,value);
			if (value > max) { max = value; ret_z = z; }
		}
		return ret_z;
	}

	// ���݂̔Ֆʂ�\��
	void print_board(void)
	{
		int x, y, z;
		_TCHAR* str[4] = { _T("�E"), _T("��"), _T("��"), _T("�{") };

		for (y = 0; y<board_size + 2; y++) for (x = 0; x<board_size + 2; x++) {
			z = (y + 0) * 256 + (x + 0);
			PRT(_T("%s"), str[board[z]]);
			if (x == board_size + 1) PRT(_T("\n"));
		}
	}

	// �I�Ǐ����i�T���v���ł͊ȒP�Ȕ��f�Ŏ��΂ƒn�̔�������Ă��܂��j
	int endgame_status(int endgame_board[])
	{
		int x, y, z, sum, i, k;
		int *p;

		for (y = 0; y<board_size; y++) for (x = 0; x<board_size; x++) {
			z = get_z(x, y);
			p = endgame_board + z;
			if (board[z] == 0) {
				*p = GTP_DAME;
				sum = 0;
				for (i = 0; i<4; i++) {
					k = board[z + dir4[i]];
					if (k == WAKU) continue;
					sum |= k;
				}
				if (sum == BLACK) *p = GTP_BLACK_TERRITORY;
				if (sum == WHITE) *p = GTP_WHITE_TERRITORY;
			}
			else {
				*p = GTP_ALIVE;
				count_dame(z);
				//			PRT("(%2d,%2d),ishi=%2d,dame=%2d\n",z&0xff,z>>8,ishi,dame);
				if (dame <= 1) *p = GTP_DEAD;
			}
		}
		return 0;
	}

	// �}�`��`��
	int endgame_draw_figure(int endgame_board[])
	{
		int x, y, z;
		int *p;

		for (y = 0; y<board_size; y++) for (x = 0; x<board_size; x++) {
			z = get_z(x, y);
			p = endgame_board + z;
			if ((rand() % 2) == 0) *p = FIGURE_NONE;
			else {
				if (rand() % 2) *p = FIGURE_BLACK;
				else              *p = FIGURE_WHITE;
				*p |= (rand() % 9) + 1;
			}
		}
		return 0;
	}

	// ���l������(0�͕\������Ȃ�)
	int endgame_draw_number(int endgame_board[])
	{
		int x, y, z;
		int *p;

		for (y = 0; y<board_size; y++) for (x = 0; x<board_size; x++) {
			z = get_z(x, y);
			p = endgame_board + z;
			*p = (rand() % 110) - 55;
		}
		return 0;
	}

	// (x,y)��1�̍��W�ɕϊ�
	int get_z(int x, int y)
	{
		return (y + 1) * 256 + (x + 1);
	}

	// �ʒu tz �ɂ�����_���̐��Ɛ΂̐����v�Z�B���ʂ̓O���[�o���ϐ��ɁB
	void count_dame(int tz)
	{
		int i;

		dame = ishi = 0;
		for (i = 0; i<BOARD_MAX; i++) check_board[i] = 0;
		count_dame_sub(tz, board[tz]);
	}

	// �_���Ɛ΂̐�����ċA�֐�
	// 4�����𒲂ׂāA�󔒂�������+1�A�����̐΂Ȃ�ċA�ŁB����̐΁A�ǂȂ炻�̂܂܁B
	void count_dame_sub(int tz, int col)
	{
		int z, i;

		check_board[tz] = 1;			// ���̐΂͌����ς�	
		ishi++;							// �΂̐�
		for (i = 0; i<4; i++) {
			z = tz + dir4[i];
			if (check_board[z]) continue;
			if (board[z] == 0) {
				check_board[z] = 1;	// ���̋�_�͌����ς�
				dame++;				// �_���̐�
			}
			if (board[z] == col) count_dame_sub(z, col);	// ���T���̎����̐�
		}
	}

	// �΂�����
	void del_stone(int tz, int col)
	{
		int z, i;

		board[tz] = 0;
		for (i = 0; i < 4; i++) {
			z = tz + dir4[i];
			if (board[z] == col) del_stone(z, col);
		}
	}

	// ���i�߂�Bz ... ���W�Acol ... �΂̐F
	int move_one(int z, int col)
	{
		int i, z1, sum, del_z = 0;
		int all_ishi = 0;	// ������΂̍��v
		int un_col = UNCOL(col);

		if (z == 0) {	// PASS�̏ꍇ
			kou_z = 0;
			return MOVE_SUCCESS;
		}
		if (z == kou_z) {
			PRT(_T("move() Err: �R�E�Iz=%04x\n"), z);
			return MOVE_KOU;
		}
		if (board[z] != 0) {
			PRT(_T("move() Err: ��_�ł͂Ȃ��Iz=%04x\n"), z);
			return MOVE_EXIST;
		}
		board[z] = col;	// �Ƃ肠�����u���Ă݂�

		for (i = 0; i<4; i++) {
			z1 = z + dir4[i];
			if (board[z1] != un_col) continue;
			// �G�̐΂����邩�H
			count_dame(z1);
			if (dame == 0) {
				hama[col - 1] += ishi;
				all_ishi += ishi;
				del_z = z1;	// ���ꂽ�΂̍��W�B�R�E�̔���Ŏg���B
				del_stone(z1, un_col);
			}
		}
		// ���E��𔻒�
		count_dame(z);
		if (dame == 0) {
			PRT(_T("move() Err: ���E��! z=%04x\n"), z);
			board[z] = 0;
			return MOVE_SUICIDE;
		}

		// ���ɃR�E�ɂȂ�ʒu�𔻒�B�΂�1����������ꍇ�B
		kou_z = 0;	// �R�E�ł͂Ȃ�
		if (all_ishi == 1) {
			// ���ꂽ�΂�4�����Ɏ����̃_��1�̘A��1��������ꍇ�A���̈ʒu�̓R�E�B
			kou_z = del_z;	// ��荇�������ꂽ�΂̏ꏊ���R�E�̈ʒu�Ƃ���
			sum = 0;
			for (i = 0; i<4; i++) {
				z1 = del_z + dir4[i];
				if (board[z1] != col) continue;
				count_dame(z1);
				if (dame == 1 && ishi == 1) sum++;
			}
			if (sum >= 2) {
				PRT(_T("�P����āA�R�E�̈ʒu�֑łƁA�P�̐΂�2�ȏ����Hz=%04x\n"), z);
				return MOVE_FATAL;
			}
			if (sum == 0) kou_z = 0;	// �R�E�ɂ͂Ȃ�Ȃ��B
		}
		return MOVE_SUCCESS;
	}


}