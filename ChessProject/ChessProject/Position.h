#pragma once

class Position
{
private:
	int x;
	int y;

public:
	Position();
	Position(int x, int y);
	void setPos(int x, int y); //set position
	Position getPos(); //get position
	Position operator+(const Position& pos); //set +
	void operator+= (const Position& pos); //set +=
};