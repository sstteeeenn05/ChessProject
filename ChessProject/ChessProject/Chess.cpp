#include "Chess.h"

Chess::Chess()
{
	pos = Position(0, 0);
	label = EMPTY;
	color = NONE;
}

Chess::Chess(Position pos, Label label, Color color)
{
	this->pos = pos;
	this->label = label;
	this->color = color;
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

void Chess::setSpace(Chess chess)
{
	this->color = chess.color;
	this->label = chess.label;
}

void Chess::setEmpty()
{
	this->color = NONE;
	this->label = EMPTY;
}

Position Chess::getPos()
{
	return pos;
}

Label Chess::getLabel()
{
	return label;
}

Color Chess::getColor()
{
	return color;
}

