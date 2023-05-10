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
	moved = false;
}

Position Chess::whiteKing = Position(4, 7);
Position Chess::blackKing = Position(4, 0);

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

void Chess::setMoved()
{
	moved = true;
}

void Chess::setWhiteKingPos(Position pos)
{
	whiteKing = pos;
}

void Chess::setBlackKingPos(Position pos)
{
	blackKing = pos;
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

Position Chess::getWhiteKingPos()
{
	return whiteKing;
}

Position Chess::getBlackKingPos()
{
	return blackKing;
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

bool Chess::checkCheck(Color color, Position pos, Chess board[8][8])
{
	Color enemy;
	int side;
	if (color == BLACK)
	{
		enemy = WHITE;
		side = 1;
	}
	else if (color == WHITE) //if the color is white
	{
		enemy = BLACK;
		side = -1;
	}
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (pos.x + i < 8 && pos.x + i >= 0 && pos.y + j < 8 && pos.y + j >= 0
				&& color != board[pos.y + j][pos.x + i].getColor() && board[pos.y + j][pos.x + i].getType() == KING)
			{
				return true;
			}
		}
	}
	int turn[8][2] = { {2,-1},{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2} };
	for (int i = 0; i < 8; i++)
	{
		if (pos.x + turn[i][0] < 8 && pos.x + turn[i][0] >= 0 && pos.y + turn[i][1] < 8 && pos.y + turn[i][1] >= 0
			&& color != board[pos.y + turn[i][1]][pos.x + turn[i][0]].getColor() && board[pos.y + turn[i][1]][pos.x + turn[i][0]].getType() == KNIGHT)
		{
			return true;
		}
	}
	int i = 1;
	while (pos.x + i < 8 && pos.x + i >= 0 && pos.y + i < 8 && pos.y + i >= 0
		&& color != board[pos.y + i][pos.x + i].getColor())
	{
		if ((board[pos.y + i][pos.x + i].getType() == QUEEN || board[pos.y + i][pos.x + i].getType() == BISHOP))
		{
			return true;
		}
		if (board[pos.y + i][pos.x + i].getColor() == enemy)
		{
			break;
		}
		i++;
	}
	i = 1;
	while (pos.x + i < 8 && pos.x + i >= 0 && pos.y - i < 8 && pos.y - i >= 0
		&& color != board[pos.y - i][pos.x + i].getColor())
	{
		if ((board[pos.y - i][pos.x + i].getType() == QUEEN || board[pos.y - i][pos.x + i].getType() == BISHOP))
		{
			return true;
		}
		if (board[pos.y - i][pos.x + i].getColor() == enemy)
		{
			break;
		}
		i++;
	}
	i = 1;
	while (pos.x - i < 8 && pos.x - i >= 0 && pos.y - i < 8 && pos.y - i >= 0
		&& color != board[pos.y - i][pos.x - i].getColor())
	{
		if ((board[pos.y - i][pos.x - i].getType() == QUEEN || board[pos.y - i][pos.x - i].getType() == BISHOP))
		{
			return true;
		}
		if (board[pos.y - i][pos.x - i].getColor() == enemy)
		{
			break;
		}
		i++;
	}
	i = 1;
	while (pos.x - i < 8 && pos.x - i >= 0 && pos.y + i < 8 && pos.y + i >= 0
		&& color != board[pos.y + i][pos.x - i].getColor())
	{
		if ((board[pos.y + i][pos.x - i].getType() == QUEEN || board[pos.y + i][pos.x - i].getType() == BISHOP))
		{
			return true;
		}
		if (board[pos.y + i][pos.x - i].getColor() == enemy)
		{
			break;
		}
		i++;
	}
	i = 1;
	while (pos.x + i < 8 && pos.x + i >= 0
		&& color != board[pos.y][pos.x + i].getColor())
	{
		if ((board[pos.y][pos.x + i].getType() == QUEEN || board[pos.y][pos.x + i].getType() == ROOK))
		{
			return true;
		}
		if (board[pos.y][pos.x + i].getColor() == enemy)
		{
			break;
		}
		i++;
	}
	i = 1;
	while (pos.y + i < 8 && pos.y + i >= 0
		&& color != board[pos.y + i][pos.x].getColor())
	{
		if ((board[pos.y + i][pos.x].getType() == QUEEN || board[pos.y + i][pos.x].getType() == ROOK))
		{
			return true;
		}
		if (board[pos.y + i][pos.x].getColor() == enemy)
		{
			break;
		}
		i++;
	}
	i = 1;
	while (pos.x - i < 8 && pos.x - i >= 0
		&& color != board[pos.y][pos.x - i].getColor())
	{
		if ((board[pos.y][pos.x - i].getType() == QUEEN || board[pos.y][pos.x - i].getType() == ROOK))
		{
			return true;
		}
		if (board[pos.y][pos.x - i].getColor() == enemy)
		{
			break;
		}
		i++;
	}
	i = 1;
	while (pos.y - i < 8 && pos.y - i >= 0
		&& color != board[pos.y - i][pos.x].getColor())
	{
		if ((board[pos.y - i][pos.x].getType() == QUEEN || board[pos.y - i][pos.x].getType() == ROOK))
		{
			return true;
		}
		if (board[pos.y - i][pos.x].getColor() == enemy)
		{
			break;
		}
		i++;
	}
	if (board[pos.y + (1 * side)][pos.x + 1].getColor() == enemy && board[pos.y + (1 * side)][pos.x + 1].getType() == PAWN)
	{
		return true;
	}
	if (board[pos.y + (1 * side)][pos.x - 1].getColor() == enemy && board[pos.y + (1 * side)][pos.x - 1].getType() == PAWN)
	{
		return true;
	}
	return false;
}

