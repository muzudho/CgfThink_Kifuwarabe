#include "../../header/h190_board___/n190_150_liberty.h"
#include "../../header/h190_board___/n190_250_markingBoard.h"
#include "..\..\header\h675_hit_____\n675_200_hitNobiSaver.h"

int HitNobiSaver::Evaluate(
	Core core, int color, int node, Board * pBoard, LibertyOfNodes * pLibertyOfNodes, MarkingBoard*	pMarkingBoard
)
{
	int score = 0;

	// �J�����g�ʒu���}�[�N���܂��B
	pMarkingBoard->SetValue(node, 1);

	// �܂��@�ċz�_���P�A
	// ���Ɂ@�ċz�_���Q�A
	// �ƁA�ċz�_�̏��Ȃ������珇�ɕ]����t���Ă����܂��B
	// �Ֆʂ��S��T����邱�ƂɂȂ�܂��B
	//
	for (int iExpectedLiberty = 1; iExpectedLiberty < 4; iExpectedLiberty++)
	{
		pBoard->ForeachAllNodesWithoutWaku([this,&core,&pBoard,&pMarkingBoard,&pLibertyOfNodes,&score, color, iExpectedLiberty](int node, bool& isBreak) {

			int currentLiberty = pLibertyOfNodes->ValueOf(node); // �΂�u���O�̌ċz�_�̐��B

			if (
				pBoard->ValueOf(node) == color // ��������Ă��邩������Ȃ������̐�
				&&
				currentLiberty == iExpectedLiberty // �ċz�_�̐�
			)
			{
				// ��A�E�A���A���𒲂ׂ܂��B
				pBoard->ForeachArroundNodes(node, [this,&core,&pBoard,&pLibertyOfNodes,&pMarkingBoard,&score, color, node, currentLiberty](int adjNode, bool& isBreak) {
					int adjColor = pBoard->ValueOf(adjNode);
					if (adjColor == EMPTY)
					{
						// �킽���̐΂̖k�ׂɂ���󂫃X�y�[�X�̈ʒu�B

						// �󂫃X�y�[�X�ɐ΂�u�����ƍl���āA�΂�u�����ǖʂ̂��̎����̐΁i�܂��͘A�j�̌ċz�_�𐔂��܂��B
						Liberty futureLiberty;
						futureLiberty.Count(adjNode, color, pBoard);

						// �]���l�v�Z
						if (futureLiberty.liberty <= currentLiberty)
						{
							// �c�P�ā@�ċz�_�������Ă���悤�ł͘b���ɂȂ�܂���B
							//score += 0;
						}
						else
						{
							// �c�P�ā@�ċz�_�������Ă���̂ŁA�ǂꂾ�����������𐔂��܂��B
							int upLiberty = futureLiberty.liberty - currentLiberty;

							score += 40  // 40����{�ɁB
								+
								(upLiberty - 1) * 50    // �ċz�_���Q�ȏ㑝����Ȃ�A
														// �ċz�_���P�����邽�т� 50 �_�̃{�[�i�X�B
								/
								(currentLiberty * currentLiberty * currentLiberty)  // �c�P��O�̌ċz�_�̐����傫���قǁA
																					// �X�R�A������i�ً}�̊֐S�𔖂ꂳ����j�d�|���B
																					// �ċz�_ 1 = 1�^1 �_
																					// �ċz�_ 2 = 1�^8 �_
																					// �ċz�_ 3 = 1�^27 �_
																					// �ċz�_ 4 = 1�^64 �_
								;
						}

						//found = true;
					}
					else if
					(
						adjColor == color // �k�ׂ������̐΂ŁA
						&&
						pMarkingBoard->ValueOf(adjNode) == 0 // �k�ׂ̃}�[�L���O�� 0 �Ȃ�
					)
					{
						// �ċA�Ăяo��
						score = this->Evaluate(core, adjColor, adjNode, pBoard, pLibertyOfNodes, pMarkingBoard);
						if ( 0 < score )
						{
							//found = true;
						}
					}

				});
			}

		});
	}

	return score;
}
