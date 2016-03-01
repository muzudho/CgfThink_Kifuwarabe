//
// cgfthink.h
//
#pragma once

//--------------------------------------------------------------------------------
// #define �܂Ƃ�
//--------------------------------------------------------------------------------

// �ʂ̃v���O��������Ăяo������֐��ɕt���邨�܂��Ȃ��B
#define DLL_EXPORT	__declspec( dllexport )

// ���
#define BLACK 1
#define WHITE 2
#define WAKU  3		// �ՊO

#define BOARD_MAX ((19+2)*256)			// 19�H�Ղ��ő�T�C�Y�Ƃ���

//--------------------------------------------------------------------------------
// enum �܂Ƃ�
//--------------------------------------------------------------------------------

// ���݋ǖʂŉ������邩�A���w��
enum GameType {
	GAME_MOVE,				// �ʏ�̎�
	GAME_END_STATUS,		// �I�Ǐ���
	GAME_DRAW_FIGURE,		// �}�`��`��
	GAME_DRAW_NUMBER 		// ���l������
};

// �R���s���[�^�[�͌�\�t�g�w�ʁx�̃X�N���[���V���b�g�ł��������Ƃ̂���l�����邩������Ȃ��A
// �ՖʁA�΂̏�ɕ`���L��
// (�` | �F) �Ŏw�肷��B���Ŏl�p��`���ꍇ�� (FIGURE_SQUARE | FIGURE_BLACK)
enum FigureType {
	FIGURE_NONE,			// �����`���Ȃ�
	FIGURE_TRIANGLE,		// �O�p�`
	FIGURE_SQUARE,			// �l�p
	FIGURE_CIRCLE,			// �~
	FIGURE_CROSS,			// �~
	FIGURE_QUESTION,		// "�H"�̋L��	
	FIGURE_HORIZON,			// ����
	FIGURE_VERTICAL,		// �c��
	FIGURE_LINE_LEFTUP,		// �΂߁A���ォ��E��
	FIGURE_LINE_RIGHTUP,	// �΂߁A��������E��
	FIGURE_BLACK = 0x1000,	// ���ŕ`���i�F�w��)
	FIGURE_WHITE = 0x2000,	// ���ŕ`��	(�F�w��j
};

// �������ŃZ�b�g����l
// ���̈ʒu�̐΂��u���v���u���v���B�s���ȏꍇ�́u���v�ŁB
// ���̈ʒu�̓_���u���n�v�u���n�v�u�_���v���B
enum GtpStatusType {
	GTP_ALIVE,				// ��
	GTP_DEAD,				// ��
	GTP_ALIVE_IN_SEKI,		// �Z�L�Ŋ��i���g�p�A�u���v�ő�p���ĉ������j
	GTP_WHITE_TERRITORY,	// ���n
	GTP_BLACK_TERRITORY,	// ���n
	GTP_DAME				// �_��
};

// move()�֐��Ŏ��i�߂����̌���
enum MoveResult {
	MOVE_SUCCESS,			// ����
	MOVE_SUICIDE,			// ���E��
	MOVE_KOU,				// �R�E
	MOVE_EXIST,				// ���ɐ΂�����
	MOVE_FATAL				// ����ȊO
};


//--------------------------------------------------------------------------------
// �֐��錾�܂Ƃ߁i�P�j�@�ʂ̃A�v���P�[�V��������Ăяo��������
//--------------------------------------------------------------------------------

// �v�l���[�`���B�{�̂��猻�݂̎萔�Ƃ���܂ł̊�������������ԂŌĂ΂��B
// ��̍��W��Ԃ��BPASS�̏ꍇ0���B
// �܂��A�I�Ǐ����̏ꍇ�́A�I�ǔ��f�̌��ʂ�Ԃ��B
DLL_EXPORT int cgfgui_thinking(
	int init_board[],	// �����Ֆʁi�u��̏ꍇ�́A�����ɒu�΂�����j
	int kifu[][3],		// ����  [][0]...���W�A[][1]...�΂̐F�A[][2]...����ԁi�b)
	int tesuu,			// �萔
	int black_turn,		// ���(����...1�A����...0)
	int board_size,		// �Ֆʂ̃T�C�Y
	double komi,		// �R�~
	int endgame_type,	// 0...�ʏ�̎v�l�A1...�I�Ǐ����A2...�}�`��\���A3...���l��\���B
	int endgame_board[]	// �I�Ǐ����̌��ʂ�������B
);

// �΋ǊJ�n���Ɉ�x�����Ă΂�܂��B
DLL_EXPORT void cgfgui_thinking_init(int *ptr_stop_thinking);

// �΋ǏI�����Ɉ�x�����Ă΂�܂��B
DLL_EXPORT void cgfgui_thinking_close(void);


//--------------------------------------------------------------------------------
// �֐��錾�܂Ƃ߁i�Q�j
//--------------------------------------------------------------------------------

// �T���v���Ŏg�p����֐�
void PRT(const char *fmt, ...);	// printf()�̑�p�֐��B�R���\�[���ɏo�́B
void PassWindowsSystem(void);	// �ꎞ�I��Windows�ɐ����n���܂��B

								// �֐��̃v���g�^�C�v�錾
void count_dame(int tz);				// �_���Ɛ΂̐��𒲂ׂ�
void count_dame_sub(int tz, int col);	// �_���Ɛ΂̐��𒲂ׂ�ċA�֐�
int move_one(int z, int col);			// 1��i�߂�Bz ... ���W�Acol ... �΂̐F
void print_board(void);					// ���݂̔Ֆʂ�\��
int think_sample(int col);
int get_z(int x, int y);					// (x,y)��1�̍��W�ɕϊ�
int endgame_status(int endgame_board[]);		// �I�Ǐ���
int endgame_draw_figure(int endgame_board[]);	// �}�`��`��
int endgame_draw_number(int endgame_board[]);	// ���l������(0�͕\������Ȃ�)


//--------------------------------------------------------------------------------
// �֐��錾�܂Ƃ߁i�R�j�@functions
//--------------------------------------------------------------------------------

// �΂�����
void del_stone(int tz, int col);


