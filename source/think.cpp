
extern "C" {

	#include <time.h>		// clock() ���g�p���邽�߂ɁB
	#include <windows.h>	// rand() �����g�p���邽�߂ɁB
	#include <tchar.h>		// Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB
	#include "../header/core/core_board.h"
	#include "../header/core/core_explain.h"
	#include "../header/core/core_endgame.h"
	#include "../header/core/core_ui.h"
	#include "../header/cgfthink.h"
	#include "../header/think.h"


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
}