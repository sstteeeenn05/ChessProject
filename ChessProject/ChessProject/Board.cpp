/*****************************************************************
* File: Board.cpp
 * Author: ��_�x�B�d����
 * Create Date: 2023/05/05
 * Editor: ��_�x�B�d����
 * Update Date: 2023/05/06
 * Description: set and update board
 *********************************************************************/

#include "Board.h"
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

	logs.clear();
}

void Board::printBoard()
{
	std::cout << "  a b c d e f g h" << std::endl;
	for (int i = 0; i < 8; i++) //output board
	{
		std::cout << 8 - i << " ";
		for (int j = 0; j < 8; j++) //output board
		{
			switch (board[i][j].getType())
			{
			case KING:
				if (board[i][j].getColor() == BLACK)
				{
					std::cout << "k ";
				}
				else
				{
					std::cout << "K ";
				}
				break;
			case QUEEN:
				if (board[i][j].getColor() == BLACK)
				{
					std::cout << "q ";
				}
				else
				{
					std::cout << "Q ";
				}
				break;
			case BISHOP:
				if (board[i][j].getColor() == BLACK)
				{
					std::cout << "b ";
				}
				else
				{
					std::cout << "B ";
				}
				break;
			case KNIGHT:
				if (board[i][j].getColor() == BLACK)
				{
					std::cout << "n ";
				}
				else
				{
					std::cout << "N ";
				}
				break;
			case ROOK:
				if (board[i][j].getColor() == BLACK)
				{
					std::cout << "r ";
				}
				else
				{
					std::cout << "R ";
				}
				break;
			case PAWN:
				if (board[i][j].getColor() == BLACK)
				{
					std::cout << "p ";
				}
				else
				{
					std::cout << "P ";
				}
				break;
			case EMPTY:
				std::cout << "  ";
				break;
			}
		}
		std::cout << std::endl;
	}
}

//intent: move the chess
//pre: whose turn
//post: none
bool Board::move(Player& player, Position source, Position target, const int& count)
{
	//if it is not this player's turn
	if (player.getColor() != board[source.y][source.x].getColor()) 
	{
		std::cout << "Fail" << std::endl;
		return false;
	}

	bool canCastle = false;

	if (board[source.y][source.x].getType() == KING && target.y == source.y && (target.x == 2 || target.x == 6))
	{
		canCastle = castling(board[source.y][source.x], target);
	}
	
	bool EnPassant = false;

	if (abs(target.y - source.y)==1 && abs(target.x - source.x) == 1 &&
		board[source.y][source.x].getType() == PAWN && board[source.y][target.x].getType() == PAWN)
	{
		EnPassant = enPassant(board[source.y][source.x], target, logs[count - 1]);
	}

	//if the target position is valid
	if (moveAvalible(board[source.y][source.x], target) || canCastle || EnPassant)
	{
		while (count < logs.size())
		{
			logs.pop_back();
		}
		Log record(board[source.y][source.x], board[target.y][target.x], canCastle, EnPassant);
		logs.push_back(record);
		
		if (board[source.y][target.x].getType() == KING)
		{
			if (player.getColor() == WHITE)
			{
				Chess::setWhiteKingPos(target);
			}
			else
			{
				Chess::setBlackKingPos(target);
			}
		}

		board[source.y][source.x].setMoved();
		board[target.y][target.x].setSpace(board[source.y][source.x]);
		board[source.y][source.x].setEmpty();

		//if is pawn check if it can promotion
		if (board[target.y][target.x].getType() == PAWN) 
		{
			board[target.y][target.x].checkPawn();
		}
		return true;
	}
	else 
	{
		return false;
	}
}

//intent: if this position is valid
//pre: which chess and the target position
//post:bool
bool Board::moveAvalible(Chess chess, Position target)
{
	std::vector<Position> validPos;
	validPos = chess.getValidPos(board);

	if (board[target.y][target.x].getColor() != chess.getColor())
	{
		//run int the vector saved valid positions
		for (auto& i : validPos)
		{

			//if the position if found
			if (i == target)
			{
				return true;
			}
		}
	}
	return false;
}

