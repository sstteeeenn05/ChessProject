#pragma once

#include "Chess.h"
#include "header.h"

class Board
{
private:
	Chess board[8][8];

public:
	Board(); //constructor, initialize the board
	//Board(std::string fen);

	Chess* drawBoard(); //let can draw board

	std::vector<Position> getValidPos(Chess chess); //get all the valid position
};