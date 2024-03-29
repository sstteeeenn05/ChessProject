/*****************************************************************
* File: Chess.cpp
 * Author: ��_�x�B�d����
 * Create Date: 2023/05/05
 * Editor: ��_�x�B�d����
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
	player = NONE;
	moved = false;
}

//intent:constructor and initialize the chess
//pre:a Position a Label and a Color
//post:a object of Chess
Chess::Chess(Position pos, Type label, Player color)
{
	this->pos = pos;
	this->type = label;
	this->player = color;
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
void Chess::setChess(Type label, Player color) 
{
	this->type = label;
	this->player = color;
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
	this->player = chess.player;
	this->type = chess.type;
}

//intent:set the space to empty
//pre:none
//post:none
void Chess::setEmpty() 
{
	this->player = NONE;
	this->type = EMPTY;
}

//intent:if the chess have moved set moved to true
//pre:none
//post:none
void Chess::setMoved()
{
	moved = true;
}

//intent:set the white king to the position
//pre:target position
//post:none
void Chess::setWhiteKingPos(Position pos)
{
	whiteKing = pos;
}

//intent:set the black king to the position
//pre:target position
//post:none
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
Player Chess::getPlayer() 
{
	return player;
}

//intent:check if this chess has moved before
//pre:none
//post:true if it has moved
bool Chess::getMoved()
{
	return moved;
}

//intent:get the position of the white king
//pre:none
//post:position
Position Chess::getWhiteKingPos()
{
	return whiteKing;
}

//intent:get the position of the black king
//pre:none
//post:position
Position Chess::getBlackKingPos()
{
	return blackKing;
}

//intent:check and change if the pawn is at the end
//pre: none
//post:none
bool Chess::checkPromotion()
{
	return type == PAWN && (player == WHITE && pos.y == 0 || player == BLACK && pos.y == 7);
}

//intent:check and change if the pawn is at the end
//pre: none
//post:none
void Chess::doPromotion() 
{
	int choose = 0;
	do {
		std::cout << "playing;" << (player == WHITE ? "white" : "black") << ";;;promotion";
		std::cin >> choose;

		//decide what type of chess that the pawn is going to be
		if (choose == 1) setType(QUEEN);
		else if (choose == 2) setType(BISHOP);
		else if (choose == 3) setType(KNIGHT);
		else if (choose == 4) setType(ROOK);
		else std::cout << "playing;" << (player == WHITE ? "white" : "black") << ";;;failed";

	} while (choose < 1 && choose > 4);
}

//intent:check if the king is being check from the king's perspective
//pre:the player right now post and the board
//post:if can check return true
bool Chess::checkCheck(Player player, Position pos, Chess board[8][8])
{
	Player enemy;
	int side;

	//if the color is black
	if (player == BLACK)
	{
		enemy = WHITE;
		side = 1;
	}
	else if (player == WHITE) //if the color is white
	{
		enemy = BLACK;
		side = -1;
	}

	//loop the nine-square
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			//if the target pos is valid and enermy king is at the target pos 
			if (pos.x + i < 8 && pos.x + i >= 0 && pos.y + j < 8 && pos.y + j >= 0
				&& player != board[pos.y + j][pos.x + i].getPlayer() && board[pos.y + j][pos.x + i].getType() == KING)
			{
				return true;
			}
		}
	}

	int turn[8][2] = { {2,-1},{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2} };

	//loop the L-line
	for (int i = 0; i < 8; i++)
	{
		//if the target pos is valid and enermy knight is at the target pos 
		if (pos.x + turn[i][0] < 8 && pos.x + turn[i][0] >= 0 && pos.y + turn[i][1] < 8 && pos.y + turn[i][1] >= 0
			&& player != board[pos.y + turn[i][1]][pos.x + turn[i][0]].getPlayer() && board[pos.y + turn[i][1]][pos.x + turn[i][0]].getType() == KNIGHT)
		{
			return true;
		}
	}

	int i = 1;

	//loop the cross line of right and down and target pos is blank or enermy
	while (pos.x + i < 8 && pos.x + i >= 0 && pos.y + i < 8 && pos.y + i >= 0
		&& player != board[pos.y + i][pos.x + i].getPlayer())
	{
		//if the target pos is queen or bishop
		if ((board[pos.y + i][pos.x + i].getType() == QUEEN || board[pos.y + i][pos.x + i].getType() == BISHOP))
		{
			return true;
		}

		//if the target pos is enermy but not queen or bishop
		if (board[pos.y + i][pos.x + i].getPlayer() == enemy)
		{
			break;
		}
		i++;
	}

	i = 1;

	//loop the cross line of right and up and target pos is blank or enermy
	while (pos.x + i < 8 && pos.x + i >= 0 && pos.y - i < 8 && pos.y - i >= 0
		&& player != board[pos.y - i][pos.x + i].getPlayer())
	{
		//if the target pos is queen or bishop
		if ((board[pos.y - i][pos.x + i].getType() == QUEEN || board[pos.y - i][pos.x + i].getType() == BISHOP))
		{
			return true;
		}

		//if the target pos is enermy but not queen or bishop
		if (board[pos.y - i][pos.x + i].getPlayer() == enemy)
		{
			break;
		}

		i++;
	}

	i = 1;

	//loop the cross line of left and up and target pos is blank or enermy
	while (pos.x - i < 8 && pos.x - i >= 0 && pos.y - i < 8 && pos.y - i >= 0
		&& player != board[pos.y - i][pos.x - i].getPlayer())
	{
		//if the target pos is queen or bishop
		if ((board[pos.y - i][pos.x - i].getType() == QUEEN || board[pos.y - i][pos.x - i].getType() == BISHOP))
		{
			return true;
		}

		//if the target pos is enermy but not queen or bishop
		if (board[pos.y - i][pos.x - i].getPlayer() == enemy)
		{
			break;
		}

		i++;
	}

	i = 1;

	//loop the cross line of left and down and target pos is blank or enermy
	while (pos.x - i < 8 && pos.x - i >= 0 && pos.y + i < 8 && pos.y + i >= 0
		&& player != board[pos.y + i][pos.x - i].getPlayer())
	{
		//if the target pos is queen or bishop
		if ((board[pos.y + i][pos.x - i].getType() == QUEEN || board[pos.y + i][pos.x - i].getType() == BISHOP))
		{
			return true;
		}

		//if the target pos is enermy but not queen or bishop
		if (board[pos.y + i][pos.x - i].getPlayer() == enemy)
		{
			break;
		}

		i++;
	}

	i = 1;

	//loop the straigth line of right and target pos is blank or enermy
	while (pos.x + i < 8 && pos.x + i >= 0
		&& player != board[pos.y][pos.x + i].getPlayer())
	{
		//if the target pos is queen or bishop
		if ((board[pos.y][pos.x + i].getType() == QUEEN || board[pos.y][pos.x + i].getType() == ROOK))
		{
			return true;
		}

		//if the target pos is enermy but not queen or rook
		if (board[pos.y][pos.x + i].getPlayer() == enemy)
		{
			break;
		}

		i++;
	}

	i = 1;

	//loop the straigth line of down and target pos is blank or enermy
	while (pos.y + i < 8 && pos.y + i >= 0
		&& player != board[pos.y + i][pos.x].getPlayer())
	{
		//if the target pos is queen or bishop
		if ((board[pos.y + i][pos.x].getType() == QUEEN || board[pos.y + i][pos.x].getType() == ROOK))
		{
			return true;
		}

		//if the target pos is enermy but not queen or rook
		if (board[pos.y + i][pos.x].getPlayer() == enemy)
		{
			break;
		}

		i++;
	}

	i = 1;

	//loop the straigth line of left and target pos is blank or enermy
	while (pos.x - i < 8 && pos.x - i >= 0
		&& player != board[pos.y][pos.x - i].getPlayer())
	{
		//if the target pos is queen or bishop
		if ((board[pos.y][pos.x - i].getType() == QUEEN || board[pos.y][pos.x - i].getType() == ROOK))
		{
			return true;
		}

		//if the target pos is enermy but not queen or rook
		if (board[pos.y][pos.x - i].getPlayer() == enemy)
		{
			break;
		}

		i++;
	}

	i = 1;

	//loop the straigth line of up 
	while (pos.y - i < 8 && pos.y - i >= 0
		&& player != board[pos.y - i][pos.x].getPlayer())
	{
		//if the target pos is queen or bishop
		if ((board[pos.y - i][pos.x].getType() == QUEEN || board[pos.y - i][pos.x].getType() == ROOK))
		{
			return true;
		}

		//if the target pos is enermy but not queen or rook
		if (board[pos.y - i][pos.x].getPlayer() == enemy)
		{
			break;
		}

		i++;
	}

	//if there is an enermy rook on right side
	if (board[pos.y + (1 * side)][pos.x + 1].getPlayer() == enemy && board[pos.y + (1 * side)][pos.x + 1].getType() == PAWN)
	{
		return true;
	}

	//if there is an enermy rook on left side
	if (board[pos.y + (1 * side)][pos.x - 1].getPlayer() == enemy && board[pos.y + (1 * side)][pos.x - 1].getType() == PAWN)
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
	Position targetPos, kingPos;
	int side, pawnOrigin, pawnEnd;
	Player enemy;
	Chess board[8][8];

	//copy the board one by one
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j] = gBoard[i][j];
		}
	}

	//if the color is black
	if (player == BLACK)
	{
		enemy = WHITE;
		side = 1;
		pawnOrigin = 1;
		kingPos = blackKing;
	}
	else if (player == WHITE) //if the color is white
	{
		enemy = BLACK;
		side = -1;
		pawnOrigin = 6;
		kingPos = whiteKing;
	}

	//judge the label
	switch (this->type)
	{
	case KING: //if is king
		nineSquare(board, validPos); //call the nineSquare function to set the valid pos
		break;

	case QUEEN: //if is queen
		strightLine(board, validPos); //call the strightLine function to set the valid pos
		crossLine(board, validPos); //call the crossLine function to set the valid pos
		break;

	case BISHOP: //if is bishop

		crossLine(board, validPos); //call the crossLine function to set the valid pos
		break;

	case KNIGHT: //if is knight
		lLine(board, validPos); //call the lLine function to set the valid pos
		break;

	case ROOK: //if is rook
		strightLine(board, validPos); //call the strightLine function to set the valid pos
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
				if (board[pos.y + (i * side)][pos.x].getPlayer() == NONE)
				{
					Chess tmp;
					tmp.setSpace(board[pos.y + (i * side)][pos.x]);
					board[pos.y + (i * side)][pos.x].setSpace(board[pos.y][pos.x]);
					board[pos.y][pos.x].setEmpty();

					//if the move won't let the king to be check by any enermy chess
					if (!checkCheck(player, kingPos, board))
					{
						validPos.push_back(targetPos);
					}

					//return the board to the origin
					board[pos.y][pos.x].setSpace(board[pos.y + (i * side)][pos.x]);
					board[pos.y + (i * side)][pos.x].setSpace(tmp);
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

			//if there is nothing in the space
			if (board[pos.y + (1 * side)][pos.x].getPlayer() == NONE)
			{
				Chess tmp;
				tmp.setSpace(board[pos.y + (1 * side)][pos.x]);
				board[pos.y + (1 * side)][pos.x].setSpace(board[pos.y][pos.x]);
				board[pos.y][pos.x].setEmpty();

				//if the move won't let the king to be check by any enermy chess
				if (!checkCheck(player, kingPos, board))
				{
					validPos.push_back(targetPos);
				}

				//return the board to the origin
				board[pos.y][pos.x].setSpace(board[pos.y + (1 * side)][pos.x]);
				board[pos.y + (1 * side)][pos.x].setSpace(tmp);
			}

			//if there is something can eat
			if (pos.x + 1 < 8 && board[pos.y + (1 * side)][pos.x + 1].getPlayer() == enemy)
			{
				targetPos = Position(pos.x + 1, pos.y + (1 * side));
				Chess tmp;
				tmp.setSpace(board[pos.y + (1 * side)][pos.x + 1]);
				board[pos.y + (1 * side)][pos.x + 1].setSpace(board[pos.y][pos.x]);
				board[pos.y][pos.x].setEmpty();

				//if the move won't let the king to be check by any enermy chess
				if (!checkCheck(player, kingPos, board))
				{
					validPos.push_back(targetPos);
				}

				//return the board to the origin
				board[pos.y][pos.x].setSpace(board[pos.y + (1 * side)][pos.x + 1]);
				board[pos.y + (1 * side)][pos.x + 1].setSpace(tmp);
			}

			//if there is something can eat
			if (pos.x - 1 >= 0 && board[pos.y + (1 * side)][pos.x - 1].getPlayer() == enemy)
			{
				targetPos = Position(pos.x - 1, pos.y + (1 * side));
				Chess tmp;
				tmp.setSpace(board[pos.y + (1 * side)][pos.x - 1]);
				board[pos.y + (1 * side)][pos.x - 1].setSpace(board[pos.y][pos.x]);
				board[pos.y][pos.x].setEmpty();

				//if the move won't let the king to be check by any enermy chess
				if (!checkCheck(player, kingPos, board))
				{
					validPos.push_back(targetPos);
				}

				//return the board to the origin
				board[pos.y][pos.x].setSpace(board[pos.y + (1 * side)][pos.x - 1]);
				board[pos.y + (1 * side)][pos.x - 1].setSpace(tmp);
			}
		}
	}
	return validPos;
}

//intent:find all the valid places on the straight line
//pre:the board and the vector to save all the valid places
//post:none
void Chess::strightLine(Chess board[8][8], std::vector<Position>& validPos)
{
	Player enemy;
	Position kingPos;

	//if the color is black
	if (player == BLACK)
	{
		enemy = WHITE;
		kingPos = blackKing;
	}
	else if (player == WHITE) //if the color is white
	{
		enemy = BLACK;
		kingPos = whiteKing;
	}
	int i = 1;

	//if the target pos is valid and it is the enermy or blank on right side 
	while (pos.x + i < 8 && pos.x + i >= 0
		&& player != board[pos.y][pos.x + i].getPlayer())
	{
		Position targetPos = Position(pos.x + i, pos.y);
		Chess tmp;
		tmp.setSpace(board[pos.y][pos.x + i]);
		board[pos.y][pos.x + i].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();

		//if the move won't let the king to be check by any enermy chess
		if (!checkCheck(player, kingPos, board))
		{
			validPos.push_back(targetPos);
		}

		//return the board to the origin
		board[pos.y][pos.x].setSpace(board[pos.y][pos.x + i]);
		board[pos.y][pos.x + i].setSpace(tmp);
		i++;

		//stop if there is the enermy
		if (board[pos.y][pos.x + i].getPlayer() == enemy)
		{
			break;
		}
	}

	i = 1;

	//if the target pos is valid and it is the enermy or blank from down
	while (pos.y + i < 8 && pos.y + i >= 0
		&& player != board[pos.y + i][pos.x].getPlayer())
	{
		Position targetPos = Position(pos.x, pos.y + i);
		Chess tmp;
		tmp.setSpace(board[pos.y + i][pos.x]);
		board[pos.y + i][pos.x].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();

		//if the move won't let the king to be check by any enermy chess
		if (!checkCheck(player, kingPos, board))
		{
			validPos.push_back(targetPos);
		}

		//return the board to the origin
		board[pos.y][pos.x].setSpace(board[pos.y + i][pos.x]);
		board[pos.y + i][pos.x].setSpace(tmp);
		i++;

		//stop if there is the enermy
		if (board[pos.y + i][pos.x].getPlayer() == enemy)
		{
			break;
		}
	}

	i = 1;

	//if the target pos is valid and it is the enermy or blank from left side
	while (pos.x - i < 8 && pos.x - i >= 0
		&& player != board[pos.y][pos.x - i].getPlayer())
	{
		Position targetPos = Position(pos.x - i, pos.y);
		Chess tmp;
		tmp.setSpace(board[pos.y][pos.x - i]);
		board[pos.y][pos.x - i].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();

		//if the move won't let the king to be check by any enermy chess
		if (!checkCheck(player, kingPos, board))
		{
			validPos.push_back(targetPos);
		}

		//return the board to the origin
		board[pos.y][pos.x].setSpace(board[pos.y][pos.x - i]);
		board[pos.y][pos.x - i].setSpace(tmp);
		i++;

		//stop if there is the enermy
		if (board[pos.y][pos.x - i].getPlayer() == enemy)
		{
			break;
		}
	}
	i = 1;

	//if the target pos is valid and it is the enermy or blank from up
	while (pos.y - i < 8 && pos.y - i >= 0
		&& player != board[pos.y - i][pos.x].getPlayer())
	{
		Position targetPos = Position(pos.x, pos.y - i);
		Chess tmp;
		tmp.setSpace(board[pos.y - i][pos.x]);
		board[pos.y - i][pos.x].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();

		//if the move won't let the king to be check by any enermy chess
		if (!checkCheck(player, kingPos, board))
		{
			validPos.push_back(targetPos);
		}

		//return the board to the origin
		board[pos.y][pos.x].setSpace(board[pos.y - i][pos.x]);
		board[pos.y - i][pos.x].setSpace(tmp);
		i++;

		//stop if there is the enermy
		if (board[pos.y - i][pos.x].getPlayer() == enemy)
		{
			break;
		}
	}
}

//intent:find all the valid places on the cross line
//pre:the board and the vector to save all the valid places
//post:none
void Chess::crossLine(Chess board[8][8], std::vector<Position>& validPos)
{
	Player enemy;
	Position kingPos;

	//if the color is black
	if (player == BLACK)
	{
		enemy = WHITE;
		kingPos = blackKing;
	}
	else if (player == WHITE) //if the color is white
	{
		enemy = BLACK;
		kingPos = whiteKing;
	}

	int i = 1;

	//if the target is valid and the target is blank or is the enermy from right and down
	while (pos.x + i < 8 && pos.x + i >= 0 && pos.y + i < 8 && pos.y + i >= 0
		&& player != board[pos.y + i][pos.x + i].getPlayer())
	{
		Position targetPos = Position(pos.x + i, pos.y + i);
		Chess tmp;
		tmp.setSpace(board[pos.y + i][pos.x + i]);
		board[pos.y + i][pos.x + i].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();

		//if the move won't let the king to be check by any enermy chess
		if (!checkCheck(player, kingPos, board))
		{
			validPos.push_back(targetPos);
		}

		//return the board to the origin
		board[pos.y][pos.x].setSpace(board[pos.y + i][pos.x + i]);
		board[pos.y + i][pos.x + i].setSpace(tmp);
		i++;

		//stop if there is the enermy
		if (board[pos.y + i][pos.x + i].getPlayer() == enemy)
		{
			break;
		}
	}

	i = 1;

	//if the target is valid and the target is blank or is the enermy from right and up
	while (pos.x + i < 8 && pos.x + i >= 0 && pos.y - i < 8 && pos.y - i >= 0
		&& player != board[pos.y - i][pos.x + i].getPlayer())
	{
		Position targetPos = Position(pos.x + i, pos.y - i);
		Chess tmp;
		tmp.setSpace(board[pos.y - i][pos.x + i]);
		board[pos.y - i][pos.x + i].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();

		//if the move won't let the king to be check by any enermy chess
		if (!checkCheck(player, kingPos, board))
		{
			validPos.push_back(targetPos);
		}

		//return the board to the origin
		board[pos.y][pos.x].setSpace(board[pos.y - i][pos.x + i]);
		board[pos.y - i][pos.x + i].setSpace(tmp);
		i++;

		//stop if there is the enermy
		if (board[pos.y - i][pos.x + i].getPlayer() == enemy)
		{
			break;
		}
	}

	i = 1;

	//if the target is valid and the target is blank or is the enermy from left and up
	while (pos.x - i < 8 && pos.x - i >= 0 && pos.y - i < 8 && pos.y - i >= 0
		&& player != board[pos.y - i][pos.x - i].getPlayer())
	{
		Position targetPos = Position(pos.x - i, pos.y - i);
		Chess tmp;
		tmp.setSpace(board[pos.y - i][pos.x - i]);
		board[pos.y - i][pos.x - i].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();

		//if the move won't let the king to be check by any enermy chess
		if (!checkCheck(player, kingPos, board))
		{
			validPos.push_back(targetPos);
		}

		//return the board to the origin
		board[pos.y][pos.x].setSpace(board[pos.y - i][pos.x - i]);
		board[pos.y - i][pos.x - i].setSpace(tmp);
		i++;

		//stop if there is the enermy
		if (board[pos.y - i][pos.x - i].getPlayer() == enemy)
		{
			break;
		}
	}
	i = 1;

	//if the target is valid and the target is blank or is the enermy from left and down
	while (pos.x - i < 8 && pos.x - i >= 0 && pos.y + i < 8 && pos.y + i >= 0
		&& player != board[pos.y + i][pos.x - i].getPlayer())
	{
		Position targetPos = Position(pos.x - i, pos.y + i);
		Chess tmp;
		tmp.setSpace(board[pos.y + i][pos.x - i]);
		board[pos.y + i][pos.x - i].setSpace(board[pos.y][pos.x]);
		board[pos.y][pos.x].setEmpty();

		//if the move won't let the king to be check by any enermy chess
		if (!checkCheck(player, kingPos, board))
		{
			validPos.push_back(targetPos);
		}

		//return the board to the origin
		board[pos.y][pos.x].setSpace(board[pos.y + i][pos.x - i]);
		board[pos.y + i][pos.x - i].setSpace(tmp);
		i++;

		//stop if there is the enermy
		if (board[pos.y + i][pos.x - i].getPlayer() == enemy)
		{
			break;
		}
	}
}

//intent:find all the valid places in the nine square
//pre:the board and the vector to save all the valid places
//post:none
void Chess::nineSquare(Chess board[8][8], std::vector<Position>& validPos)
{
	//loop from left to right and up to down of nine-suare
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{

			//if the pos is valid and is blank or enermy and won't let the king itself to be check
			if (pos.x + i < 8 && pos.x + i >= 0 && pos.y + j < 8 && pos.y + j >= 0
				&& player != board[pos.y + j][pos.x + i].getPlayer() && !checkCheck(player, Position(pos.x + i, pos.y + j),board))
			{
				Position targetPos = Position(pos.x + i, pos.y + j);
				validPos.push_back(targetPos);
			}
		}
	}
}

//intent:find all the valid places on the L line
//pre:the board and the vector to save all the valid places
//post:none
void Chess::lLine(Chess board[8][8], std::vector<Position>& validPos)
{
	Position kingPos;

	//if the color is black
	if (player == BLACK)
	{
		kingPos = blackKing;
	}
	else if (player == WHITE) //if the color is white
	{
		kingPos = whiteKing;
	}

	int turn[8][2] = { {2,-1},{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2} };

	//loop the L-line
	for (int i = 0; i < 8; i++)
	{
		//if the target is valid and is enermy or blank
		if (pos.x + turn[i][0] < 8 && pos.x + turn[i][0] >= 0 && pos.y + turn[i][1] < 8 && pos.y + turn[i][1] >= 0
			&& player != board[pos.y + turn[i][1]][pos.x + turn[i][0]].getPlayer())
		{
			Position targetPos = Position(pos.x + turn[i][0], pos.y + turn[i][1]);
			Chess tmp;
			tmp.setSpace(board[pos.y + turn[i][1]][pos.x + turn[i][0]]);
			board[pos.y + turn[i][1]][pos.x + turn[i][0]].setSpace(board[pos.y][pos.x]);
			board[pos.y][pos.x].setEmpty();

			//if the move won't let the king to be check by any enermy chess
			if (!checkCheck(player, kingPos, board))
			{
				validPos.push_back(targetPos);
			}

			//return the board to the origin
			board[pos.y][pos.x].setSpace(board[pos.y + turn[i][1]][pos.x + turn[i][0]]);
			board[pos.y + turn[i][1]][pos.x + turn[i][0]].setSpace(tmp);
		}
	}
}