void Board::undo(int& count) //undo
{
	if (count == 0)
	{
		std::cout << "Fail" << std::endl;
		return;
	}
	else
	{
		Log record(logs[count - 1].source, logs[count - 1].target, logs[count - 1].castling, logs[count - 1].enPassant);
		if (record.castling)
		{
			if (record.target.getPos().x == 6)
			{
				board[record.source.getPos().y][7].setSpace(board[record.source.getPos().y][5]);
				board[record.source.getPos().y][5].setEmpty();
			}
			else
			{
				board[record.source.getPos().y][0].setSpace(board[record.source.getPos().y][3]);
				board[record.source.getPos().y][3].setEmpty();
			}
		}
		if (record.enPassant)
		{
			if (record.source.getPos().y == 4)
			{
				board[4][record.target.getPos().x].setChess(PAWN, WHITE);
			}
			else
			{
				board[3][record.target.getPos().x].setChess(PAWN, BLACK);
			}
		}
		board[record.source.getPos().y][record.source.getPos().x] = record.source;
		board[record.target.getPos().y][record.target.getPos().x] = record.target;
		count--;
		std::cout << "Success" << std::endl;
	}
}

void Board::redo(int& count) //redo
{
	if (count >= logs.size())
	{
		std::cout << "Fail" << std::endl;
		return;
	}
	else
	{
		Log record(logs[count].source, logs[count].target, logs[count].castling, logs[count].enPassant);
		if (record.castling)
		{
			if (record.target.getPos().x == 6)
			{
				board[record.source.getPos().y][5].setSpace(board[record.source.getPos().y][7]);
				board[record.source.getPos().y][7].setEmpty();
			}
			else
			{
				board[record.source.getPos().y][3].setSpace(board[record.source.getPos().y][0]);
				board[record.source.getPos().y][0].setEmpty();
			}
		}
		if (record.enPassant)
		{
			if (record.source.getPos().y == 4)
			{
				board[4][record.target.getPos().x].setEmpty();
			}
			else
			{
				board[3][record.target.getPos().x].setEmpty();
			}
		}
		board[record.target.getPos().y][record.target.getPos().x].setSpace(board[record.source.getPos().y][record.source.getPos().x]);
		board[record.source.getPos().y][record.source.getPos().x].setEmpty();
		count++;
		std::cout << "Success" << std::endl;
	}
}

bool Board::castling(Chess& chess, Position target)
{
	if (!chess.getMoved())
	{
		if (!(board[chess.getPos().y][0].getMoved()) && target.x == 2
			&&board[chess.getPos().y][2].getColor() != chess.getColor() && board[chess.getPos().y][3].getColor() != chess.getColor())
		{
			board[chess.getPos().y][3].setSpace(board[chess.getPos().y][0]);
			board[chess.getPos().y][0].setEmpty();
			return true;
		}
		if ((!board[chess.getPos().y][7].getMoved()) && target.x == 6
			&&board[chess.getPos().y][6].getColor() != chess.getColor() && board[chess.getPos().y][5].getColor() != chess.getColor())
		{
			board[chess.getPos().y][5].setSpace(board[chess.getPos().y][7]);
			board[chess.getPos().y][7].setEmpty();
			return true;
		}
	}
	return false;
}

bool Board::enPassant(Chess& chess, Position target, Log record)
{
	if (record.source.getType() != PAWN)
	{
		return false;
	}
	if (abs(record.source.getPos().y - record.target.getPos().y) != 2)
	{
		return false;
	}
	Color color = chess.getColor();
	if (color == BLACK)
	{
		if (chess.getPos().y != 4)
		{
			return false;
		}
		board[4][target.x].setEmpty();
		return true;
	}
	else
	{
		if (chess.getPos().y != 3)
		{
			return false;
		}
		board[3][target.x].setEmpty();
		return true;
	}
}

Color Board::colorOfPosition(int x, int y)
{
	return board[y][x].getColor();
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