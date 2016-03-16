#pragma once

//--------------------------------------------------------------------------------
// 定義
//--------------------------------------------------------------------------------

// 碁石
#define EMPTY 0		// 空きスペース
#define BLACK 1
#define WHITE 2
#define WAKU  3		// 盤外

#define BOARD_MAX ((19+2)*256)	// 19路盤を最大サイズとする
#define INVCLR(x) (3-(x))		// 石の色を反転させる


class Board {
public:
	// 盤上の石の色。
	int table[BOARD_MAX];

	int size;

	// 上、右、下、左　に移動するのに使う加減値
	int dir4[4] = {
		-0x100,	// 上
		+0x001,	// 右
		+0x100,	// 下
		-0x001	// 左
	};// オリジナルのcgfthinkでは右、左、下、上の順だった。

	// 次にコウになる位置。無ければ 0。
	int kouNode;

	// ハマ。取った石の数のこと。[0]... 黒が取った石の数, [1]...白が取った石の数
	int hama[2];

public:

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

	// 枠も含めて碁盤を全走査
	// .cpp に本体を書くとなんかエラーが出たので、.h に書いているんだぜ☆（＾ｑ＾）
	template<typename Func> void ForeachAllNodesWithWaku(Func func)
	{
		for (int x = 0; x < this->size + 2; x++)
		{
			for (int y = 0; y < this->size + 2; y++)
			{
				int node = Board::ConvertToNode(x, y);
				func(node);
			}
		}
	}

	// 枠も含めて碁盤を全走査
	// .cpp に本体を書くとなんかエラーが出たので、.h に書いているんだぜ☆（＾ｑ＾）
	template<typename Func> void ForeachAllXyWithWaku(Func func)
	{
		for (int x = 0; x < this->size + 2; x++)
		{
			for (int y = 0; y < this->size + 2; y++)
			{
				func(x,y);
			}
		}
	}

	// 枠を含めない碁盤を全走査
	// .cpp に本体を書くとなんかエラーが出たので、.h に書いているんだぜ☆（＾ｑ＾）
	template<typename Func> void ForeachAllNodesWithoutWaku( Func func)
	{
		for (int x = 1; x < this->size + 1; x++)
		{
			for (int y = 1; y < this->size + 1; y++)
			{
				int node = Board::ConvertToNode(x, y);
				func(node);
			}
		}
	}
};