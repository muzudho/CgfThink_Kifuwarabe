//
// /header/h680_suicide/h680_suicide.h
//

#pragma once

//--------------------------------------------------------------------------------
// �N���X
//--------------------------------------------------------------------------------

//
// ���E����w���Ȃ��d�g�݁B
//
class Suicide
{
public:
	// �G�΂�������t���O
	int flgCapture = 0;

public:
	Suicide();

	// ���E��ɂȂ�󋵂łȂ��������B
	void Research(
		int invColor,
		int adjColor
	);

	// ���̌�_�ɂ͑ł����܂Ȃ��Ȃ�^�B
	bool DontHit(
		int		color,
		int		node
	);
};
