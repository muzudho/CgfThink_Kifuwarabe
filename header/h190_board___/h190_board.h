//
// /header/h190_board/h190_board.h
//
#pragma once

extern "C" {

	//--------------------------------------------------------------------------------
	// #define
	//--------------------------------------------------------------------------------

	// ���
	#define EMPTY 0		// �󂫃X�y�[�X
	#define BLACK 1
	#define WHITE 2
	#define WAKU  3		// �ՊO

	#define BOARD_MAX ((19+2)*256)	// 19�H�Ղ��ő�T�C�Y�Ƃ���
	#define INVCLR(x) (3-(x))		// �΂̐F�𔽓]������


	//--------------------------------------------------------------------------------
	// .cpp �t�@�C���Œ�`����Ă��� �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	extern int g_board[BOARD_MAX];
	extern int g_dir4[4];
	extern int g_boardSize;
	extern int g_kouNode;
	extern int g_hama[2];


	//--------------------------------------------------------------------------------
	// �֐�
	//--------------------------------------------------------------------------------

	// (x,y)��1�̍��W�ɕϊ�
	int	ConvertToNode(
		int x,
		int y
		);

	// (node)��(x,y)���W�ɕϊ�
	void ConvertToXy(
		int& x,
		int& y,
		int node
		);

	// 1��i�߂�B
	int	MoveOne(
		int node,	// ���W
		int color		// �΂̐F
		);

	// �A�ɂȂ��Ă���΂������B�P�̐΂ł������B
	void	DeleteRenStones(
		int tNode,
		int color
		);

}