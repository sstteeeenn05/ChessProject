#include "Chess.h"

Chess::Chess() //constructor, initialize the chess
{
	pos = Position(0, 0);
	label = EMPTY;
	color = NONE;
}

Chess::Chess(Position pos, Label label, Color color) //constructor, initialize the chess
{
	this->pos = pos;
	this->label = label;
	this->color = color;
}

void Chess::setPos(Position pos) //set position
{
	this->pos = pos;
}

void Chess::setChess(Label label, Color color) //set chess label and color
{
	this->label = label;
	this->color = color;
}

void Chess::setSpace(Chess chess) //set the space to the chess
{
	this->color = chess.color;
	this->label = chess.label;
}

void Chess::setEmpty() //set the space to empty
{
	this->color = NONE;
	this->label = EMPTY;
}

Position Chess::getPos() //get position
{
	return pos;
}

Label Chess::getLabel() //get label
{
	return label;
}

Color Chess::getColor() //get color
{
	return color;
}