//intent:get all the valid position
//pre:none
//post:a vector full of valid positions
std::vector<Position> Chess::getValidPos(Chess gBoard[8][8])
{
	std::vector<Position> validPos;
	validPos.clear();
	Position targetPos;
	int side, pawnOrigin;
	Color enemy;
	Chess board[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j] = gBoard[i][j];
		}
	}
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
		nineSquare(board, validPos);
		break;

	case QUEEN: //if is queen
		strightLine(board, validPos);
		crossLine(board, validPos);
		break;

	case BISHOP: //if is bishop

		crossLine(board, validPos);
		break;

	case KNIGHT: //if is knight
		lLine(board, validPos);
		break;

	case ROOK:

		strightLine(board, validPos);
		break;

	case PAWN: //if is pawn

		//the first step, can move one or two
		if (pos.y == pawnOrigin)
		{

			//can move one or two step, check if can move
			for (int i = 1; i <= 2; i++)
			{
				targetPos = Position(pos.x, (pos.y + (i * side)));

				//if there is nothing in the space
				if (board[pos.y + (i * side)][pos.x].getColor() == NONE)
				{
					validPos.push_back(targetPos);
				}
				else //if there is something in the space
				{
					break;
				}
			}
		}

		//not the first step, can move one step
		if (pos.y + 1 < 8 && pos.y - 1 >= 0)
		{
			targetPos = Position(pos.x, (pos.y + (1 * side)));

			//if there is nothing in the sapce
			if (board[pos.y + (1 * side)][pos.x].getColor() == NONE)
			{
				validPos.push_back(targetPos);
			}

			//if there is something can eat
			if (pos.x + 1 < 8 && board[pos.y + (1 * side)][pos.x + 1].getColor() == enemy)
			{
				targetPos = Position(pos.x + 1, pos.y + (1 * side));
				validPos.push_back(targetPos);
			}

			//if there is something can eat
			if (pos.x - 1 >= 0 && board[pos.y + (1 * side)][pos.x - 1].getColor() == enemy)
			{
				targetPos = Position(pos.x - 1, pos.y + (1 * side));
				validPos.push_back(targetPos);
			}
		}
		break;
	}
	return validPos;
}

