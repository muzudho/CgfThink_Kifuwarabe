#pragma once


#define BOARD_MAX ((19+2)*256)	// 19�H�Ղ��ő�T�C�Y�Ƃ���


class AbstractBoard {
protected:
	// �Տ�̐΂̐F�B
	int table[BOARD_MAX];

	// ���H��
	int size;

public:
	int*	GetTable();

	void	SetSize(int size);
	int		GetSize();

	// �Z���̒l
	void	SetValue(int node, int value);
	int		ValueOf(int node);
};