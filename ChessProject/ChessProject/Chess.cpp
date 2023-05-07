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
	label = EMPTY;
	color = NONE;
}

//intent:constructor and initialize the chess
//pre:a Position a Label and a Color
//post:a object of Chess
Chess::Chess(Position pos, Type label, Color color)
{
	this->pos = pos;
	this->label = label;
	this->color = color;
}

//intent:set position
//pre:a position
//post:none
void Chess::setPos(Position pos)
{
	this->pos = pos;
}

//intent:set chess label and color
//pre: a Label and a Color
//post:none
void Chess::setChess(Type label, Color color) 
{
	this->label = label;
	this->color = color;
}

//intent:set the Label
//pre:the label 
//post:none
void Chess::setLabel(Type label)
{
	this->label = label;
}

//intent:set the space to the chess
//pre:a object of the Chess
//post:none
void Chess::setSpace(Chess chess) 
{
	this->color = chess.color;
	this->label = chess.label;
}

//intent:set the space to empty
//pre:none
//post:none
void Chess::setEmpty() 
{
	this->color = NONE;
	this->label = EMPTY;
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
Type Chess::getLabel() 
{
	return label;
}

//intent:get the color of this chess
//pre:none
//post:return the object of Color
Color Chess::getColor() 
{
	return color;
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
			while (this->label == PAWN) 
			{
				std::cout << "Promotion Pawn to 1.Queen 2.Bishop 3.Knight 4.Rook" << std::endl;
				int choose;
				std::cin >> choose;

				//what to chage to 
				switch (choose) 
				{

				//change to queen
				case 1: 
					this->setLabel(QUEEN);
					break;

				//change to bishop
				case 2: 
					this->setLabel(BISHOP);
					break;

				//change to knight
				case 3: 
					this->setLabel(KNIGHT);
					break;

				//change to rook
				case 4: 
					this->setLabel(ROOK);
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
			while (this->label == PAWN)
			{
				std::cout << "Promotion Pawn to 1.Queen 2.Bishop 3.Knight 4.Rook" << std::endl;
				int choose;
				std::cin >> choose;

				//what to chage to 
				switch (choose)
				{

				//change to queen
				case 1:
					this->setLabel(QUEEN);
					break;

				//change to bishop
				case 2:
					this->setLabel(BISHOP);
					break;

				//change to knight
				case 3:
					this->setLabel(KNIGHT);
					break;

				//change to rook
				case 4:
					this->setLabel(ROOK);
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