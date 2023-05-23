#include "Game.h"

void Game::init() {
    gameBoard.init();
}

void Game::init(std::string boardCode, std::string turn, std::string castling) {
    gameBoard.init(boardCode, turn, castling);
}

int Game::inputCommandAndPrint() {
    std::string key;
    std::cin >> key;
    if (key == "start") std::cout << ";playing;" << (gameBoard.getNowPlayer()==WHITE?"white":"black") << ";0;0;;;" << gameBoard.getBoard();
    else return 0;
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
            value = maskBoard.length() ? "success" : "failed";
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

        bool changeRound = value == "success" && mode != "preview";
        if (changeRound && gameBoard.isWinOrTie() == PLAYING) who = enemy == WHITE ? "white" : "black";
        else who = player == WHITE ? "white" : "black";
        board = Board::getBoard();

        std::cout << mode << ";" << status << ";" << who << ";" << canUndo << ";" << canRedo << ";" << value << ";" << maskBoard << ";"
                  << board << std::endl;
    }
}
