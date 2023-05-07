/*****************************************************************
* File: Player.cpp
 * Author: �d����
 * Create Date: 2023/05/06
 * Editor: �d����
 * Update Date: 2023/05/06
 * Description: prototype of the  Player
 *********************************************************************/

#include "Player.h"

Player::Player() //constructor and initialize the player
{
	this->color = NONE;
}

Player::Player(Color color) //constructor and initialize the player
{
	this->color = color;
}

void Player::setColor(Color color) //set color
{
	this->color = color;
}

Color Player::getColor() //get color
{
	return this->color;
}