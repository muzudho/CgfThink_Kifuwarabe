//
// /source/s490_endgame/s490_endgame.cpp
//
extern "C" {

	#include <windows.h> // �R���\�[���ւ̏o�͓�
	#include "../../header/h190_board___/n190_100_board.h"
	#include "../../header/h190_board___/n190_150_liberty.h"
	#include "../../header/h390_explain_/n390_100_explain.h"
	#include "../../header/h490_endgame_/n490_100_endgame.h"


	int EndgameStatus(int arr_endgameBoard[], int board[], int boardSize)
	{
		int		x;
		int		y;
		int		node;
		int		sum;
		int		i;
		int		adjColor;	// �א�(adjacent)����΂̐F
		int*	ptr;

		for (y = 0; y < boardSize; y++) {
			for (x = 0; x< boardSize; x++) {
				node = ConvertToNode(x, y);

				ptr = arr_endgameBoard + node;
				if (board[node] == 0) {
					*ptr = GTP_DAME;
					sum = 0;
					for (i = 0; i<4; i++) {
						adjColor = board[node + g_dir4[i]];
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
					Liberty liberty;
					liberty.Count(node, board);
					//			PRT("(%2d,%2d),ishi=%2d,dame=%2d\n",z&0xff,z>>8,ishi,dame);
					if (liberty.liberty <= 1) {
						*ptr = GTP_DEAD;
					}
				}
			}
		}
		return 0;
	}

	int EndgameDrawFigure(int arr_endgameBoard[], int boardSize)
	{
		int		x;
		int		y;
		int		node;
		int*	ptr;

		for (y = 0; y < boardSize; y++) {
			for (x = 0; x < boardSize; x++) {
				node = ConvertToNode(x, y);
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

	int EndgameDrawNumber(int arr_endgameBoard[], int boardSize)
	{
		int		x;
		int		y;
		int		node;
		int*	ptr;

		for (y = 0; y < boardSize; y++) {
			for (x = 0; x < boardSize; x++) {
				node = ConvertToNode(x, y);
				ptr = arr_endgameBoard + node;
				*ptr = (rand() % 110) - 55;
			}
		}
		return 0;
	}
}