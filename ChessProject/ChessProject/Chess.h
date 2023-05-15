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
	void setMoved(); 
	static void setWhiteKingPos(Position pos);
	static void setBlackKingPos(Position pos);

	Position getPos(); //get position
	Type getType(); //get type
	Player getPlayer(); //get color
	bool getMoved();
	static Position getWhiteKingPos();
	static Position getBlackKingPos();

	bool checkPromotion(); //check and change if the pawn is at end lines
	void doPromotion();

	std::vector<Position>getValidPos(Chess board[8][8]);

	bool checkCheck(Player player, Position pos, Chess board[8][8]);

	void strightLine(Chess board[8][8], std::vector<Position>& validPos); //find all the valid places on the straight line
	void crossLine(Chess board[8][8], std::vector<Position>& validPos); //find all the valid places on the cross line
	void nineSquare(Chess board[8][8], std::vector<Position>& validPos); //find all the valid places in the nine square
	void lLine(Chess board[8][8], std::vector<Position>& validPos); //find all the valid places on L line
};
