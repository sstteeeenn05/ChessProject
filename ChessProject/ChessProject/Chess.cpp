#include "Chess.h"

Chess::Chess() //constructor, initialize the chess
{
	pos = Position(0, 0);
	label = EMPTY;
	color = NONE;
}

Chess::Chess(Position pos, Label label, Color color) //constructor, initialize the chess
{
	this->pos = pos;
	this->label = label;
	this->color = color;
}

void Chess::setPos(Position pos) //set position
{
	this->pos = pos;
}

void Chess::setChess(Label label, Color color) //set chess label and color
{
	this->label = label;
	this->color = color;
}

void Chess::setLabel(Label label) //set label
{
	this->label = label;
}

void Chess::setSpace(Chess chess) //set the space to the chess
{
	this->color = chess.color;
	this->label = chess.label;
}

void Chess::setEmpty() //set the space to empty
{
	this->color = NONE;
	this->label = EMPTY;
}

Position Chess::getPos() //get position
{
	return pos;
}

Label Chess::getLabel() //get label
{
	return label;
}

Color Chess::getColor() //get color
{
	return color;
}

void Chess::checkPawn() //check and change if the pawn at the end
{
	if (this->color == BLACK) //if the color is black
	{
		if (this->pos.y == 7) //at the end
		{
			while (this->label == PAWN) //has not promotion
			{
				std::cout << "Promotion Pawn to 1.Queen 2.Bishop 3.Knight 4.Rook" << std::endl;
				int choose;
				std::cin >> choose;
				switch (choose) //change to what
				{
				case 1: //change to queen
					this->setLabel(QUEEN);
					break;
				case 2: //change to bishop
					this->setLabel(BISHOP);
					break;
				case 3: //change to knight
					this->setLabel(KNIGHT);
					break;
				case 4: //change to rook
					this->setLabel(ROOK);
					break;
				default: //change fail
					std::cout << "Fail" << std::endl;
					break;
				}
			}
			std::cout << "Success" << std::endl;
		}
	}
	else //if the color is white
	{
		if (this->pos.y == 0) //if at the end
		{
			while (this->label == PAWN) //has not promotion
			{
				std::cout << "Promotion Pawn to 1.Queen 2.Bishop 3.Knight 4.Rook" << std::endl;
				int choose;
				std::cin >> choose;
				switch (choose) //change to what
				{
				case 1: //change to queen
					this->setLabel(QUEEN);
					break;
				case 2: //change to bishop
					this->setLabel(BISHOP);
					break;
				case 3: //change to knight
					this->setLabel(KNIGHT);
					break;
				case 4: //change to rook
					this->setLabel(ROOK);
					break;
				default: //change fail
					std::cout << "Fail" << std::endl;
					break;
				}
			}
			std::cout << "Success" << std::endl;
		}
	}
}