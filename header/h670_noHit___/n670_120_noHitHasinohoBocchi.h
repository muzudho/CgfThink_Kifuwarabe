#pragma once


#include "../../header/h670_noHit___/n670_000_noHit.h"


// �Տ�̒[�̕��Ɂ@�ڂ����̐΁i�אڂ���΂��Ȃ��΁j�@��ł��Ȃ��d�g�݁B
// �ċz�_���ǂ��ł��܂��ā@�������ɂ����B
class NoHitHasinohoBocchi : NoHit {
private:
	// �ڂ����΂Ȃ�^�B
	bool isBocchi;
	// �ՊO�Ȃ�^�B
	bool isSoto;
	// �g�ɗאڂ��Ă���Ȃ�^�B
	bool isEdge;
	// �p�Ȃ�^�B
	bool isCorner;
public:
	NoHitHasinohoBocchi();

	// �ǂ̂悤�ȏ󋵂������B
	void Research(
		int node,
		int board[],
		int boardSize
		);

	// �]���l���o���܂��B
	int Evaluate(
		);
};