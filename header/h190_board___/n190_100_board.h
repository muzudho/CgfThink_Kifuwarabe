#pragma once

//--------------------------------------------------------------------------------
// ��`
//--------------------------------------------------------------------------------

// ���
#define EMPTY 0		// �󂫃X�y�[�X
#define BLACK 1
#define WHITE 2
#define WAKU  3		// �ՊO

#define BOARD_MAX ((19+2)*256)	// 19�H�Ղ��ő�T�C�Y�Ƃ���
#define INVCLR(x) (3-(x))		// �΂̐F�𔽓]������


class Board {
public:
	// �Տ�̐΂̐F�B
	int table[BOARD_MAX];

	int size;

	// ��A�E�A���A���@�Ɉړ�����̂Ɏg�������l
	int dir4[4] = {
		-0x100,	// ��
		+0x001,	// �E
		+0x100,	// ��
		-0x001	// ��
	};// �I���W�i����cgfthink�ł͉E�A���A���A��̏��������B

	// ���ɃR�E�ɂȂ�ʒu�B������� 0�B
	int kouNode;

	// �n�}�B������΂̐��̂��ƁB[0]...�󂫁B[1]... ����������΂̐�, [2]...����������΂̐�
	int hama[3];

public:

	// (x,y)��1�̍��W�ɕϊ�
	static int ConvertToNode(
		int x,
		int y
	);

	// (node)��(x,y)���W�ɕϊ�
	static void ConvertToXy(
		int& x,
		int& y,
		int node
	);

	Board();
	~Board();

	// �A�ɂȂ��Ă���΂������B�P�̐΂ł������B
	void	DeleteRenStones(
		int tNode,
		int color
	);

	// �g���܂߂Č�Ղ�S����
	// .cpp �ɖ{�̂������ƂȂ񂩃G���[���o���̂ŁA.h �ɏ����Ă���񂾂����i�O���O�j
	template<typename Func> void ForeachAllNodesWithWaku(Func func)
	{
		for (int x = 0; x < this->size + 2; x++)
		{
			for (int y = 0; y < this->size + 2; y++)
			{
				int node = Board::ConvertToNode(x, y);

				bool isBreak = false;
				func(node, isBreak);
				if (isBreak)
				{
					break;
				}
			}
		}
	}

	// �g���܂߂Č�Ղ�S����
	// .cpp �ɖ{�̂������ƂȂ񂩃G���[���o���̂ŁA.h �ɏ����Ă���񂾂����i�O���O�j
	template<typename Func> void ForeachAllXyWithWaku(Func func)
	{
		for (int x = 0; x < this->size + 2; x++)
		{
			for (int y = 0; y < this->size + 2; y++)
			{
				bool isBreak = false;
				func(x,y, isBreak);
				if (isBreak)
				{
					break;
				}
			}
		}
	}

	// �g���܂߂Ȃ���Ղ�S����
	// .cpp �ɖ{�̂������ƂȂ񂩃G���[���o���̂ŁA.h �ɏ����Ă���񂾂����i�O���O�j
	template<typename Func> void ForeachAllNodesWithoutWaku( Func func)
	{
		for (int x = 1; x < this->size + 1; x++)
		{
			for (int y = 1; y < this->size + 1; y++)
			{
				int node = Board::ConvertToNode(x, y);

				bool isBreak = false;
				func(node, isBreak);
				if (isBreak)
				{
					break;
				}
			}
		}
	}

	// �w��̃m�[�h�ɗאڂ����A�E�A���A���̃m�[�h�𑖍��B
	template<typename Func> void ForeachArroundNodes(int node, Func func)
	{
		for (int iDir = 0; iDir < 4; iDir++) {
			bool isBreak = false;
			func(node + this->dir4[iDir], isBreak);
			if (isBreak)
			{
				break;
			}
		}
	}

	// �w��̃m�[�h�ɗאڂ����A�E�A���A���̃m�[�h�𑖍��B
	template<typename Func> void ForeachArroundDirAndNodes(int node, Func func)
	{
		for (int iDir = 0; iDir < 4; iDir++) {
			bool isBreak = false;
			func(iDir, node + this->dir4[iDir], isBreak);
			if (isBreak)
			{
				break;
			}
		}
	}

};