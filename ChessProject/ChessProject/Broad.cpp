/*****************************************************************
* File: Broad.cpp
 * Author: ­ð¤_µx¡B§d²»ºö
 * Create Date: 2023/05/06
 * Editor: ­ð¤_µx¡B§d²»ºö
 * Update Date: 2023/05/06
 * Description: set and update broad
 *********************************************************************/

#include "Broad.h"
#include <map>

/*std::map <char, std::string> charToLabel =
{
	'k' = "KING",

}*/

//intent:constructor and initialize the board
//pre:none
//post:none
Board::Board() 
{
	//set the empty space
	for (int i = 2; i < 6; i++) 
	{
		for (int j = 0; j < 8; j++) 
		{
			board[i][j].setEmpty();
		}
	}

	//set the black chess one by one
	board[0][0].setChess(ROOK, BLACK);
	board[0][1].setChess(KNIGHT, BLACK);
	board[0][2].setChess(BISHOP, BLACK);
	board[0][3].setChess(QUEEN, BLACK);
	board[0][4].setChess(KING, BLACK);
	board[0][5].setChess(BISHOP, BLACK);
	board[0][6].setChess(KNIGHT, BLACK);
	board[0][7].setChess(ROOK, BLACK);

	//set the black pawn
	for (int i = 0; i < 8; i++) 
	{
		board[1][i].setChess(PAWN, BLACK);
	}

	//set the white chess one by one
	board[7][0].setChess(ROOK, WHITE);
	board[7][1].setChess(KNIGHT, WHITE);
	board[7][2].setChess(BISHOP, WHITE);
	board[7][3].setChess(QUEEN, WHITE);
	board[7][4].setChess(KING, WHITE);
	board[7][5].setChess(BISHOP, WHITE);
	board[7][6].setChess(KNIGHT, WHITE);
	board[7][7].setChess(ROOK, WHITE);

	//set the white pawn
	for (int i = 0; i < 8; i++) 
	{
		board[6][i].setChess(PAWN, WHITE);
	}

	//set the position
	for (int i = 0; i < 8; i++) 
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j].setPos(Position(j, i));
		}
	}
}

//intent: return the chess board to display it
//pre:none
//post:the address of the board
Chess* Board::getBoard() 
{
	return &board[0][0];
}

