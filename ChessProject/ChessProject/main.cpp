#include "Header.h"
#include "Board.h"
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
	int count = 0;
	printBoard(board.getBoard());
	while (true)
	{
		char choose;
		std::cout << "u:undo r:redo" << std::endl;
		std::cin >> choose;
		switch (choose)
		{
		case 'u':
			board.undo(count);
			break;
		case 'r':
			board.redo(count);
			break;
		default:
			std::string a, b;
			Position source, target;
			std::cin >> a >> b;
			a = choose + a;
			source.x = a[0] - 'a';
			source.y = 8 - (a[1] - '0');
			target.x = b[0] - 'a';
			target.y = 8 - (b[1] - '0');
			bool validMotion = false;
			if (count % 2 == 1)
			{
				validMotion = board.move(p2, source, target, count);
			}
			else
			{
				validMotion = board.move(p1, source, target, count);
			}
			if (validMotion)
			{
				std::cout << "Success" << std::endl;
				count++;
			}
			else
			{
				std::cout << "Fail" << std::endl;
			}
			break;
		}
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