void Chess::strightLine(Chess board[8][8], std::vector<Position>& validPos)
{
	Color enemy;
	Position kingPos;
	if (color == BLACK)
	{
		enemy = WHITE;
		kingPos = blackKing;
	}
	else if (color == WHITE) //if the color is white
	{
		enemy = BLACK;
		kingPos = whiteKing;
	}
	int i = 1;
	while (pos.x + i < 8 && pos.x + i >= 0
		&& color != board[pos.y][pos.x + i].getColor())
	{
		Position targetPos = Position(pos.x + i, pos.y);
		Chess tmp;
		tmp.setSpace(board[pos.y][pos.x + i]);
		board[pos.y][pos.x + i].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();
		if (!checkCheck(color, kingPos, board))
		{
			validPos.push_back(targetPos);
		}
		board[pos.y][pos.x].setSpace(board[pos.y][pos.x + i]);
		board[pos.y][pos.x + i].setSpace(tmp);
		i++;
		if (board[pos.y][pos.x + i].getColor() == enemy)
		{
			break;
		}
	}
	i = 1;
	while (pos.y + i < 8 && pos.y + i >= 0
		&& color != board[pos.y + i][pos.x].getColor())
	{
		Position targetPos = Position(pos.x, pos.y + i);
		Chess tmp;
		tmp.setSpace(board[pos.y + i][pos.x]);
		board[pos.y + i][pos.x].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();
		if (!checkCheck(color, kingPos, board))
		{
			validPos.push_back(targetPos);
		}
		board[pos.y][pos.x].setSpace(board[pos.y + i][pos.x]);
		board[pos.y + i][pos.x].setSpace(tmp);
		i++;
		if (board[pos.y + i][pos.x].getColor() == enemy)
		{
			break;
		}
	}
	i = 1;
	while (pos.x - i < 8 && pos.x - i >= 0
		&& color != board[pos.y][pos.x - i].getColor())
	{
		Position targetPos = Position(pos.x - i, pos.y);
		Chess tmp;
		tmp.setSpace(board[pos.y][pos.x - i]);
		board[pos.y][pos.x - i].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();
		if (!checkCheck(color, kingPos, board))
		{
			validPos.push_back(targetPos);
		}
		board[pos.y][pos.x].setSpace(board[pos.y][pos.x - i]);
		board[pos.y][pos.x - i].setSpace(tmp);
		i++;
		if (board[pos.y][pos.x - i].getColor() == enemy)
		{
			break;
		}
	}
	i = 1;
	while (pos.y - i < 8 && pos.y - i >= 0
		&& color != board[pos.y - i][pos.x].getColor())
	{
		Position targetPos = Position(pos.x, pos.y - i);
		Chess tmp;
		tmp.setSpace(board[pos.y - i][pos.x]);
		board[pos.y - i][pos.x].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();
		if (!checkCheck(color, kingPos, board))
		{
			validPos.push_back(targetPos);
		}
		board[pos.y][pos.x].setSpace(board[pos.y - i][pos.x]);
		board[pos.y - i][pos.x].setSpace(tmp);
		i++;
		if (board[pos.y - i][pos.x].getColor() == enemy)
		{
			break;
		}
	}
}

