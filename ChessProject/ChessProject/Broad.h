/*****************************************************************
* File: Broad.h
 * Author: 唐于硯、吳祥綺
 * Create Date: 2023/05/05
 * Editor: 唐于硯、吳祥綺
 * Update Date: 2023/05/06
 * Description: prototype of the  broad
 *********************************************************************/

#pragma once

#include "Chess.h"
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

//move  getValidPos 改到 chess.h
// moveAvalible 在 Broad.h
//getValidPos 版面內只存可以動的(都是空的)
//要吃棋子再去movaAvalible 比較
