
extern "C" {

	#include <time.h>		// clock() ���g�p���邽�߂ɁB
	#include <windows.h>	// rand() �����g�p���邽�߂ɁB
	#include <tchar.h> // Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB
	#include "../../header/cgfthink.h"
	#include "../../header/core/board.h"
	#include "../../header/core/explain.h"
	#include "../../header/core/endgame.h"
	#include "../../header/library/think.h"

	//--------------------------------------------------------------------------------
	// ��`
	//--------------------------------------------------------------------------------

	#define PRT_LEN_MAX 256			// �ő�256�����܂ŏo�͉�

	//--------------------------------------------------------------------------------
	// �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	int g_board[BOARD_MAX];

	// �E�A���A���A��Ɉړ�����̂Ɏg�������l
	int g_dir4[4] = {
		+0x001,	// �E
		-0x001,	// ��
		+0x100,	// ��
		-0x100	// ��
	};

	// ���ɂ��̐΂����������ꍇ��1
	int g_checkedBoard[BOARD_MAX];

	// �Ֆʂ̃T�C�Y�B19�H�Ղł�19�A9�H�Ղł�9
	int g_boardSize;

	// ������΂̐�(�ċA�֐��Ŏg��)
	int g_ishi = 0;

	// �A�̃_���̐�(�ċA�֐��Ŏg��)
	int g_dame = 0;

	// ���ɃR�E�ɂȂ�ʒu�B������� 0�B
	int g_kouNode = 0;

	// �n�}�B������΂̐��̂��ƁB[0]... ����������΂̐�, [1]...����������΂̐�
	int g_hama[2];

	// �R���\�[���ɏo�͂��邽�߂̃n���h��
	HANDLE g_hConsoleWindow = INVALID_HANDLE_VALUE; //static HANDLE hOutput;

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
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	// printf()�̑�p�֐��B
	void PRT(const _TCHAR* format, ...)
	{
		va_list argList;
		int len;
		static _TCHAR text[PRT_LEN_MAX];
		DWORD nw;

		if (g_hConsoleWindow == INVALID_HANDLE_VALUE) return;
		va_start(argList, format);
		len = _vsnwprintf(text, PRT_LEN_MAX - 1, format, argList);
		va_end(argList);

		if (len < 0 || len >= PRT_LEN_MAX) return;
		WriteConsole(g_hConsoleWindow, text, (DWORD)wcslen(text), &nw, NULL);
	}

	// �����ɋ߂��]���֐��B�����΂����ɍs���悤�ɁB
	int think_sample(int col)
	{
		int max;
		int bestmoveNode;
		int x;
		int y;
		int node;
		int i;
		int value;
		int capture;
		int z1;
		int flag;
		int safe;
		int k;
		int un_col = UNCOL(col);

		// ���s���邽�тɈႤ�l��������悤�Ɍ��݂̎����ŗ�����������
		srand((unsigned)clock());

		max = -1;
		bestmoveNode = 0;
		for (y = 0; y<g_boardSize; y++) for (x = 0; x<g_boardSize; x++) {
			node = ConvertNode(x, y);
			if (g_board[node]) continue;
			if (node == g_kouNode) continue;	// �R�E

			value = rand() % 100;
			capture = safe = 0;
			for (i = 0; i<4; i++) {
				z1 = node + g_dir4[i];
				k = g_board[z1];
				if (k == WAKU) safe++;
				if (k == 0 || k == WAKU) continue;
				count_dame(z1);
				if (k == un_col && g_dame == 1) capture = 1;	// �G�΂�����
				if (k == col && g_dame >= 2) safe++;			// ���S�Ȗ����Ɍq����
				value += (k == un_col) * g_ishi * (10 / (g_dame + 1));
			}
			if (safe == 4) continue;	// ��ɂ͑ł��Ȃ��B
			if (capture == 0) {		// �΂����Ȃ��ꍇ�͎��ۂɒu���Ă݂Ď��E�肩�ǂ�������
				int kz = g_kouNode;			// �R�E�̈ʒu��ޔ�
				flag = move_one(node, col);
				g_board[node] = 0;
				g_kouNode = kz;
				if (flag == MOVE_SUICIDE) continue;	// ���E��
			}
			//		PRT("x,y=(%d,%d)=%d\n",x,y,value);
			if (value > max) { max = value; bestmoveNode = node; }
		}
		return bestmoveNode;
	}

	// ���݂̔Ֆʂ�\��
	void print_board(void)
	{
		int x;
		int y;
		int node;
		_TCHAR* str[4] = { _T("�E"), _T("��"), _T("��"), _T("�{") };

		for (y = 0; y<g_boardSize + 2; y++) for (x = 0; x<g_boardSize + 2; x++) {
			node = (y + 0) * 256 + (x + 0);
			PRT(_T("%s"), str[g_board[node]]);
			if (x == g_boardSize + 1) PRT(_T("\n"));
		}
	}

	// �I�Ǐ����i�T���v���ł͊ȒP�Ȕ��f�Ŏ��΂ƒn�̔�������Ă��܂��j
	int endgame_status(int endgame_board[])
	{
		int x;
		int y;
		int node;
		int sum;
		int i;
		int k;
		int *p;

		for (y = 0; y<g_boardSize; y++) for (x = 0; x<g_boardSize; x++) {
			node = ConvertNode(x, y);
			p = endgame_board + node;
			if (g_board[node] == 0) {
				*p = GTP_DAME;
				sum = 0;
				for (i = 0; i<4; i++) {
					k = g_board[node + g_dir4[i]];
					if (k == WAKU) continue;
					sum |= k;
				}
				if (sum == BLACK) *p = GTP_BLACK_TERRITORY;
				if (sum == WHITE) *p = GTP_WHITE_TERRITORY;
			}
			else {
				*p = GTP_ALIVE;
				count_dame(node);
				//			PRT("(%2d,%2d),ishi=%2d,dame=%2d\n",z&0xff,z>>8,ishi,dame);
				if (g_dame <= 1) *p = GTP_DEAD;
			}
		}
		return 0;
	}

	// �}�`��`��
	int endgame_draw_figure(int endgame_board[])
	{
		int x;
		int y;
		int node;
		int *p;

		for (y = 0; y<g_boardSize; y++) for (x = 0; x<g_boardSize; x++) {
			node = ConvertNode(x, y);
			p = endgame_board + node;
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
		int x;
		int y;
		int node;
		int *p;

		for (y = 0; y<g_boardSize; y++) for (x = 0; x<g_boardSize; x++) {
			node = ConvertNode(x, y);
			p = endgame_board + node;
			*p = (rand() % 110) - 55;
		}
		return 0;
	}

	// (x,y)��1�̍��W�ɕϊ�
	int ConvertNode(int x, int y)
	{
		return (y + 1) * 256 + (x + 1);
	}

	// �ʒu tz �ɂ�����_���̐��Ɛ΂̐����v�Z�B���ʂ̓O���[�o���ϐ��ɁB
	void count_dame(int tz)
	{
		int i;

		g_dame = g_ishi = 0;
		for (i = 0; i<BOARD_MAX; i++) g_checkedBoard[i] = 0;
		count_dame_sub(tz, g_board[tz]);
	}

	// �_���Ɛ΂̐�����ċA�֐�
	// 4�����𒲂ׂāA�󔒂�������+1�A�����̐΂Ȃ�ċA�ŁB����̐΁A�ǂȂ炻�̂܂܁B
	void count_dame_sub(int tNode, int col)
	{
		int node;
		int i;

		g_checkedBoard[tNode] = 1;			// ���̐΂͌����ς�	
		g_ishi++;							// �΂̐�
		for (i = 0; i<4; i++) {
			node = tNode + g_dir4[i];
			if (g_checkedBoard[node]) continue;
			if (g_board[node] == 0) {
				g_checkedBoard[node] = 1;	// ���̋�_�͌����ς�
				g_dame++;				// �_���̐�
			}
			if (g_board[node] == col) count_dame_sub(node, col);	// ���T���̎����̐�
		}
	}

	// �΂�����
	void del_stone(int tNode, int col)
	{
		int node;
		int i;

		g_board[tNode] = 0;
		for (i = 0; i < 4; i++) {
			node = tNode + g_dir4[i];
			if (g_board[node] == col) del_stone(node, col);
		}
	}

	// ���i�߂�B
	int move_one(
		int node	,	// ���W
		int col			// �΂̐F
	)
	{
		int i;
		int z1;
		int sum;
		int delNode = 0;
		int all_ishi = 0;	// ������΂̍��v
		int un_col = UNCOL(col);

		if (node == 0) {	// PASS�̏ꍇ
			g_kouNode = 0;
			return MOVE_SUCCESS;
		}
		if (node == g_kouNode) {
			PRT(_T("move() Err: �R�E�Iz=%04x\n"), node);
			return MOVE_KOU;
		}
		if (g_board[node] != 0) {
			PRT(_T("move() Err: ��_�ł͂Ȃ��Iz=%04x\n"), node);
			return MOVE_EXIST;
		}
		g_board[node] = col;	// �Ƃ肠�����u���Ă݂�

		for (i = 0; i<4; i++) {
			z1 = node + g_dir4[i];
			if (g_board[z1] != un_col) continue;
			// �G�̐΂����邩�H
			count_dame(z1);
			if (g_dame == 0) {
				g_hama[col - 1] += g_ishi;
				all_ishi += g_ishi;
				delNode = z1;	// ���ꂽ�΂̍��W�B�R�E�̔���Ŏg���B
				del_stone(z1, un_col);
			}
		}
		// ���E��𔻒�
		count_dame(node);
		if (g_dame == 0) {
			PRT(_T("move() Err: ���E��! z=%04x\n"), node);
			g_board[node] = 0;
			return MOVE_SUICIDE;
		}

		// ���ɃR�E�ɂȂ�ʒu�𔻒�B�΂�1����������ꍇ�B
		g_kouNode = 0;	// �R�E�ł͂Ȃ�
		if (all_ishi == 1) {
			// ���ꂽ�΂�4�����Ɏ����̃_��1�̘A��1��������ꍇ�A���̈ʒu�̓R�E�B
			g_kouNode = delNode;	// ��荇�������ꂽ�΂̏ꏊ���R�E�̈ʒu�Ƃ���
			sum = 0;
			for (i = 0; i<4; i++) {
				z1 = delNode + g_dir4[i];
				if (g_board[z1] != col) continue;
				count_dame(z1);
				if (g_dame == 1 && g_ishi == 1) sum++;
			}
			if (sum >= 2) {
				PRT(_T("�P����āA�R�E�̈ʒu�֑łƁA�P�̐΂�2�ȏ����Hz=%04x\n"), node);
				return MOVE_FATAL;
			}
			if (sum == 0) g_kouNode = 0;	// �R�E�ɂ͂Ȃ�Ȃ��B
		}
		return MOVE_SUCCESS;
	}


}