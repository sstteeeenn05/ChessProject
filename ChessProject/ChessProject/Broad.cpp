#include "Broad.h"
#include <map>

/*std::map <char, std::string> charToLabel =
{
	'k' = "king",

}*/

Board::Board()
{
	for (int i = 2; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j].setChess(EMPTY, NONE);
		}
	}
	//X ¤W­±¬O0
	board[0][0].setChess(ROOK, BLACK);
	board[0][1].setChess(KNIGHT, BLACK);
	board[0][2].setChess(BISHOP, BLACK);
	board[0][3].setChess(QUEEN, BLACK);
	board[0][4].setChess(KING, BLACK);
	board[0][5].setChess(BISHOP, BLACK);
	board[0][6].setChess(KNIGHT, BLACK);
	board[0][7].setChess(ROOK, BLACK);

	for (int i = 0; i < 8; i++)
	{
		board[1][i].setChess(PAWN, BLACK);
	}

	board[7][0].setChess(ROOK, WHITE);
	board[7][1].setChess(KNIGHT, WHITE);
	board[7][2].setChess(BISHOP, WHITE);
	board[7][3].setChess(QUEEN, WHITE);
	board[7][4].setChess(KING, WHITE);
	board[7][5].setChess(BISHOP, WHITE);
	board[7][6].setChess(KNIGHT, WHITE);
	board[7][7].setChess(ROOK, WHITE);

	for (int i = 0; i < 8; i++)
	{
		board[6][i].setChess(PAWN, WHITE);
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j].setPos(Position(i, j));
		}
	}
}

Chess* Board::drawBoard()
{
	return &board[0][0];
}

