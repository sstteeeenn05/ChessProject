#pragma once
#include <vector>
#include "header.h"
#include "Position.h"

class Chess
{
protected:
	Position pos;
	Label label;
	Color color;

public:
	Chess();
	Chess(Position pos, Label label, Color color);
	//Chess(Chess baseChess);

	
	void setPos(Position pos);
	void setChess(Label label, Color color);
	void setSpace(Chess chess);
	void setEmpty();

	Position getPos();
	Label getLabel();
	Color getColor();
};
