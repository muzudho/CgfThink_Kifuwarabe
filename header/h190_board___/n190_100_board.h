#pragma once

#include <vector>
#include "../h190_board___/n190_050_abstractBoard.h"


//--------------------------------------------------------------------------------
// 定義
//--------------------------------------------------------------------------------

// 碁石
#define EMPTY 0		// 空きスペース
#define BLACK 1
#define WHITE 2
#define WAKU  3		// 盤外

#define INVCLR(x) (3-(x))		// 石の色を反転させる


class Board : public AbstractBoard{
public:

	// 次にコウになる位置。無ければ 0。
	int kouNode;

	// ハマ。取った石の数のこと。[0]...空き。[1]... 黒が取った石の数, [2]...白が取った石の数
	int hama[3];

public:
	void	Initialize( int initBoard[]);

	Board();
	~Board();

	// 上側に隣接している位置
	int NorthOf(int node);

	// 右側に隣接している位置
	int EastOf(int node);

	// 下側に隣接している位置
	int SouthOf(int node);

	// 左側に隣接している位置
	int WestOf(int node);

	// 指定したnode（石）に隣接している空きスペース（1以上3以下）を配列に入れて返します。
	std::vector<int> GetOpenNodesOfStone(
		int node,
		int size123 // 1〜3 のいずれかを指定してください。
		);

	// 連になっている石を消す。１個の石でも消す。
	void	DeleteRenStones(
		int tNode,
		int color
	);

};