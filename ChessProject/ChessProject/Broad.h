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

//move  getValidPos ��� chess.h
// moveAvalible �b Broad.h
//getValidPos �������u�s�i�H�ʪ�(���O�Ū�)
//�n�Y�Ѥl�A�hmovaAvalible ���
