/*****************************************************************
* File: Board.h
 * Author: ��_�x�B�d����
 * Create Date: 2023/05/05
 * Editor: ��_�x�B�d����
 * Update Date: 2023/05/06
 * Description: prototype of the  board
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

	void printBoard();

	bool move(Player& player, Position source, Position target, const int& count); //move
	bool moveAvalible(Chess chess, Position target); //check if can move

	Color colorOfPosition(int x, int y);

	void undo(int& count); //undo
	void redo(int& count); //redo

	bool castling(Chess& chess, Position target);
	bool enPassant(Chess& chess, Position target, Log record);

	bool checkMovement(Player player);
	bool checkWin(Player player);
};

// moveAvalible �b Board.h
//�n�Y�Ѥl�A�hmovaAvalible ���
