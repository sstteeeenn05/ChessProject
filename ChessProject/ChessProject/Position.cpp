#include "Position.h"

Position::Position() //constructor, initialize position
{
	this->x = 0;
	this->y = 0;
}

Position::Position(int x, int y) //constructor, initialize position
{
	this->x = x;
	this->y = y;
}

void Position::setPos(int x, int y) //set position
{
	this->x = x;
	this->y = y;
}

Position Position::getPos() //get position
{
	return *this;
}

bool Position::operator==(Position& pos) //set ==
{
	if (this->x != pos.x || this->y != pos.y)
	{
		return false;
	}
	else
	{
		return true;
	}
}