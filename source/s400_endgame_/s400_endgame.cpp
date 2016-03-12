//
// /source/s400_endgame/s400_endgame.cpp
//
extern "C" {

	#include <windows.h> // �R���\�[���ւ̏o�͓�
	#include "../../header/h190_board___/h190_board.h"
	#include "../../header/h390_explain_/h390_explain.h"
	#include "../../header/h400_endgame_/h400_endgame.h"


	int EndgameStatus(int arr_endgameBoard[])
	{
		int		x;
		int		y;
		int		node;
		int		sum;
		int		i;
		int		adjColor;	// �א�(adjacent)����΂̐F
		int*	ptr;

		for (y = 0; y < g_boardSize; y++) {
			for (x = 0; x<g_boardSize; x++) {
				node = ConvertNode(x, y);

				ptr = arr_endgameBoard + node;
				if (g_board[node] == 0) {
					*ptr = GTP_DAME;
					sum = 0;
					for (i = 0; i<4; i++) {
						adjColor = g_board[node + g_dir4[i]];
						if (adjColor == WAKU) {
							continue;
						}
						sum |= adjColor;
					}
					if (sum == BLACK) {
						*ptr = GTP_BLACK_TERRITORY;
					}
					if (sum == WHITE) {
						*ptr = GTP_WHITE_TERRITORY;
					}
				}
				else {
					*ptr = GTP_ALIVE;
					CountLiberty(node);
					//			PRT("(%2d,%2d),ishi=%2d,dame=%2d\n",z&0xff,z>>8,ishi,dame);
					if (g_liberty <= 1) {
						*ptr = GTP_DEAD;
					}
				}
			}
		}
		return 0;
	}

	int EndgameDrawFigure(int arr_endgameBoard[])
	{
		int		x;
		int		y;
		int		node;
		int*	ptr;

		for (y = 0; y < g_boardSize; y++) {
			for (x = 0; x < g_boardSize; x++) {
				node = ConvertNode(x, y);
				ptr = arr_endgameBoard + node;
				if ((rand() % 2) == 0) {
					*ptr = FIGURE_NONE;
				} else {
					if (rand() % 2) {
						*ptr = FIGURE_BLACK;
					} else {
						*ptr = FIGURE_WHITE;
					}
					*ptr |= (rand() % 9) + 1;
				}
			}
		}
		return 0;
	}

	int EndgameDrawNumber(int arr_endgameBoard[])
	{
		int		x;
		int		y;
		int		node;
		int*	ptr;

		for (y = 0; y < g_boardSize; y++) {
			for (x = 0; x < g_boardSize; x++) {
				node = ConvertNode(x, y);
				ptr = arr_endgameBoard + node;
				*ptr = (rand() % 110) - 55;
			}
		}
		return 0;
	}
}