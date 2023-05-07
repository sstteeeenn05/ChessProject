/*****************************************************************
* File: Player.h
 * Author: §d²»ºö
 * Create Date: 2023/05/06
 * Editor: §d²»ºö
 * Update Date: 2023/05/06
 * Description: set and get Player
 *********************************************************************/

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