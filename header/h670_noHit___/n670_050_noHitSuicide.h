//
// /header/h680_suicide/h680_suicide.h
//

#pragma once


#include "../../header/h190_board___/n190_150_liberty.h"
#include "../../header/h670_noHit___/n670_000_noHit.h"


//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------

//
// ���E����w���Ȃ��d�g�݁B
//
class NoHitSuicide : NoHit
{
public:
	// �G�΂�������t���O
	int flgCapture;

public:
	NoHitSuicide();

	// ���E��ɂȂ�󋵂łȂ��������B
	bool IsThis(
		int		color,
		int		node,
		Liberty liberties[4]
	);
};
