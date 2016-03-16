#include "../../header/h090_core____/n090_100_core.h"
#include "../../header/h190_board___/n190_100_board.h"
#include "../../header/h190_board___/n190_150_liberty.h"
#include "../../header/h300_move____/n300_100_move.h"


int Move::MoveOne(
	Core core,
	int node,
	int color,
	Board* pBoard
	)
{
	int sum;
	int delNode		= 0;
	int tottaIshi	= 0;				// ������΂̍��v
	int invClr		= INVCLR(color);	// ����̐΂̐F

	//----------------------------------------
	// �p�X�̏ꍇ
	//----------------------------------------
	if (node == 0) {
		// ������󂯕t���܂��B
		pBoard->kouNode = 0;
		return MOVE_SUCCESS;
	}

	//----------------------------------------
	// �R�E�ɒu�����Ƃ����ꍇ
	//----------------------------------------
	if (node == pBoard->kouNode) {
		core.PRT(_T("move() Err: �R�E�Iz=%04x\n"), node);
		// �����e���܂��B
		return MOVE_KOU;
	}

	//----------------------------------------
	// ��_�łȂ��Ƃ���ɒu�����Ƃ����ꍇ
	//----------------------------------------
	if (pBoard->ValueOf(node) != 0) {
		core.PRT(_T("move() Err: ��_�ł͂Ȃ��Iz=%04x\n"), node);
		// �����e���܂��B
		return MOVE_EXIST;
	}

	pBoard->SetValue(node, color);	// �Ƃ肠�����u���Ă݂�

	// �������牺�́A�΂�u�������Ƃ̔Ֆʂł��B

	// ���E��ɂȂ邩�ǂ������肷�邽�߂ɁA
	// �܂��A�V�����@�R�E�@����邩�ǂ������肷�邽�߂ɁA
	// ����̐΂����Ƃ���܂Ői�߂܂��B
	pBoard->ForeachArroundNodes(node, [&pBoard, &tottaIshi, &delNode, color, invClr](int adjNode, bool& isBreak) {
		Liberty liberty1;

		if (pBoard->ValueOf(adjNode) != invClr) {
			// �אڂ���΂��@����̐΁@�łȂ��Ȃ疳���B
			goto gt_Next1;
		}

		//----------------------------------------
		// ����̐΂����邩���肵�܂��B
		//----------------------------------------

		// �אڂ���΁i�A�j�̌ċz�_�𐔂��܂��B
		liberty1.Count(adjNode, pBoard);

		if (liberty1.liberty == 0) {
			// �ċz�_���Ȃ��悤�Ȃ�A�΁i�A�j�͎��܂��B

			// �͂񂾐΂̐����@�n�}�ɉ��_�B
			pBoard->hama[color] += liberty1.renIshi;
			tottaIshi += liberty1.renIshi;
			delNode = adjNode;	// ���ꂽ�΂̍��W�B�R�E�̔���Ŏg���B

								// ���������Ȃ��悤�ɁA�͂܂�Ă��鑊��̐΁i�v�Z�ς݁j�������܂��B
			pBoard->DeleteRenStones(adjNode, invClr);
		}

	gt_Next1:
		;
	});

	//----------------------------------------
	// ���E��ɂȂ邩�𔻒�
	//----------------------------------------
	Liberty liberty;
	liberty.Count(node, pBoard);

	if (liberty.liberty == 0) {
		// �u�����΂Ɍċz�_���Ȃ��ꍇ�B

		// �����e���܂��B
		core.PRT(_T("move() Err: ���E��! z=%04x\n"), node);
		pBoard->SetValue(node, 0);
		return MOVE_SUICIDE;
	}

	//----------------------------------------
	// ���ɃR�E�ɂȂ�ʒu�𔻒肵�Ă����B
	//----------------------------------------

	pBoard->kouNode = 0;	// �R�E�ł͂Ȃ�

	// �R�E�ɂȂ�̂́A�΂�1����������ꍇ�ł��B
	if (tottaIshi == 1) {
		// ���ꂽ�΂�4�����ɁA�����̌ċz�_��1�̘A��1��������ꍇ�A���̈ʒu�̓R�E�B
		pBoard->kouNode = delNode;	// ��荇�������ꂽ�΂̏ꏊ���R�E�̈ʒu�Ƃ���
		sum = 0;
		pBoard->ForeachArroundNodes(delNode, [&pBoard, &sum, color](int adjNode, bool& isBreak) {
			Liberty liberty2;

			if (pBoard->ValueOf(adjNode) != color) {
				goto gt_Next2;
			}

			liberty2.Count(adjNode, pBoard);
			if (liberty2.liberty == 1 && liberty2.renIshi == 1) {
				sum++;
			}

		gt_Next2:
			;
		});

		if (sum >= 2) {
			core.PRT(_T("�P����āA�R�E�̈ʒu�֑łƁA�P�̐΂�2�ȏ����Hnode=%04x\n"), node);
			// ����̓G���[�B

			// �����e���܂��B
			return MOVE_FATAL;
		}
		if (sum == 0) {
			pBoard->kouNode = 0;	// �R�E�ɂ͂Ȃ�Ȃ��B
		}
	}

	//----------------------------------------
	// �u�����Ƃɐ������i�O���O�j�I
	//----------------------------------------

	// ������󂯓���܂��B
	return MOVE_SUCCESS;
}
