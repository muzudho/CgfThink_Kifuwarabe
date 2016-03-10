//
// /source/s300_move/s300_move.cpp
//
extern "C" {

	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h300_move____/h300_move.h"

	int MoveOne(
		int node,
		int color
		)
	{
		int i;
		int adjNode;			// �l���ɗאڂ����_
		int sum;
		int delNode = 0;
		int all_ishi = 0;	// ������΂̍��v
		int un_col = INVCLR(color);

		if (node == 0) {		// PASS�̏ꍇ
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
		g_board[node] = color;	// �Ƃ肠�����u���Ă݂�

		for (i = 0; i < 4; i++) {
			adjNode = node + g_dir4[i];
			if (g_board[adjNode] != un_col) {
				continue;
			}
			// �G�̐΂����邩�H
			CountLiberty(adjNode);
			if (g_liberty == 0) {
				g_hama[color - 1] += g_kakondaIshi;
				all_ishi += g_kakondaIshi;
				delNode = adjNode;	// ���ꂽ�΂̍��W�B�R�E�̔���Ŏg���B
				DeleteStone(adjNode, un_col);
			}
		}
		// ���E��𔻒�
		CountLiberty(node);
		if (g_liberty == 0) {
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
				adjNode = delNode + g_dir4[i];
				if (g_board[adjNode] != color) {
					continue;
				}
				CountLiberty(adjNode);
				if (g_liberty == 1 && g_kakondaIshi == 1) {
					sum++;
				}
			}
			if (sum >= 2) {
				PRT(_T("�P����āA�R�E�̈ʒu�֑łƁA�P�̐΂�2�ȏ����Hnode=%04x\n"), node);
				return MOVE_FATAL;
			}
			if (sum == 0) {
				g_kouNode = 0;	// �R�E�ɂ͂Ȃ�Ȃ��B
			}
		}
		return MOVE_SUCCESS;
	}


}