#pragma once


#define BOARD_MAX ((19+2)*256)	// 19路盤を最大サイズとする


class AbstractBoard {
protected:
	// 盤上の石の色。
	int table[BOARD_MAX];

	// 何路盤
	int size;

public:
	int*	GetTable();

	void	SetSize(int size);
	int		GetSize();

	// セルの値
	void	SetValue(int node, int value);
	int		ValueOf(int node);
};