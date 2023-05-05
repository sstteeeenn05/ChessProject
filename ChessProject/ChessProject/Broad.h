#pragma once

#include "Chess.h"
#include "header.h"

class Board
{
private:
	Chess board[8][8];

public:
	Board();
	//Board(std::string fen);

	Chess* drawBoard();

	std::vector<Position> getValidPos(Chess chess);
};