#include <iostream>
#include "DataType.h"
#include "Chess.h"

int main() {
	Chess::init();
	Player player, enemy, p1 = WHITE, p2 = BLACK;
	std::string status, who, canUndo, canRedo, value, board;
	while (true)
	{
		player = Chess::getNowPlayer();
		enemy = Chess::getNowEnemy();
		std::string mode;
		std::cin >> mode;

		if (mode == "undo") value = Chess::undo() ? "success" : "failed";
		if (mode == "redo") value = Chess::redo() ? "success" : "failed";
		if (mode == "preview") {
			Position point;
			std::cin >> point.x >> point.y;
			value = Chess::getMaskBoard(point);
		}
		if (mode == "move") {
			Position source, target;
			std::cin >> source.x >> source.y >> target.x >> target.y;
			value = Chess::move(player, source, target) ? "success" : "failed";
		}

		// if (!board.checkMovement(player)) status = "draw";
        /*else*/ if (Chess::isCheck()) status = "check";
		// else if (board.checkWin(player)) status = "win";
		else status = "playing";

		canUndo = '0' + Chess::canUndo();
		canRedo = '0' + Chess::canRedo();

		bool changeRound = value == "success";
		if (changeRound) who = enemy == WHITE ? "white" : "black";
		else who = player == WHITE ? "white" : "black";
		board = Chess::getBoard();

		std::cout << status << ";" << who << ";" << canUndo << ";" << canRedo << ";" << value << ";" << board << std::endl;
	}
}