void Chess::crossLine(Chess board[8][8], std::vector<Position>& validPos)
{
	Color enemy;
	Position kingPos;
	if (color == BLACK)
	{
		enemy = WHITE;
		kingPos = blackKing;
	}
	else if (color == WHITE) //if the color is white
	{
		enemy = BLACK;
		kingPos = whiteKing;
	}
	int i = 1;
	while (pos.x + i < 8 && pos.x + i >= 0 && pos.y + i < 8 && pos.y + i >= 0
		&& color != board[pos.y + i][pos.x + i].getColor())
	{
		Position targetPos = Position(pos.x + i, pos.y + i);
		Chess tmp;
		tmp.setSpace(board[pos.y + i][pos.x + i]);
		board[pos.y + i][pos.x + i].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();
		if (!checkCheck(color, kingPos, board))
		{
			validPos.push_back(targetPos);
		}
		board[pos.y][pos.x].setSpace(board[pos.y + i][pos.x + i]);
		board[pos.y + i][pos.x + i].setSpace(tmp);
		i++;
		if (board[pos.y + i][pos.x + i].getColor() == enemy)
		{
			break;
		}
	}
	i = 1;
	while (pos.x + i < 8 && pos.x + i >= 0 && pos.y - i < 8 && pos.y - i >= 0
		&& color != board[pos.y - i][pos.x + i].getColor())
	{
		Position targetPos = Position(pos.x + i, pos.y - i);
		Chess tmp;
		tmp.setSpace(board[pos.y - i][pos.x + i]);
		board[pos.y - i][pos.x + i].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();
		if (!checkCheck(color, kingPos, board))
		{
			validPos.push_back(targetPos);
		}
		board[pos.y][pos.x].setSpace(board[pos.y - i][pos.x + i]);
		board[pos.y - i][pos.x + i].setSpace(tmp);
		i++;
		if (board[pos.y - i][pos.x + i].getColor() == enemy)
		{
			break;
		}
	}
	i = 1;
	while (pos.x - i < 8 && pos.x - i >= 0 && pos.y - i < 8 && pos.y - i >= 0
		&& color != board[pos.y - i][pos.x - i].getColor())
	{
		Position targetPos = Position(pos.x - i, pos.y - i);
		Chess tmp;
		tmp.setSpace(board[pos.y - i][pos.x - i]);
		board[pos.y - i][pos.x - i].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();
		if (!checkCheck(color, kingPos, board))
		{
			validPos.push_back(targetPos);
		}
		board[pos.y][pos.x].setSpace(board[pos.y - i][pos.x - i]);
		board[pos.y - i][pos.x - i].setSpace(tmp);
		i++;
		if (board[pos.y - i][pos.x - i].getColor() == enemy)
		{
			break;
		}
	}
	i = 1;
	while (pos.x - i < 8 && pos.x - i >= 0 && pos.y + i < 8 && pos.y + i >= 0
		&& color != board[pos.y + i][pos.x - i].getColor())
	{
		Position targetPos = Position(pos.x - i, pos.y + i);
		Chess tmp;
		tmp.setSpace(board[pos.y + i][pos.x - i]);
		board[pos.y + i][pos.x - i].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();
		if (!checkCheck(color, kingPos, board))
		{
			validPos.push_back(targetPos);
		}
		board[pos.y][pos.x].setSpace(board[pos.y + i][pos.x - i]);
		board[pos.y + i][pos.x - i].setSpace(tmp);
		i++;
		if (board[pos.y + i][pos.x - i].getColor() == enemy)
		{
			break;
		}
	}
}

void Chess::nineSquare(Chess board[8][8], std::vector<Position>& validPos)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (pos.x + i < 8 && pos.x + i >= 0 && pos.y + j < 8 && pos.y + j >= 0
				&& color != board[pos.y + j][pos.x + i].getColor() && !checkCheck(color, Position(pos.x + i, pos.y + j),board))
			{
				Position targetPos = Position(pos.x + i, pos.y + j);
				validPos.push_back(targetPos);
			}
		}
	}
}

void Chess::lLine(Chess board[8][8], std::vector<Position>& validPos)
{
	Position kingPos;
	if (color == BLACK)
	{
		kingPos = blackKing;
	}
	else if (color == WHITE) //if the color is white
	{
		kingPos = whiteKing;
	}
	int turn[8][2] = { {2,-1},{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2} };
	for (int i = 0; i < 8; i++)
	{
		if (pos.x + turn[i][0] < 8 && pos.x + turn[i][0] >= 0 && pos.y + turn[i][1] < 8 && pos.y + turn[i][1] >= 0
			&& color != board[pos.y + turn[i][1]][pos.x + turn[i][0]].getColor())
		{
			Position targetPos = Position(pos.x + turn[i][0], pos.y + turn[i][1]);
			Chess tmp;
			tmp.setSpace(board[pos.y + turn[i][1]][pos.x + turn[i][0]]);
			board[pos.y + turn[i][1]][pos.x + turn[i][0]].setSpace(board[pos.y][pos.x]);
			board[pos.y][pos.x].setEmpty();
			if (!checkCheck(color, kingPos, board))
			{
				validPos.push_back(targetPos);
			}
			board[pos.y][pos.x].setSpace(board[pos.y + turn[i][1]][pos.x + turn[i][0]]);
			board[pos.y + turn[i][1]][pos.x + turn[i][0]].setSpace(tmp);
		}
	}
}
