#include "Chess.h"

Chess::Chess()
{
	pos = Position(0, 0);
	label = EMPTY;
	color = NONE;
	validPos.clear();
}

Chess::Chess(Position pos, Label label, Color color, std::vector<Position> validPos)
{
	this->pos = pos;
	this->label = label;
	this->color = color;
	this->validPos = validPos;
}

std::vector< Position> Chess::getValidPos()
{
	return validPos;
}

Position Chess::getPos()
{
	return pos;
}

void Chess::setPos(Position pos)
{
	this->pos = pos;
}

void Chess::setChess(Label label, Color color)
{
	this->label = label;
	this->color = color;
}

Label Chess::getLabel()
{
	return label;
}

Color Chess::getColor()
{
	return color;
}

