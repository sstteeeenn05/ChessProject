/*****************************************************************
* File: Chess.h
 * Author: ­ð¤_µx¡B§d²»ºö
 * Create Date: 2023/05/05
 * Editor: ­ð¤_µx¡B§d²»ºö
 * Update Date: 2023/05/07
 * Description: prototype of the chess
 *********************************************************************/

#pragma once

#include "Header.h"
#include "Player.h"
#include "Position.h"

class Chess
{
private:
	Position pos;
	Type type;
	Color color;

	//Player& player;

public:
	Chess(); //constructor and initialize the chess
	Chess(Position pos, Type type, Color color); //constructor and initialize the chess
	//Chess(Chess baseChess);
	
	void setPos(Position pos); //set position
	void setChess(Type type, Color color); //set chess type and color
	void setLabel(Type type); //set type
	void setSpace(Chess chess); //set the space to the chess
	void setEmpty(); //set the space to empty

	Position getPos(); //get position
	Type getLabel(); //get type
	Color getColor(); //get color

	void checkPawn(); //check and change if the pawn is at end lines
	std::vector<Position>getValidPos();
};
