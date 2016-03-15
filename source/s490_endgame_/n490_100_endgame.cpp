	#include <windows.h> // コンソールへの出力等


extern "C" {

	#include "../../header/h190_board___/n190_100_board.h"
	#include "../../header/h190_board___/n190_150_liberty.h"
	#include "../../header/h390_explain_/n390_100_explain.h"
	#include "../../header/h490_endgame_/n490_100_endgame.h"


	int EndgameStatus(int arr_endgameBoard[], Board* pBoard)
	{
		int		x;
		int		y;
		int		node;
		int		sum;
		int		i;
		int		adjColor;	// 隣接(adjacent)する石の色
		int*	ptr;

		for (y = 1; y < pBoard->size+1; y++) {
			for (x = 1; x< pBoard->size+1; x++) {
				node = Board::ConvertToNode(x, y);

				ptr = arr_endgameBoard + node;
				if (pBoard->table[node] == 0) {
					*ptr = GTP_DAME;
					sum = 0;
					for (i = 0; i<4; i++) {
						adjColor = pBoard->table[node + pBoard->dir4[i]];
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
					liberty.Count(node, pBoard);
					//			PRT("(%2d,%2d),ishi=%2d,dame=%2d\n",z&0xff,z>>8,ishi,dame);
					if (liberty.liberty <= 1) {
						*ptr = GTP_DEAD;
					}
				}
			}
		}
		return 0;
	}

	int EndgameDrawFigure(int arr_endgameBoard[], Board* pBoard)
	{
		int		x;
		int		y;
		int		node;
		int*	ptr;

		for (y = 1; y < pBoard->size+1; y++) {
			for (x = 1; x < pBoard->size+1; x++) {
				node = Board::ConvertToNode(x, y);
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

	int EndgameDrawNumber(int arr_endgameBoard[], Board* pBoard)
	{
		int		x;
		int		y;
		int		node;
		int*	ptr;

		for (y = 1; y < pBoard->size+1; y++) {
			for (x = 1; x < pBoard->size+1; x++) {
				node = Board::ConvertToNode(x, y);
				ptr = arr_endgameBoard + node;
				*ptr = (rand() % 110) - 55;
			}
		}
		return 0;
	}
}
