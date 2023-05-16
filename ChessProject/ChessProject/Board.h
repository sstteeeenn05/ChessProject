/*****************************************************************
* File: Board.h
 * Author: ­ð¤_µx¡B§d²»ºö
 * Create Date: 2023/05/05
 * Editor: ­ð¤_µx¡B§d²»ºö
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

	std::string getBoard(); //get all the chess on the board 
	std::string getMaskBoard(Position point);

	bool move(Player& player, Position source, Position target); //move the chess
	bool moveAvalible(Chess& source, Chess& target); //check if this position is valid

	bool canUndo(); //check if there is any steps before
	bool canRedo(); //check if there is any steps after
	bool undo(); //undo the last step
	bool redo(); //redo the next step
	int getLogIndex(); //get the index of log

	bool castling(Position source, Position target); //check if king can castling and move the pawn
	bool enPassant(Chess& chess, Position target, Log record); //check if pawn can enPassant and eat the enermy pawn

	bool checkMovement(Player player); //check if the king is being check by checking all the same color chess on the board
	bool checkWin(Player player); //check if the player win
};