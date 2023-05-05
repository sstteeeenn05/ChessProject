#include "Header.h"
#include "Broad.h"
#include "Chess.h"
#include "Player.h"
#include "Position.h"

void printBoard(Chess* source); //print board

int main()
{
	Board board;
	Player p1, p2;
	p1.setColor(WHITE);
	p2.setColor(BLACK);
	int count = 1;
	printBoard(board.getBoard());
	while (true)
	{
		if (count % 2 == 0)
		{
			board.move(p2);
		}
		else
		{
			board.move(p1);
		}
		count++;
		system("cls");
		printBoard(board.getBoard());
	}
}

void printBoard(Chess* source) //print board
{
	Chess board[8][8];
	for (int i = 0; i < 8; i++) //give the data to board
	{
		for (int j = 0; j < 8; j++) //give the data to board
		{
			board[i][j] = *(source + ((i * 8) + j));
		}
	}
	std::cout << "  a b c d e f g h" << std::endl;
	for (int i = 0; i < 8; i++) //output board
	{
		std::cout << 8 - i << " ";
		for (int j = 0; j < 8; j++) //output board
		{
			switch (board[i][j].getLabel())
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