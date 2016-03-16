#include "..\..\header\h190_board___\n190_050_abstractBoard.h"

int AbstractBoard::ConvertToNode(int x, int y)
{
	return y * 256 + x;
}

void AbstractBoard::ConvertToXy(int& x, int& y, int node)
{
	y = node / 256;
	x = node % 256;
}

AbstractBoard::AbstractBoard()
{
	this->size = 0;
}

AbstractBoard::~AbstractBoard()
{
}

int* AbstractBoard::GetTable()
{
	return this->table;
}

void AbstractBoard::SetSize(int size)
{
	this->size = size;
}

int AbstractBoard::GetSize()
{
	return this->size;
}

void AbstractBoard::SetValue(int node, int value)
{
	this->table[node] = value;
}

int AbstractBoard::ValueOf(int node)
{
	return this->table[node];
}
