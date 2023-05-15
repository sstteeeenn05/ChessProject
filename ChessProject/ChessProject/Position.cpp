/*****************************************************************
* File: Position.cpp
 * Author: ­ğ¤_µx¡B§d²»ºö
 * Create Date: 2023/05/05
 * Editor: ­ğ¤_µx¡B§d²»ºö
 * Update Date: 2023/05/07
 * Description: set and update Position
 *********************************************************************/

#include "Position.h"

//intent:constructor and initialize position
//pre:none
//post:none
Position::Position() 
{
	this->x = 0;
	this->y = 0;
}

//intent:constructor and initialize position
//pre:x and y
//post:none
Position::Position(int x, int y)
{
	this->x = x;
	this->y = y;
}

//intent:set position
//pre:x and y
//post:none
void Position::setPos(int x, int y) 
{
	this->x = x;
	this->y = y;
}

//intent:get position
//pre:none
//post:a class of position
Position Position::getPos() //get position
{
	return *this;
}

//intent:overload operation ==
//pre:a reference of the position
//post:true if the two are the same
bool Position::operator==(Position& pos) 
{
	//if one of x or y is different
	if (this->x != pos.x || this->y != pos.y)
	{
		return false;
	}
	else
	{
		return true;
	}
}
