//
// /source/core/board.cpp
//
extern "C" {

	#include "../../header/h190_board/h190_board.h"
	#include "../../header/h400_core/h400_board.h"
	#include "../../header/h400_core/h400_ui.h"
	#include "../../header/think.h"


	//--------------------------------------------------------------------------------
	// �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	int g_board[BOARD_MAX];
	int g_boardSize;


	//--------------------------------------------------------------------------------
	// �֐�
	//--------------------------------------------------------------------------------

	void CountLiberty(int tNode)
	{
		int i;

		g_liberty = g_kakondaIshi = 0;
		for (i = 0; i < BOARD_MAX; i++) {
			g_checkedBoard[i] = 0; 
		}
		CountLibertyElement(tNode, g_board[tNode]);
	}

	void CountLibertyElement(int tNode, int color)
	{
		int adjNode;
		int i;

		g_checkedBoard[tNode] = 1;				// ���̐΂͌����ς�	
		g_kakondaIshi++;								// ���鑊��̐΂̐�
		for (i = 0; i < 4; i++) {
			adjNode = tNode + g_dir4[i];
			if (g_checkedBoard[adjNode]) {
				continue;
			}
			if (g_board[adjNode] == 0) {				// ��_
				g_checkedBoard[adjNode] = 1;			// ���̋�_�͌����ς݂Ƃ���
				g_liberty++;							// ���o�e�B�̐�
			}
			if (g_board[adjNode] == color) {
				CountLibertyElement(adjNode, color);	// ���T���̎����̐�
			}
		}
	}

	int ConvertNode(int x, int y)
	{
		return (y + 1) * 256 + (x + 1);
	}

	int MoveOne(
		int node	,
		int color
	)
	{
		int i;
		int adjNode;			// �l���ɗאڂ����_
		int sum;
		int delNode		= 0;
		int all_ishi	= 0;	// ������΂̍��v
		int un_col		= INVCLR(color);

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

	void DeleteStone(
		int tNode,
		int color
	)
	{
		int adjNode;	// �㉺���E�ɗאڂ����_
		int i;

		g_board[tNode] = 0;
		for (i = 0; i < 4; i++) {
			adjNode = tNode + g_dir4[i];
			if (g_board[adjNode] == color) {
				DeleteStone(adjNode, color);
			}
		}
	}

}
