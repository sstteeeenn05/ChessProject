#pragma once

class Position
{
public:

	int x;
	int y;

	Position();
	Position(int x, int y);

	void setPos(int x, int y); //set position

	Position getPos(); //get position

	Position operator+(const Position& pos); //set +
	void operator+= (const Position& pos); //set +=
};