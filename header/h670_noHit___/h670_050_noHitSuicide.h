//
// /header/h680_suicide/h680_suicide.h
//

#pragma once


#include "../../header/h200_research/h200_100_Liberty.h"
#include "../../header/h670_noHit___/h670_000_noHit.h"


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
