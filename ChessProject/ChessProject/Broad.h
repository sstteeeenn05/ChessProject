/*****************************************************************
* File: Broad.h
 * Author: ��_�x�B�d����
 * Create Date: 2023/05/05
 * Editor: ��_�x�B�d����
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

// moveAvalible �b Broad.h
//�n�Y�Ѥl�A�hmovaAvalible ���
