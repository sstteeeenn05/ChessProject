#include "Player.h"

Player::Player() //constructor, initialize the player
{
	this->color = NONE;
	this->status = CONTINUE;
}

Player::Player(Color color) //constructor, initialize the player
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