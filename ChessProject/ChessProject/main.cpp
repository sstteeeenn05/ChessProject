#include <ctime>
#include "Header.h"
#include "Board.h"
#include "Chess.h"
#include "Player.h"
#include "Position.h"

void printBoard(Chess* source); //print board

int main()
{
	clock_t startTime, endTime;
	startTime = clock();
	Board board;
	Player p1, p2;
	int checkFirst;
	std::cout << "Who goes first? 1.BLACK 2.WHITE" << std::endl;
	std::cin >> checkFirst;
	switch (checkFirst)
	{
	case 1:
		p1.setColor(BLACK);
		p2.setColor(WHITE);
		break;
	case 2:
	default:
		p1.setColor(WHITE);
		p2.setColor(BLACK);
		break;
	}
	int count = 0;
	system("cls");
	board.printBoard();
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
		board.printBoard();
		endTime = clock();
		std::cout << "Time : " << ((endTime - startTime) / CLOCKS_PER_SEC) / 60 << " min ";
		std::cout << ((endTime - startTime) / CLOCKS_PER_SEC) % 60 << " sec" << std::endl;
	}
}

