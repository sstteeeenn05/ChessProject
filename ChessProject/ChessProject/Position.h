#pragma once

class Position
{
public:

	int x;
	int y;

	Position(); //constructor, initialize position
	Position(int x, int y); //constructor, initialize position

	void setPos(int x, int y); //set position

	Position getPos(); //get position

	Position operator+(const Position& pos); //set +
	void operator+= (const Position& pos); //set +=
};