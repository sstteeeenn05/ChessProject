/*****************************************************************
* File: Position.cpp
 * Author: ­ğ¤_µx¡B§d²»ºö
 * Create Date: 2023/05/05
 * Editor: ­ğ¤_µx¡B§d²»ºö
 * Update Date: 2023/05/07
 * Description: set and update Position
 *********************************************************************/

#include "Position.h"

Position::Position() //constructor and initialize position
{
	this->x = 0;
	this->y = 0;
}

Position::Position(int x, int y) //constructor and initialize position
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

bool Position::operator==(Position& pos) //oveload operation ==
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
