//
// /source/core/endgame.cpp
//
extern "C" {

	#include <windows.h> // �R���\�[���ւ̏o�͓�
	#include "../../header/core/core_board.h"
	#include "../../header/core/core_endgame.h"
	#include "../../header/core/core_explain.h"
	#include "../../header/think.h"
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
