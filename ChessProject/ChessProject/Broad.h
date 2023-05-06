#pragma once

#include "Chess.h"
#include "Header.h"
#include "header.h"
#include "Player.h"
#include "Header.h"

class Board
{
private:
	Chess board[8][8];

public:
	Board(); //constructor and initialize the board
	//Board(std::string fen);

	Chess* getBoard(); //get board
	std::vector<Position> getValidPos(Chess chess); //get all the valid position

	void move(Player& player); //move
	bool moveAvalible(Chess chess, Position target); //check if can move
};