//intent:get all the valid position
//pre:none
//post:a vector full of valid positions
std::vector<Position> Board::getValidPos(Chess chess) 
{
	Color color = chess.getColor(), enemy;
	Label label = chess.getLabel();
	Position pos = chess.getPos();
	std::vector<Position> validPos;
	validPos.clear();
	Position targetPos;
	int side, pawnOrigin;

	//if the color is black
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
	switch (label) 
	{
	case KING: //if is king

		//if can move
		if (pos.x + 1 < 8 && pos.y + 1 < 8 && board[pos.y + 1][pos.x + 1].getColor() != color) 
		{
			targetPos = Position(pos.x + 1, pos.y + 1);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.y + 1 < 8 && board[pos.y + 1][pos.x].getColor() != color)
		{

			targetPos = Position(pos.x, pos.y + 1);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.x - 1 >= 0 && pos.y + 1 < 8 && board[pos.y + 1][pos.x - 1].getColor() != color) 
		{

			targetPos = Position(pos.x - 1, pos.y + 1);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.x + 1 < 8 && board[pos.y][pos.x + 1].getColor() != color) 
		{

			targetPos = Position(pos.x + 1, pos.y);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.x - 1 >= 0 && board[pos.y][pos.x - 1].getColor() != color) 
		{

			targetPos = Position(pos.x - 1, pos.y);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.x + 1 < 8 && pos.y - 1 >= 0 && board[pos.y - 1][pos.x + 1].getColor() != color) 
		{

			targetPos = Position(pos.x + 1, pos.y - 1);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.y - 1 >= 0 && board[pos.y - 1][pos.x].getColor() != color) 
		{

			targetPos = Position(pos.x, pos.y - 1);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.x - 1 >= 0 && pos.y - 1 >= 0 && board[pos.y - 1][pos.x - 1].getColor() != color) 
		{

			targetPos = Position(pos.x - 1, pos.y - 1);
			validPos.push_back(targetPos);
		}
		break;
	case QUEEN: //if is queen

		//check the rigth side
		for (int i = pos.x + 1; i < 8; i++) 
		{
			targetPos = Position(i, pos.y);

			//if there is nothing in the space
			if (board[pos.y][i].getColor() == NONE) 
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y][i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}

		//check the left side
		for (int i = pos.x - 1; i >= 0; i--) 
		{
			targetPos = Position(i, pos.y);

			//if there is nothing in the space
			if (board[pos.y][i].getColor() == NONE) 
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y][i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}

		//check from below
		for (int i = pos.y + 1; i < 8; i++) 
		{
			targetPos = Position(pos.x, i);

			//if there is nothing in the space
			if (board[i][pos.x].getColor() == NONE) 
			{
				validPos.push_back(targetPos);
			}
			else if (board[i][pos.x].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}

		//check above
		for (int i = pos.y - 1; i >= 0; i--) 
		{
			targetPos = Position(pos.x, i);

			//if there is nothing in the space
			if (board[i][pos.x].getColor() == NONE) 
			{
				validPos.push_back(targetPos);
			}
			else if (board[i][pos.x].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}
	case BISHOP: //if is bishop

		//run the most turn
		for (int i = 1; i < 8 && pos.x + i < 8 && pos.y + i < 8; i++) 
		{
			targetPos = Position(pos.x + i, pos.y + i);

			//if there is nothing in the sapce
			if (board[pos.y + i][pos.x + i].getColor() == NONE) 
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y + i][pos.x + i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //can not move
			{
				break;
			}
		}

		//run the most turn
		for (int i = 1; i < 8 && pos.x + i < 8 && pos.y - i >= 0; i++)
		{
			targetPos = Position(pos.x + i, pos.y - i);

			//if there is nothing in the sapce
			if (board[pos.y - i][pos.x + i].getColor() == NONE) 
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y - i][pos.x + i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //can not move
			{
				break;
			}
		}

		//run the most turn
		for (int i = 1; i < 8 && pos.x - i >= 0 && pos.y + i < 8; i++) 
		{
			targetPos = Position(pos.x - i, pos.y + i);

			//if there is nothing in the sapce
			if (board[pos.y + i][pos.x - i].getColor() == NONE) 
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y + i][pos.x - i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //can not move
			{
				break;
			}
		}

		//run the most turn
		for (int i = 1; i < 8 && pos.x - i >= 0 && pos.y - i >= 0; i++) 
		{
			targetPos = Position(pos.x - i, pos.y - i);

			//if there is nothing in the sapce
			if (board[pos.y - i][pos.x - i].getColor() == NONE) 
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y - i][pos.x - i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //can not move
			{
				break;
			}
		}
		break;
	case KNIGHT: //if is knight

		//if can move
		if (pos.x + 1 < 8 && pos.y + 2 < 8 && board[pos.y + 2][pos.x + 1].getColor() != color) 
		{
			targetPos = Position(pos.x + 1, pos.y + 2);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.x + 1 < 8 && pos.y - 2 >= 0 && board[pos.y - 2][pos.x + 1].getColor() != color) 
		{
			targetPos = Position(pos.x + 1, pos.y - 2);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.x + 2 < 8 && pos.y + 1 < 8 && board[pos.y + 1][pos.x + 2].getColor() != color) 
		{
			targetPos = Position(pos.x + 2, pos.y + 1);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.x + 2 < 8 && pos.y - 1 >= 0 && board[pos.y - 1][pos.x + 2].getColor() != color) 
		{
			targetPos = Position(pos.x + 2, pos.y - 1);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.x - 1 < 8 && pos.y + 2 < 8 && board[pos.y + 2][pos.x - 1].getColor() != color) 
		{
			targetPos = Position(pos.x - 1, pos.y + 2);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.x - 1 < 8 && pos.y - 2 >= 0 && board[pos.y - 2][pos.x - 1].getColor() != color) 
		{
			targetPos = Position(pos.x - 1, pos.y - 2);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.x - 2 < 8 && pos.y + 1 < 8 && board[pos.y + 1][pos.x - 2].getColor() != color) 
		{
			targetPos = Position(pos.x - 2, pos.y + 1);
			validPos.push_back(targetPos);
		}

		//if can move
		if (pos.x - 2 < 8 && pos.y - 1 >= 0 && board[pos.y - 1][pos.x - 2].getColor() != color) 
		{
			targetPos = Position(pos.x - 2, pos.y - 1);
			validPos.push_back(targetPos);
		}
		break;
	case ROOK: //if is rook

		//check go right
		for (int i = pos.x + 1; i < 8; i++) 
		{
			targetPos = Position(i, pos.y);

			//if there is nothing in the space
			if (board[pos.y][i].getColor() == NONE)
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y][i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}

		//check go left
		for (int i = pos.x - 1; i >= 0; i--) 
		{
			targetPos = Position(i, pos.y);

			//if there is nothing in the space
			if (board[pos.y][i].getColor() == NONE) 
			{
				validPos.push_back(targetPos);
			}
			else if (board[pos.y][i].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}

		//check go down
		for (int i = pos.y + 1; i < 8; i++) 
		{
			targetPos = Position(pos.x, i);

			//if there is nothing in the space
			if (board[i][pos.x].getColor() == NONE) 
			{
				validPos.push_back(targetPos);
			}
			else if (board[i][pos.x].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
			}
		}

		//check go up
		for (int i = pos.y - 1; i >= 0; i--) 
		{
			targetPos = Position(pos.x, i);

			//if there is nothing in the space
			if (board[i][pos.x].getColor() == NONE) 
			{
				validPos.push_back(targetPos);
			}
			else if (board[i][pos.x].getColor() == enemy) //if there is something can eat
			{
				validPos.push_back(targetPos);
				break;
			}
			else //if there is something in the space
			{
				break;
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

//intent: move the chess
//pre: whose turn
//post: none
void Board::move(Player& player) 
{
	Position source, target;
	std::string a, b;

	//loop until it move successfully
	while (true) 
	{
		std::cin >> a >> b;
		source.x = a[0] - 'a';
		source.y = 8 - (a[1] - '0');
		target.x = b[0] - 'a';
		target.y = 8 - (b[1] - '0');

		//if it is not this player's turn
		if (player.getColor() != board[source.y][source.x].getColor()) 
		{
			std::cout << "Fail" << std::endl;
			continue;
		}

		//if the target position is valid
		if (moveAvalible(board[source.y][source.x], target)) 
		{
			board[target.y][target.x].setSpace(board[source.y][source.x]);
			board[source.y][source.x].setEmpty();
			std::cout << "Success" << std::endl;

			//if is pawn check if it can promotion
			if (board[target.y][target.x].getLabel() == PAWN) 
			{
				board[target.y][target.x].checkPawn();
			}
			break;
		}
		else 
		{
			std::cout << "Fail" << std::endl;
			continue;
		}
	}
}

//intent: if this position is valid
//pre: which chess and the target position
//post:bool
bool Board::moveAvalible(Chess chess, Position target)
{
	std::vector<Position> validPos;
	validPos = getValidPos(chess);

	//run int the vector saved valid positions
	for (auto& i : validPos)
	{
		std::cout << i.x << i.y << std::endl;

		//if the position if found
		if (i == target)
		{
			return true;
		}
	}
	return false;
}



/*Board::Board(std::string fen)
{
	int i, j = 0;
	for (int i = 0; i < 8; i++)
	{
		for (; j < fen.length(); j++)
		{
			if (fen[j] == '/')
			{
				break;
			}
			else if (fen[j] >= 'a' && fen[j] <= 'z')
			{
				board[j/8][j%8]=
			}
		}
	}

	for (i = 0; i < fen.length(); i++)
	{
		if (fen[i] != '/')
		{
			if (fen[i] >= 'a' && fen[i] <= 'a') //black chess
			{

			}
		}
	}
}*/