std::vector<Position> Board::getValidPos(Chess chess)
{
	Color color = chess.getColor();
	Label label = chess.getLabel();
	Position pos = chess.getPos();
	std::vector<Position> validPos;
	Position targetPos;
	if (color == BLACK)
	{
		switch (label)
		{
		case KING:
			break;
		case QUEEN:
			break;
		case BISHOP:
			break;
		case KNIGHT:
			break;
		case ROOK:
			for (int i = pos.x; i < 8; i++)
			{
				targetPos = Position(i, pos.y);
				if (board[i][pos.y].getColor() == NONE) //if there is nothing in the space
				{
					validPos.push_back(targetPos);
				}
				if (board[i][pos.y].getColor() == WHITE) //if there is something can eat
				{
					validPos.push_back(targetPos);
					break;
				}
				else //if there is something in the space
				{
					break;
				}
			}
			for (int i = pos.x; i >= 0; i--)
			{
				targetPos = Position(i, pos.y);
				if (board[i][pos.y].getColor() == NONE) //if there is nothing in the space
				{
					validPos.push_back(targetPos);
				}
				if (board[i][pos.y].getColor() == WHITE) //if there is something can eat
				{
					validPos.push_back(targetPos);
					break;
				}
				else //if there is something in the space
				{
					break;
				}
			}
			for (int i = pos.y + 1; i < 8; i++)
			{
				targetPos = Position(pos.x, i);
				if (board[pos.x][i].getColor() == NONE) //if there is nothing in the space
				{
					validPos.push_back(targetPos);
				}
				if (board[pos.x][i].getColor() == WHITE) //if there is something can eat
				{
					validPos.push_back(targetPos);
					break;
				}
				else //if there is something in the space
				{
					break;
				}
			}
			for (int i = pos.y; i >= 0; i--)
			{
				targetPos = Position(pos.x, i);
				if (board[pos.x][i].getColor() == NONE) //if there is nothing in the space
				{
					validPos.push_back(targetPos);
				}
				if (board[pos.x][i].getColor() == WHITE) //if there is something can eat
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
		case PAWN:
			if (pos.y == 1) //the first step, can move one or tow
			{
				for (int i = 1; i <= 2; i++) //can move one or two step, check if can move
				{
					targetPos = Position(pos.x, pos.y + i);
					if (board[pos.x][pos.y + i].getColor() == NONE) //if there is nothing in the space
					{
						validPos.push_back(targetPos);
					}
					else //if there is something in the space
					{
						break;
					}
				}
			}
			else if (pos.y + 1 < 8) //not the first step, can move one step
			{
				targetPos = Position(pos.x, pos.y + 1);
				if (board[pos.x][pos.y + 1].getColor() == NONE) //if there is nothing in the sapce
				{
					validPos.push_back(targetPos);
				}
			}
			if (pos.x + 1 < 8 && pos.y + 1 < 8 && board[pos.x + 1][pos.y + 1].getColor() == WHITE) //if there is something can eat
			{
				targetPos = Position(pos.x + 1, pos.y + 1);
				validPos.push_back(targetPos);
			}
			else if (pos.x - 1 >= 0 && pos.y + 1 < 8 && board[pos.x - 1][pos.y + 1].getColor() == WHITE) //if there is something can eat
			{
				targetPos = Position(pos.x - 1, pos.y + 1);
				validPos.push_back(targetPos);
			}
			break;
		}
	}
	else if (color == WHITE)
	{
		switch (label)
		{
		case KING:
			break;
		case QUEEN:
			break;
		case BISHOP:
			break;
		case KNIGHT:
			break;
		case ROOK:
			for (int i = pos.x; i < 8; i++)
			{
				targetPos = Position(i, pos.y);
				if (board[i][pos.y].getColor() == NONE) //if there is nothing in the space
				{
					validPos.push_back(targetPos);
				}
				if (board[i][pos.y].getColor() == BLACK) //if there is something can eat
				{
					validPos.push_back(targetPos);
					break;
				}
				else //if there is something in the space
				{
					break;
				}
			}
			for (int i = pos.x; i >= 0; i--)
			{
				targetPos = Position(i, pos.y);
				if (board[i][pos.y].getColor() == NONE) //if there is nothing in the space
				{
					validPos.push_back(targetPos);
				}
				if (board[i][pos.y].getColor() == BLACK) //if there is something can eat
				{
					validPos.push_back(targetPos);
					break;
				}
				else //if there is something in the space
				{
					break;
				}
			}
			for (int i = pos.y + 1; i < 8; i++)
			{
				targetPos = Position(pos.x, i);
				if (board[pos.x][i].getColor() == NONE) //if there is nothing in the space
				{
					validPos.push_back(targetPos);
				}
				if (board[pos.x][i].getColor() == BLACK) //if there is something can eat
				{
					validPos.push_back(targetPos);
					break;
				}
				else //if there is something in the space
				{
					break;
				}
			}
			for (int i = pos.y; i >= 0; i--)
			{
				targetPos = Position(pos.x, i);
				if (board[pos.x][i].getColor() == NONE) //if there is nothing in the space
				{
					validPos.push_back(targetPos);
				}
				if (board[pos.x][i].getColor() == BLACK) //if there is something can eat
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
		case PAWN:
			if (pos.y == 6) //has not moved
			{
				for (int i = 1; i <= 2; i++) //can move one or two step, check if can move
				{
					targetPos = Position(pos.x, pos.y - i);
					if (board[pos.x][pos.y - i].getColor() == NONE) //if there is nothing in the space
					{
						validPos.push_back(targetPos);
					}
					else //if there is something in the space
					{
						break;
					}
				}
			}
			else if (pos.y - 1 >= 0) //not the first step, can move one step
			{
				targetPos = Position(pos.x, pos.y - 1);
				if (board[pos.x][pos.y - 1].getColor() == NONE) //if there is nothing in the space
				{
					validPos.push_back(targetPos);
				}
			}
			if (pos.x + 1 < 8 && pos.y - 1 >= 0 && board[pos.x + 1][pos.y - 1].getColor() == BLACK) //if there is something can eat
			{
				targetPos = Position(pos.x + 1, pos.y - 1);
				validPos.push_back(targetPos);
			}
			else if (pos.x - 1 >= 0 && pos.y - 1 >= 0 && board[pos.x - 1][pos.y - 1].getColor() == BLACK) //if there is something can eat
			{
				targetPos = Position(pos.x - 1, pos.y - 1);
				validPos.push_back(targetPos);
			}
			break;
		}
	}
	return validPos;
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