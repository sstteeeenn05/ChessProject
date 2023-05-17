#include "ViewManager.h"

void ViewManager::init() {
    gameBoard.init();
}

void ViewManager::init(std::string boardCode, std::string turn, std::string castling) {
    gameBoard.init(boardCode, turn, castling);
}

int ViewManager::inputCommandAndPrint() {
    while (true) {
        std::string status, who, canUndo, canRedo, value, maskBoard, board;
        player = gameBoard.getNowPlayer();
        enemy = gameBoard.getNowEnemy();
        std::string mode;
        std::cin >> mode;

        if (mode == "exit") return 0;
        if (mode == "undo") value = gameBoard.undo() ? "success" : "failed";
        if (mode == "redo") value = gameBoard.redo() ? "success" : "failed";
        if (mode == "preview") {
            Position point;
            std::cin >> point.x >> point.y;
            maskBoard = gameBoard.getMaskBoard(point);
        }
        if (mode == "move") {
            Position source, target;
            std::cin >> source.x >> source.y >> target.x >> target.y;
            value = gameBoard.move(player, source, target) ? "success" : "failed";
        }

        if (gameBoard.isWinOrTie() == WIN) status = "win";
        else if (gameBoard.isWinOrTie() == TIE) status = "tie";
        else if (gameBoard.isCheck()) status = "check";
        else status = "playing";

        canUndo = '0' + gameBoard.canUndo();
        canRedo = '0' + gameBoard.canRedo();

        bool changeRound = value == "success";
        if (changeRound && gameBoard.isWinOrTie() == PLAYING) who = enemy == WHITE ? "white" : "black";
        else who = player == WHITE ? "white" : "black";
        board = Board::getBoard();

        std::cout << status << ";" << who << ";" << canUndo << ";" << canRedo << ";" << value << ";" << maskBoard << ";"
                  << board << std::endl;
    }
}
