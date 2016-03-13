//
// /source/s300_move/s300_move.cpp
//
extern "C" {

	#include "../../header/h090_core____/h090_core.h"
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h200_research/h200_100_Liberty.h"
	#include "../../header/h300_move____/h300_move.h"

	int MoveOne(
		int node,
		int color
		)
	{
		int i;
		int adjNode;						// �l���ɗאڂ����_
		int sum;
		int delNode		= 0;
		int tottaIshi	= 0;				// ������΂̍��v
		int invClr		= INVCLR(color);	// ����̐΂̐F

		//----------------------------------------
		// �p�X�̏ꍇ
		//----------------------------------------
		if (node == 0) {
			// ������󂯕t���܂��B
			g_kouNode = 0;
			return MOVE_SUCCESS;
		}

		//----------------------------------------
		// �R�E�ɒu�����Ƃ����ꍇ
		//----------------------------------------
		if (node == g_kouNode) {
			PRT(_T("move() Err: �R�E�Iz=%04x\n"), node);
			// �����e���܂��B
			return MOVE_KOU;
		}

		//----------------------------------------
		// ��_�łȂ��Ƃ���ɒu�����Ƃ����ꍇ
		//----------------------------------------
		if (g_board[node] != 0) {
			PRT(_T("move() Err: ��_�ł͂Ȃ��Iz=%04x\n"), node);
			// �����e���܂��B
			return MOVE_EXIST;
		}

		g_board[node] = color;	// �Ƃ肠�����u���Ă݂�

		// �������牺�́A�΂�u�������Ƃ̔Ֆʂł��B

		// ���E��ɂȂ邩�ǂ������肷�邽�߂ɁA
		// �܂��A�V�����@�R�E�@����邩�ǂ������肷�邽�߂ɁA
		// ����̐΂����Ƃ���܂Ői�߂܂��B
		for (i = 0; i < 4; i++) {
			adjNode = node + g_dir4[i];

			if (g_board[adjNode] != invClr) {
				// �אڂ���΂��@����̐΁@�łȂ��Ȃ疳���B
				continue;
			}

			//----------------------------------------
			// ����̐΂����邩���肵�܂��B
			//----------------------------------------

			// �אڂ���΁i�A�j�̌ċz�_�𐔂��܂��B
			Liberty liberty;
			liberty.Count(adjNode);

			if (liberty.liberty == 0) {
				// �ċz�_���Ȃ��悤�Ȃ�A�΁i�A�j�͎��܂��B

				// �͂񂾐΂̐����@�n�}�ɉ��_�B
				g_hama[color - 1] += liberty.renIshi;
				tottaIshi += liberty.renIshi;
				delNode = adjNode;	// ���ꂽ�΂̍��W�B�R�E�̔���Ŏg���B

				// ���������Ȃ��悤�ɁA�͂܂�Ă��鑊��̐΁i�v�Z�ς݁j�������܂��B
				DeleteRenStones(adjNode, invClr);
			}
		}

		//----------------------------------------
		// ���E��ɂȂ邩�𔻒�
		//----------------------------------------
		Liberty liberty;
		liberty.Count(node);

		if (liberty.liberty == 0) {
			// �u�����΂Ɍċz�_���Ȃ��ꍇ�B

			// �����e���܂��B
			PRT(_T("move() Err: ���E��! z=%04x\n"), node);
			g_board[node] = 0;
			return MOVE_SUICIDE;
		}

		//----------------------------------------
		// ���ɃR�E�ɂȂ�ʒu�𔻒肵�Ă����B
		//----------------------------------------

		g_kouNode = 0;	// �R�E�ł͂Ȃ�

		// �R�E�ɂȂ�̂́A�΂�1����������ꍇ�ł��B
		if (tottaIshi == 1) {
			// ���ꂽ�΂�4�����ɁA�����̌ċz�_��1�̘A��1��������ꍇ�A���̈ʒu�̓R�E�B
			g_kouNode = delNode;	// ��荇�������ꂽ�΂̏ꏊ���R�E�̈ʒu�Ƃ���
			sum = 0;
			for (i = 0; i < 4; i++) {
				adjNode = delNode + g_dir4[i];
				if (g_board[adjNode] != color) {
					continue;
				}
				Liberty liberty;
				liberty.Count(adjNode);
				if (liberty.liberty == 1 && liberty.renIshi == 1) {
					sum++;
				}
			}
			if (sum >= 2) {
				PRT(_T("�P����āA�R�E�̈ʒu�֑łƁA�P�̐΂�2�ȏ����Hnode=%04x\n"), node);
				// ����̓G���[�B

				// �����e���܂��B
				return MOVE_FATAL;
			}
			if (sum == 0) {
				g_kouNode = 0;	// �R�E�ɂ͂Ȃ�Ȃ��B
			}
		}

		//----------------------------------------
		// �u�����Ƃɐ������i�O���O�j�I
		//----------------------------------------

		// ������󂯓���܂��B
		return MOVE_SUCCESS;
	}


}