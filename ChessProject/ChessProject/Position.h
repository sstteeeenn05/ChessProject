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

	bool operator==(Position&); //set ==
};