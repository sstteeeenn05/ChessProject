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
	std::vector<Position> validPos;

public:
	Chess();
	Chess(Position pos, Label label, Color color, std::vector<Position> validPos);
	//Chess(Chess baseChess);

	Position getPos();
	void setPos(Position pos);

	void setChess(Label label, Color color);

	std::vector< Position> getValidPos();
	Label getLabel();
	Color getColor();
};
