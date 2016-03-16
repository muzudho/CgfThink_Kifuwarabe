#pragma once

#include <vector>
#include "../h190_board___/n190_050_abstractBoard.h"


//--------------------------------------------------------------------------------
// ��`
//--------------------------------------------------------------------------------

// ���
#define EMPTY 0		// �󂫃X�y�[�X
#define BLACK 1
#define WHITE 2
#define WAKU  3		// �ՊO

#define INVCLR(x) (3-(x))		// �΂̐F�𔽓]������


class Board : public AbstractBoard{
public:

	// ���ɃR�E�ɂȂ�ʒu�B������� 0�B
	int kouNode;

	// �n�}�B������΂̐��̂��ƁB[0]...�󂫁B[1]... ����������΂̐�, [2]...����������΂̐�
	int hama[3];

public:
	void	Initialize( int initBoard[]);

	Board();
	~Board();

	// �㑤�ɗאڂ��Ă���ʒu
	int NorthOf(int node);

	// �E���ɗאڂ��Ă���ʒu
	int EastOf(int node);

	// �����ɗאڂ��Ă���ʒu
	int SouthOf(int node);

	// �����ɗאڂ��Ă���ʒu
	int WestOf(int node);

	// �w�肵��node�i�΁j�ɗאڂ��Ă���󂫃X�y�[�X�i1�ȏ�3�ȉ��j��z��ɓ���ĕԂ��܂��B
	std::vector<int> GetOpenNodesOfStone(
		int node,
		int size123 // 1�`3 �̂����ꂩ���w�肵�Ă��������B
		);

	// �A�ɂȂ��Ă���΂������B�P�̐΂ł������B
	void	DeleteRenStones(
		int tNode,
		int color
	);

};