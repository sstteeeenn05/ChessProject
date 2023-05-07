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
#include "Position.h"

class Chess
{
protected:
	Position pos;
	Label label;
	Color color;

public:
	Chess(); //constructor and initialize the chess
	Chess(Position pos, Label label, Color color); //constructor and initialize the chess
	//Chess(Chess baseChess);
	
	void setPos(Position pos); //set position
	void setChess(Label label, Color color); //set chess label and color
	void setLabel(Label label); //set label
	void setSpace(Chess chess); //set the space to the chess
	void setEmpty(); //set the space to empty

	Position getPos(); //get position
	Label getLabel(); //get label
	Color getColor(); //get color

	void checkPawn(); //check and change if the pawn is at end lines
};
