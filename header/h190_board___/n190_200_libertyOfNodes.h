#pragma once


#include "../h190_board___/n190_050_abstractBoard.h"
#include "../h190_board___/n190_100_board.h"



// �����̈ʒu���܂ސ΁A�܂��͘A�̑��ċz�_�̐����A�e��_�Ɋi�[�����e�[�u���ł��B
class LibertyOfNodes : public AbstractBoard{
public:
	LibertyOfNodes();
	~LibertyOfNodes();

	void Initialize( Board* pBoard);

	// �g���܂߂Č�Ղ�S����
	// .cpp �ɖ{�̂������ƂȂ񂩃G���[���o���̂ŁA.h �ɏ����Ă���񂾂����i�O���O�j
	template<typename Func> void ForeachAllXyWithWaku(Func func)
	{
		for (int y = 0; y < this->size + 2; y++)
		{
			for (int x = 0; x < this->size + 2; x++)
			{
				bool isBreak = false;
				func(x, y, isBreak);
				if (isBreak)
				{
					break;
				}
			}
		}
	}
};