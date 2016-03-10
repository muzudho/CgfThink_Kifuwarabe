//
// /header/h800_cgfthink/h800_cgfthink.h
//

// #pragma once �Ƃ́A
// �������̃\�[�X�t�@�C���i�`.c�A�`.cpp�j����@���̃w�b�_�E�t�@�C����
// ���� #include ���悤�Ƃ��Ă��@�Q��ڈȍ~�����������������
#pragma once

extern "C" {

	#include <windows.h>	// HANDLE �����g�p���邽�߂ɁB
	#include "../h190_board/h190_board.h"


	//--------------------------------------------------------------------------------
	// #define �܂Ƃ�
	//--------------------------------------------------------------------------------

	// DLL_EXPORT �́A���Ȃ������ǂ�ł��邱�� .dll �t�@�C���̃\�[�X�R�[�h�Ƃ͕ʂ́A
	// ���s���� .exe �v���O��������A�Ăяo���������֐��̓��ɕt���邨�܂��Ȃ�������
	#define DLL_EXPORT	__declspec( dllexport )


	//--------------------------------------------------------------------------------
	// �O���O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	//
	// extern �Ƃ́A�ϐ���֐����@�ʂ̃t�@�C���Ŋ��� ��`���Ă���Ƃ��ɁA
	// �������̃t�@�C���Œ�`���Ă���̂ł�����g���܂��A�Ƃ����Ӗ��Œ�`�̓��ɕt����A
	// ���܂��Ȃ��������I
	// �R���p�C�����ǂ݂ɍs���t�@�C���̂ǂ����� 1�� extern �̂��Ă��Ȃ���`��
	// �����OK�Ȃ񂾂����I ������`���Ă���܂��A�Ƃ����R���p�C���E�G���[��h���邺���I
	//

	extern int g_board[BOARD_MAX];
	extern int g_dir4[4];
	extern int g_checkedBoard[BOARD_MAX];
	//extern int g_boardSize;
	extern int g_kakondaIshi;
	extern int g_liberty;
	extern int g_kouNode;
	extern int g_hama[2];
	extern HANDLE g_hConsoleWindow;

}

//--------------------------------------------------------------------------------
// �֐��錾�܂Ƃ߁@�ʂ̃A�v���P�[�V��������Ăяo��������
//--------------------------------------------------------------------------------

// �΋ǊJ�n���Ɉ�x�����Ă΂�܂��B
DLL_EXPORT void cgfgui_thinking_init(
	int *pThinkStoped	// ���i��0�B���~�{�^���������ꂽ�Ƃ��� 1 �ɂȂ�܂��B���̒l��1�ɂȂ����ꍇ�͎v�l���I�����Ă��������B
	);

// �v�l���[�`���B����1��̍��W��Ԃ��BPASS�̏ꍇ0�B
// GUI���猻�݂̋ǖʂ̏�񂪓n�����B
// �܂��A�I�Ǐ����̏ꍇ�́A�I�ǔ��f�̌��ʂ�Ԃ��B
DLL_EXPORT int cgfgui_thinking(
	int		initBoard[]		,	// �����Ֆʁi�u��̏ꍇ�́A�����ɒu�΂�����j
	int		kifu[][3]		,	// ����	[�萔][0]...���W
								//		[�萔][1]...�΂̐F
								//		[�萔][2]...����ԁi�b)
								// �萔�� 0 ����n�܂�AcurTesuu ��1��O�܂ł���B
	int		tesuu			,	// ���݂̎萔
	int		flgBlackTurn	,	// ����ԃt���O(����...1�A����...0)�B���������萔�ƈ���Ă�₱�����B
	int		boardSize		,	// �Ֆʂ̃T�C�Y
	double	komi			,	// �R�~
	int		endgameType		,	// 0...�ʏ�̎v�l�A1...�I�Ǐ����A2...�}�`��\���A3...���l��\���B
	int		endgameBoard[]		// �I�Ǐ����̌��ʂ�������B
);

// �΋ǏI�����Ɉ�x�����Ă΂�܂��B
// �������̉���Ȃǂ��K�v�ȏꍇ�ɂ����ɋL�q���Ă��������B
DLL_EXPORT void cgfgui_thinking_close(void);


