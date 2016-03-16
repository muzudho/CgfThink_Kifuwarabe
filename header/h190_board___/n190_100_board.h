#pragma once

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

	// 上、右、下、左　に移動するのに使う加減値
	int dir4[4] = {
		-0x100,	// 上
		+0x001,	// 右
		+0x100,	// 下
		-0x001	// 左
	};// オリジナルのcgfthinkでは右、左、下、上の順だった。

	// 次にコウになる位置。無ければ 0。
	int kouNode;

	// ハマ。取った石の数のこと。[0]...空き。[1]... 黒が取った石の数, [2]...白が取った石の数
	int hama[3];

public:
	void	Initialize( int initBoard[]);

	// (x,y)を1つの座標に変換
	static int ConvertToNode(
		int x,
		int y
	);

	// (node)を(x,y)座標に変換
	static void ConvertToXy(
		int& x,
		int& y,
		int node
	);

	Board();
	~Board();

	// 連になっている石を消す。１個の石でも消す。
	void	DeleteRenStones(
		int tNode,
		int color
	);

	// 碁盤の枠を全走査。左上角から時計回り。
	// .cpp に本体を書くとなんかエラーが出たので、.h に書いているんだぜ☆（＾ｑ＾）
	template<typename Func> void ForeachAllNodesOfWaku(Func func)
	{
		// 上辺（最後の手前まで）
		{
			int y = 0;
			for (int x = 0; x < this->size + 2 - 1; x++)
			{
				int node = Board::ConvertToNode(x, y);

				bool isBreak = false;
				func(node, isBreak);
				if (isBreak)
				{
					break;
				}
			}
		}

		// 右辺（最後の手前まで）
		{
			int x = this->size + 2 - 1;
			for (int y = 0; y < this->size + 2 - 1; y++)
			{
				int node = Board::ConvertToNode(x, y);

				bool isBreak = false;
				func(node, isBreak);
				if (isBreak)
				{
					break;
				}
			}
		}

		// 下辺（最後の手前まで）
		{
			int y = this->size + 2 - 1;
			for (int x = this->size + 2 - 1; 0 < x; x--)
			{
				int node = Board::ConvertToNode(x, y);

				bool isBreak = false;
				func(node, isBreak);
				if (isBreak)
				{
					break;
				}
			}
		}

		// 左辺（最後の手前まで）
		{
			int x = 0;
			for (int y = this->size + 2 - 1; 0 < y; y--)
			{
				int node = Board::ConvertToNode(x, y);

				bool isBreak = false;
				func(node, isBreak);
				if (isBreak)
				{
					break;
				}
			}
		}
	}

	// 枠も含めて碁盤を全走査。左上から右上へ、端で改行して次の行の先頭から。
	// .cpp に本体を書くとなんかエラーが出たので、.h に書いているんだぜ☆（＾ｑ＾）
	template<typename Func> void ForeachAllNodesWithWaku(Func func)
	{
		for (int y = 0; y < this->size + 2; y++)
		{
			for (int x = 0; x < this->size + 2; x++)
			{
				int node = Board::ConvertToNode(x, y);

				bool isBreak = false;
				func(node, isBreak);
				if (isBreak)
				{
					break;
				}
			}
		}
	}

	// 枠も含めて碁盤を全走査。左上から右上へ、端で改行して次の行の先頭から。
	// .cpp に本体を書くとなんかエラーが出たので、.h に書いているんだぜ☆（＾ｑ＾）
	template<typename Func> void ForeachAllXyWithWaku(Func func)
	{
		for (int y = 0; y < this->size + 2; y++)
		{
			for (int x = 0; x < this->size + 2; x++)
			{
				bool isBreak = false;
				func(x,y, isBreak);
				if (isBreak)
				{
					break;
				}
			}
		}
	}

	// 枠を含めない碁盤を全走査。左上から右上へ、端で改行して次の行の先頭から。
	// .cpp に本体を書くとなんかエラーが出たので、.h に書いているんだぜ☆（＾ｑ＾）
	template<typename Func> void ForeachAllNodesWithoutWaku( Func func)
	{
		for (int y = 1; y < this->size + 1; y++)
		{
			for (int x = 1; x < this->size + 1; x++)
			{
				int node = Board::ConvertToNode(x, y);

				bool isBreak = false;
				func(node, isBreak);
				if (isBreak)
				{
					break;
				}
			}
		}
	}

	// 指定のノードに隣接する上、右、下、左のノードを走査。
	template<typename Func> void ForeachArroundNodes(int node, Func func)
	{
		for (int iDir = 0; iDir < 4; iDir++) {
			bool isBreak = false;
			func(node + this->dir4[iDir], isBreak);
			if (isBreak)
			{
				break;
			}
		}
	}

	// 指定のノードに隣接する上、右、下、左のノードを走査。
	template<typename Func> void ForeachArroundDirAndNodes(int node, Func func)
	{
		for (int iDir = 0; iDir < 4; iDir++) {
			bool isBreak = false;
			func(iDir, node + this->dir4[iDir], isBreak);
			if (isBreak)
			{
				break;
			}
		}
	}

};