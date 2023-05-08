/*****************************************************************
* File: Chess.cpp
 * Author: ­ð¤_µx¡B§d²»ºö
 * Create Date: 2023/05/05
 * Editor: ­ð¤_µx¡B§d²»ºö
 * Update Date: 2023/05/07
 * Description: set and update chess
 *********************************************************************/

#include "Chess.h"

//intent:constructor initialize the chess
//pre: none
//post:none
Chess::Chess() 
{
	pos = Position(0, 0);
	type = EMPTY;
	color = NONE;
}

//intent:constructor and initialize the chess
//pre:a Position a Label and a Color
//post:a object of Chess
Chess::Chess(Position pos, Type label, Color color)
{
	this->pos = pos;
	this->type = label;
	this->color = color;
	moved=false;
}

//intent:set position
//pre:a position
//post:none
void Chess::setPos(Position pos)
{
	this->pos = pos;
	moved=true;
}

//intent:set chess label and color
//pre: a Label and a Color
//post:none
void Chess::setChess(Type label, Color color) 
{
	this->type = label;
	this->color = color;
}

//intent:set the Label
//pre:the label 
//post:none
void Chess::setType(Type label)
{
	this->type = label;
}

//intent:set the space to the chess
//pre:a object of the Chess
//post:none
void Chess::setSpace(Chess chess) 
{
	this->color = chess.color;
	this->type = chess.type;
}

//intent:set the space to empty
//pre:none
//post:none
void Chess::setEmpty() 
{
	this->color = NONE;
	this->type = EMPTY;
}

//intent:get the Position of this class
//pre:none
//post:return the Promotion
Position Chess::getPos() 
{
	return pos;
}

//intent:get the of this chess
//pre:none
//post:return the object of Label
Type Chess::getType() 
{
	return type;
}

//intent:get the color of this chess
//pre:none
//post:return the object of Color
Color Chess::getColor() 
{
	return color;
}

bool Chess::getMoved()
{
	return moved;
}

//intent:check and change if the pawn is at the end
//pre: none
//post:none
void Chess::checkPawn() 
{
	//if the color is black
	if (this->color == BLACK) 
	{
		//pawn at the end
		if (this->pos.y == 7) 
		{
			//loop until promotion successfully
			while (this->type == PAWN) 
			{
				std::cout << "Promotion Pawn to 1.Queen 2.Bishop 3.Knight 4.Rook" << std::endl;
				int choose;
				std::cin >> choose;

				//what to chage to 
				switch (choose) 
				{

				//change to queen
				case 1: 
					this->setType(QUEEN);
					break;

				//change to bishop
				case 2: 
					this->setType(BISHOP);
					break;

				//change to knight
				case 3: 
					this->setType(KNIGHT);
					break;

				//change to rook
				case 4: 
					this->setType(ROOK);
					break;

				//fail to change
				default: 
					std::cout << "Fail" << std::endl;
					break;
				}
			}
			std::cout << "Success" << std::endl;
		}
	}
	else //if the color is white
	{
		//if at the end
		if (this->pos.y == 0) 
		{
			//loop until promotion successfully
			while (this->type == PAWN)
			{
				std::cout << "Promotion Pawn to 1.Queen 2.Bishop 3.Knight 4.Rook" << std::endl;
				int choose;
				std::cin >> choose;

				//what to chage to 
				switch (choose)
				{

				//change to queen
				case 1:
					this->setType(QUEEN);
					break;

				//change to bishop
				case 2:
					this->setType(BISHOP);
					break;

				//change to knight
				case 3:
					this->setType(KNIGHT);
					break;

				//change to rook
				case 4:
					this->setType(ROOK);
					break;

				//fail to change
				default:
					std::cout << "Fail" << std::endl;
					break;
				}
			}
			std::cout << "Success" << std::endl;
		}
	}
}

//intent:get all the valid position
//pre:none
//post:a vector full of valid positions
std::vector<Position> Chess::getValidPos()
{
	std::vector<Position> validPos;
	validPos.clear();
	Position targetPos;
	int side, pawnOrigin;
	Color enemy;
	if (color == BLACK)
	{
		enemy = WHITE;
		side = 1;
		pawnOrigin = 1;
	}
	else if (color == WHITE) //if the color is white
	{
		enemy = BLACK;
		side = -1;
		pawnOrigin = 6;
	}
	//judge the label
	switch (this->type)
	{
	case KING: //if is king

		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (pos.x + i < 8 && pos.x + i >= 0 && pos.y + j < 8 && pos.y + j >= 0)
				{
					targetPos = Position(pos.x + i, pos.y + j);
					validPos.push_back(targetPos);
				}
			}
		}
		break;

	case QUEEN: //if is queen

		for (int i = -7; i <= 7; i++)
		{
			if (pos.x + i < 8 && pos.x + i >= 0)
			{
				Position targetPos = Position(pos.x + i, pos.y);
				validPos.push_back(targetPos);
			}
			if (pos.y + i < 8 && pos.y + i >= 0)
			{
				Position targetPos = Position(pos.x, pos.y + i);
				validPos.push_back(targetPos);
			}
		}

	case BISHOP: //if is bishop

		for (int i = -7; i <= 7; i++)
		{
			if (pos.x + i < 8 && pos.x + i >= 0 && pos.y + i < 8 && pos.y + i >= 0)
			{
				Position targetPos = Position(pos.x + i, pos.y + i);
				validPos.push_back(targetPos);
			}
			if (pos.x + i < 8 && pos.x + i >= 0 && pos.y - i < 8 && pos.y - i >= 0)
			{
				Position targetPos = Position(pos.x + i, pos.y - i);
				validPos.push_back(targetPos);
			}
		}
		break;

	case KNIGHT: //if is knight

		{
			int turn[8][2] = { {2,-1},{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2} };
			for (int i = 0; i < 8; i++)
			{
				targetPos = Position(pos.x + turn[i][0], pos.y + turn[i][1]);
				validPos.push_back(targetPos);
			}
		}
		break;

	case ROOK:

		for (int i = -7; i <= 7; i++)
		{
			if (pos.x + i < 8 && pos.x + i >= 0)
			{
				Position targetPos = Position(pos.x + i, pos.y);
				validPos.push_back(targetPos);
			}
			if (pos.y + i < 8 && pos.y + i >= 0)
			{
				Position targetPos = Position(pos.x, pos.y + i);
				validPos.push_back(targetPos);
			}
		}
		break;

	case PAWN: //if is pawn

		//the first step, can move one or tow
		if (pos.y == pawnOrigin)
		{
			//can move one or two step, check if can move
			for (int i = 1; i <= 2; i++)
			{
				targetPos = Position(pos.x, (pos.y + (i * side)));
				validPos.push_back(targetPos);
			}
		}
		//not the first step, can move one step
		if (pos.y + 1 < 8 && pos.y - 1 >= 0)
		{
			targetPos = Position(pos.x, (pos.y + (1 * side)));
			validPos.push_back(targetPos);
		}
		break;
	}
	return validPos;
}