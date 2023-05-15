#include <iostream>
#include "DataType.h"
#include "Chess.h"

int main() {
	Chess::init();
	Player player, enemy, p1 = WHITE, p2 = BLACK;
	while (true)
	{
		std::string status, who, canUndo, canRedo, value, maskBoard, board;
		player = Chess::getNowPlayer();
		enemy = Chess::getNowEnemy();
		std::string mode;
		std::cin >> mode;

        if (mode == "exit") exit(0);
		if (mode == "undo") value = Chess::undo() ? "success" : "failed";
		if (mode == "redo") value = Chess::redo() ? "success" : "failed";
		if (mode == "preview") {
			Position point;
			std::cin >> point.x >> point.y;
			maskBoard = Chess::getMaskBoard(point);
		}
		if (mode == "move") {
			Position source, target;
			std::cin >> source.x >> source.y >> target.x >> target.y;
			value = Chess::move(player, source, target) ? "success" : "failed";
		}

        if (Chess::isWinOrTie() == WIN) status = "win";
        else if (Chess::isWinOrTie() == TIE) status = "tie";
        else if (Chess::isCheck()) status = "check";
        else status = "playing";

		canUndo = '0' + Chess::canUndo();
		canRedo = '0' + Chess::canRedo();

		bool changeRound = value == "success";
		if (changeRound && Chess::isWinOrTie() == PLAYING) who = enemy == WHITE ? "white" : "black";
		else who = player == WHITE ? "white" : "black";
		board = Chess::getBoard();

		std::cout << status << ";" << who << ";" << canUndo << ";" << canRedo << ";" << value << ";" << maskBoard << ";" << board << std::endl;
	}
}