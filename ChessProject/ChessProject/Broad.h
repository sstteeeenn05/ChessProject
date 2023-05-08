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
#include "Log.h";
#include "Player.h"
#include "Header.h"

class Board
{
private:
	Chess board[8][8];
	std::vector<Log> logs;

public:
	Board(); //constructor and initialize the board
	//Board(std::string fen);

	Chess* getBoard(); //get board

	bool move(Player& player, Position source, Position target); //move
	bool moveAvalible(Chess chess, Position target); //check if can move
	void undo(int& count); //undo
	void redo(int& count); //redo

	bool castling(Chess& chess);
};

// moveAvalible 在 Broad.h
//要吃棋子再去movaAvalible 比較
