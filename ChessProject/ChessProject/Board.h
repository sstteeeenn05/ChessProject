/*****************************************************************
* File: Board.h
 * Author: 唐于硯、吳祥綺
 * Create Date: 2023/05/05
 * Editor: 唐于硯、吳祥綺
 * Update Date: 2023/05/06
 * Description: prototype of the  board
 *********************************************************************/

#pragma once

#include "Chess.h"
#include "Log.h";
#include "Header.h"


class Board
{
private:
	Chess board[8][8];
	std::vector<Log> logs;
	int logIndex = 0;

public:
	Board(); //constructor and initialize the board
	//Board(std::string fen);

	std::string getBoard();
	std::string getMaskBoard(Position point);

	bool move(Player& player, Position source, Position target); //move
	bool moveAvalible(Chess& source, Chess& target); //check if can move

	bool canUndo();
	bool canRedo();
	bool undo(); //undo
	bool redo(); //redo
	int getLogIndex();

	bool castling(Position source, Position target);
	bool enPassant(Chess& chess, Position target, Log record);

	bool checkMovement(Player player);
	bool checkWin(Player player);
};

// moveAvalible 在 Board.h
//要吃棋子再去movaAvalible 比較
