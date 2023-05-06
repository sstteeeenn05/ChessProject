#pragma once
#include "Header.h"


class Player
{
public:
	Color color;

	Player(); //constructor, initialize the player
	Player(Color color); //constructor, initialize the player
	
	void setColor(Color color); //set color
	Color getColor(); //get color
};