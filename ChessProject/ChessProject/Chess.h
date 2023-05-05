#pragma once
#include "header.h"
#include "Position.h"

class Chess
{
protected:
	Position pos;
	Label label;
	Color color;

public:
	Chess(); //constructor, initialize the chess
	Chess(Position pos, Label label, Color color); //constructor, initialize the chess
	//Chess(Chess baseChess);

	
	void setPos(Position pos); //set position
	void setChess(Label label, Color color); //set chess label and color
	void setSpace(Chess chess); //set the space to the chess
	void setEmpty(); //set the space to empty

	Position getPos(); //get position
	Label getLabel(); //get label
	Color getColor(); //get color
};
