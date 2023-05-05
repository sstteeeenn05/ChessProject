#pragma once

#include "Chess.h"
<<<<<<< Updated upstream
#include "Header.h"
=======
<<<<<<< HEAD
#include "header.h"
#include "Player.h"
=======
#include "Header.h"
>>>>>>> f51c2130c90e0368030ad528c27f0042d58ff3b5
>>>>>>> Stashed changes

class Board
{
private:
	Chess board[8][8];

public:
	Board(); //constructor, initialize the board
	//Board(std::string fen);

	Chess* getBoard(); //get board
	std::vector<Position> getValidPos(Chess chess); //get all the valid position
	void move(Player& player); //move
	bool moveAvalible(Chess chess, Position target); //check if can move
};