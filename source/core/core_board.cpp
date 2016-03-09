//
// /source/core/board.cpp
//
extern "C" {

	#include "../../header/core/core_board.h"
	#include "../../header/think.h"

	// �ʒu tz �ɂ�����_���̐��Ɛ΂̐����v�Z�B���ʂ̓O���[�o���ϐ��ɁB
	void count_dame(int tz)
	{
		int i;

		g_dame = g_ishi = 0;
		for (i = 0; i < BOARD_MAX; i++) g_checkedBoard[i] = 0;
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
		for (i = 0; i < 4; i++) {
			node = tNode + g_dir4[i];
			if (g_checkedBoard[node]) continue;
			if (g_board[node] == 0) {
				g_checkedBoard[node] = 1;	// ���̋�_�͌����ς�
				g_dame++;				// �_���̐�
			}
			if (g_board[node] == col) count_dame_sub(node, col);	// ���T���̎����̐�
		}
	}

	// (x,y)��1�̍��W�ɕϊ�
	int ConvertNode(int x, int y)
	{
		return (y + 1) * 256 + (x + 1);
	}

	// ���i�߂�B
	int move_one(
		int node,	// ���W
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

		for (i = 0; i < 4; i++) {
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
			for (i = 0; i < 4; i++) {
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

}
