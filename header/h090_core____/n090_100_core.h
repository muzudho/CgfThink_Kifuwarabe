//
// /header/h090_core/h090_core.h
//
#pragma once

extern "C" {

	#include <windows.h>	// rand() �����g�p���邽�߂ɁB
	#include <tchar.h> // Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB


	//--------------------------------------------------------------------------------
	// ��`
	//--------------------------------------------------------------------------------

	#define PRT_LEN_MAX 256			// �ő�256�����܂ŏo�͉�


	//--------------------------------------------------------------------------------
	// .cpp �t�@�C���Œ�`����Ă��� �O���[�o���ϐ�
	//--------------------------------------------------------------------------------

	extern HANDLE g_hConsoleWindow;


	//--------------------------------------------------------------------------------
	// �֐�
	//--------------------------------------------------------------------------------

	// printf()�̑�p�֐��B�R���\�[���ɏo�́B
	void PRT(
		const _TCHAR* format,
		...
	);

	// �ꎞ�I��Windows�ɐ����n���܂��B
	// �v�l���ɂ��̊֐����ĂԂƎv�l���f�{�^�����L���ɂȂ�܂��B
	// ���b30��ȏ�Ă΂��悤�ɂ���ƃX���[�Y�ɒ��f�ł��܂��B
	void YieldWindowsSystem(
		void
	);
}