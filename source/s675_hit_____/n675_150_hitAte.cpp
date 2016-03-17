#include <tchar.h> // Unicode�Ή��� _T() �֐����g�p���邽�߂ɁB
#include "../../header/h090_core____/n090_100_core.h"
#include "..\..\header\h675_hit_____\n675_150_hitAte.h"

int HitAte::Evaluate(Core core, int color, int node, Board* pBoard, LibertyOfNodes* pLibertyOfNodes)
{
	int score		= 0;
	int opponent	= INVCLR(color);

	// ��E�����ɁA����̐΂��Ȃ����T���܂��B
	pBoard->ForeachArroundNodes(node, [&core,&pBoard,&pLibertyOfNodes,&score, opponent](int adjNode, bool& isBreak) {
		int libertyOfRen = pLibertyOfNodes->ValueOf(adjNode);
		int x, y;
		AbstractBoard::ConvertToXy(x, y, adjNode);
		//core.PRT(_T("adj(%d,%d)LibRen=%d"), x, y, libertyOfRen);

		if (pBoard->ValueOf(adjNode) == opponent && libertyOfRen<4)
		{
			// ����̐΁i�܂��͘A�j�ŁA�ċz�_�� 3 �ӏ��ȉ��̕���I�т܂��B

			std::vector<int> openNodes = pBoard->GetOpenNodesOfStone(core, adjNode, libertyOfRen);
			//core.PRT(_T("�J%d"), openNodes.size());
			if (!openNodes.empty())
			{
				// ���̐΁i�A�ł͂Ȃ��j�̊J���Ă�������i�P�����`�R�����j������ꍇ�B

				if (openNodes.size() == 1)
				{
					//core.PRT(_T("Ate!"));

					// �A�^���@�̏�Ԃł��B
					if (score < 120)
					{
						// ���̎w����Ɂ@����Ƃ������肪�Ȃ��悤�Ȃ�A�A�e�ɂ����܂��傤�B
						score += 120;
					}
				}
				else {
					// �A�^���@�ł͂Ȃ��ꍇ�B
				}
			}
		}

		//core.PRT(_T(";"));
	});


	return score;
}
