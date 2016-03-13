//
// /header/h190_board/h190_board.h
//
#pragma once

extern "C" {

	//--------------------------------------------------------------------------------
	// #define
	//--------------------------------------------------------------------------------

	// ���
	#define BLACK 1
	#define WHITE 2
	#define WAKU  3		// �ՊO

	#define BOARD_MAX ((19+2)*256)	// 19�H�Ղ��ő�T�C�Y�Ƃ���
	#define INVCLR(x) (3-(x))		// �΂̐F�𔽓]������


	//--------------------------------------------------------------------------------
	// .cpp �t�@�C���Œ�`����Ă��� �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	extern int g_board[BOARD_MAX];
	extern int g_checkedBoard[BOARD_MAX];
	extern int g_dir4[4];
	extern int g_boardSize;
	extern int g_renIshi;
	extern int g_liberty;
	extern int g_kouNode;
	extern int g_hama[2];


	//--------------------------------------------------------------------------------
	// �֐�
	//--------------------------------------------------------------------------------

	// �ʒu tNode �ɂ����郊�o�e�B�i�΂̌ċz�_�j�̐��Ɛ΂̐����v�Z�B���ʂ̓O���[�o���ϐ��Ɋi�[�B
	void	CountLiberty(
		int node
	);

	// ���o�e�B�i�΂̌ċz�_�j�Ɛ΂̐�����ċA�֐�
	// 4�����𒲂ׂāA�󔒂�������+1�A�����̐΂Ȃ�ċA�ŁB����̐΁A�ǂȂ炻�̂܂܁B
	void	CountLibertyElement(
		int tNode,
		int color
		);

	// (x,y)��1�̍��W�ɕϊ�
	int	ConvertNode(
		int x,
		int y
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