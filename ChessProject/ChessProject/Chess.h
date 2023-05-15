/*****************************************************************
* File: Chess.h
 * Author: ­ð¤_µx¡B§d²»ºö
 * Create Date: 2023/05/05
 * Editor: ­ð¤_µx¡B§d²»ºö
 * Update Date: 2023/05/07
 * Description: prototype of the chess
 *********************************************************************/

#pragma once

#include "Header.h"
#include "Position.h"

class Chess
{
private:
	Position pos;
	Type type;
	Player player;
	bool moved;
	static Position whiteKing, blackKing;

public:
	Chess(); //constructor and initialize the chess
	Chess(Position pos, Type type, Player player); //constructor and initialize the chess
	
	void setPos(Position pos); //set position
	void setChess(Type type, Player player); //set chess type and color
	void setType(Type type); //set type
	void setSpace(Chess chess); //set the space to the chess
	void setEmpty(); //set the space to empty
	void setMoved(); //if the chess have moved set moved to true
	static void setWhiteKingPos(Position pos); //set the white king to the position
	static void setBlackKingPos(Position pos); //set the black king to the position

	Position getPos(); //get position
	Type getType(); //get type
	Player getPlayer(); //get color
	bool getMoved(); //check if this chess has moved before
	static Position getWhiteKingPos(); //get the position of the white king
	static Position getBlackKingPos(); //get the position of the black king

	bool checkPromotion(); //check and change if the pawn is at end lines
	void doPromotion(); //check and change if the pawn is at the end

	std::vector<Position>getValidPos(Chess board[8][8]); //get all the valid position

	bool checkCheck(Player player, Position pos, Chess board[8][8]); //check if the king is being check from the king's perspective

	void strightLine(Chess board[8][8], std::vector<Position>& validPos); //find all the valid places on the straight line
	void crossLine(Chess board[8][8], std::vector<Position>& validPos); //find all the valid places on the cross line
	void nineSquare(Chess board[8][8], std::vector<Position>& validPos); //find all the valid places in the nine square
	void lLine(Chess board[8][8], std::vector<Position>& validPos); //find all the valid places on L line
};
