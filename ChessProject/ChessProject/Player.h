#pragma once
#include "Header.h"


class Player
{
public:
	Color color;

	Player(); //constructor and initialize the player
	Player(Color color); //constructor and initialize the player
	
	void setColor(Color color); //set color
	Color getColor(); //get color
};