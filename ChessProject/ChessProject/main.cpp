#include <ctime>
#include "Header.h"
#include "Board.h"
#include "Chess.h"
#include "Position.h"

int main()
{
	Board board;
	Player player, enemy, p1 = WHITE, p2 = BLACK;
	std::string status, who, canUndo, canRedo, value;
	while (true)
	{
		player = board.getLogIndex() & 1 ? p2 : p1;
		enemy = board.getLogIndex() & 1 ? p1 : p2;
		std::string mode;
		std::cin >> mode;

		if (mode == "undo") value = board.undo() ? "success" : "failed";
		if (mode == "redo") value = board.redo() ? "success" : "failed";
		if (mode == "preview") {
			Position point;
			std::cin >> point.x >> point.y;
			value = board.getMaskBoard(point);
		}
		if (mode == "move") {
			Position source, target;
			std::cin >> source.x >> source.y >> target.x >> target.y;
			value = board.move(player, source, target) ? "success" : "failed";
		}
		if (mode == "print") value = board.getBoard();

		if (!board.checkMovement(player)) status = "draw";
		else if (!board.checkMovement(enemy)) status = "checkmate";
		else if (board.checkWin(player)) status = "win";
		else status = "playing";

		canUndo = '0' + board.canUndo();
		canRedo = '0' + board.canRedo();

		bool changeRound = mode == "undo" || mode == "redo" || mode == "move";
		if (changeRound) who = enemy == WHITE ? "white" : "black";
		else who = player == WHITE ? "white" : "black";

		std::cout << status << ";" << who << ";" << canUndo << ";" << canRedo << ";" << value << std::endl;
	}
	
}

