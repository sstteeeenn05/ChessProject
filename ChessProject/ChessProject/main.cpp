#include <ctime>
#include "Header.h"
#include "Board.h"
#include "Chess.h"
#include "Position.h"


int main()
{
	clock_t startTime, endTime;
	startTime = clock();
	Board board;
	Player player, p1, p2;
	int checkFirst;
	std::cout << "Who goes first? 1.BLACK 2.WHITE" << std::endl;
	std::cin >> checkFirst;
	switch (checkFirst)
	{
	case 1:
		p1 = BLACK;
		p2 = WHITE;
		break;
	case 2:
	default:
		p1 = WHITE;
		p2 = BLACK;
		break;
	}
	int count = 0;
	system("cls");
	board.printBoard();
	while (true)
	{
		if (count % 2 == 1)
		{
			player = p2;
		}
		else
		{
			player = p1;
		}
		if (!board.checkMovement(player))
		{
			std::cout << "Draw!" << std::endl;
			break;
		}
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
			validMotion = board.move(player, source, target, count);
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
		board.printBoard();
		endTime = clock();
		std::cout << "Time : " << ((endTime - startTime) / CLOCKS_PER_SEC) / 60 << " min ";
		std::cout << ((endTime - startTime) / CLOCKS_PER_SEC) % 60 << " sec" << std::endl;
		if (board.checkWin(player))
		{
			if (player == WHITE)
			{
				std::cout << "White win!" << std::endl;
			}
			else
			{
				std::cout << "Black win!" << std::endl;
			}
			break;
		}
	}
	
}

