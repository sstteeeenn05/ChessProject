/*****************************************************************
* File: Chess.h
 * Author: ��_�x�B�d����
 * Create Date: 2023/05/05
 * Editor: ��_�x�B�d����
 * Update Date: 2023/05/07
 * Description: prototype of the chess
 *********************************************************************/

#pragma once

#include "Header.h"
#include "Player.h"
#include "Position.h"

class Chess
{
private:
	Position pos;
	Type type;
	Color color;
	bool moved;
	static Position whiteKing, blackKing;
public:
	Chess(); //constructor and initialize the chess
	Chess(Position pos, Type type, Color color); //constructor and initialize the chess
	//Chess(Chess baseChess);
	
	void setPos(Position pos); //set position
	void setChess(Type type, Color color); //set chess type and color
	void setType(Type type); //set type
	void setSpace(Chess chess); //set the space to the chess
	void setEmpty(); //set the space to empty
	void setMoved();
	static void setWhiteKingPos(Position pos);
	static void setBlackKingPos(Position pos);

	Position getPos(); //get position
	Type getType(); //get type
	Color getColor(); //get color
	bool getMoved();
	static Position getWhiteKingPos();
	static Position getBlackKingPos();

	void checkPawn(); //check and change if the pawn is at end lines

	std::vector<Position>getValidPos(Chess board[8][8]);

	bool checkCheck(Color color, Position pos, Chess board[8][8]);

	void strightLine(Chess board[8][8], std::vector<Position>& validPos);
	void crossLine(Chess board[8][8], std::vector<Position>& validPos);
	void nineSquare(Chess board[8][8], std::vector<Position>& validPos);
	void lLine(Chess board[8][8], std::vector<Position>& validPos);
};
