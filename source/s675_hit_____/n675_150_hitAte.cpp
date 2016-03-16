#include "..\..\header\h675_hit_____\n675_150_hitAte.h"

int HitAte::Evaluate(int color, int node, Board* pBoard, LibertyOfNodes* libertyOfNodes)
{
	int score		= 0;
	int opponent	= INVCLR(color);

	int libertyOfRen = libertyOfNodes->ValueOf(node);
	if (pBoard->ValueOf(node) == opponent && libertyOfRen<4)
	{
		// ����̐΁i�܂��͘A�j�ŁA�ċz�_�� 3 �ӏ��ȉ��̕���I�т܂��B

		std::vector<int> openNodes = pBoard->GetOpenNodesOfStone(node, libertyOfRen);
		if(!openNodes.empty())
		{
			// ���̐΁i�A�ł͂Ȃ��j�̊J���Ă�������i�P�����`�R�����j������ꍇ�B

			if (openNodes.size()==1)
			{
				// �A�^���@�̏�Ԃł��B
				if (score < 120)
				{
					// ���̎w����Ɂ@����Ƃ������肪�Ȃ��悤�Ȃ�A�A�e�ɂ����܂��傤�B
					score = 120;
				}
			} else {
				// �A�^���@�ł͂Ȃ��ꍇ�B
			}
		}
	}

	return score;
}
