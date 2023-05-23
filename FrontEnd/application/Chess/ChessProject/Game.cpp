#include "Game.h"

// intent: Initialize the game with default settings
// pre: None
// post: Initializes the game board with default settings
void Game::init() {
    gameBoard.init();
}

// intent: Initialize the game with custom settings
// pre: boardCode, turn, and castling are valid strings representing the board configuration, turn, and castling availability
// post: Initializes the game board with custom settings
void Game::init(std::string boardCode, std::string turn, std::string castling) {
    gameBoard.init(boardCode, turn, castling);
}

// intent: Handle user input and print game information
// pre: None
// post: Handles user input, performs game actions, and prints the game information
int Game::inputCommandAndPrint() {
    std::string key;
    std::cin >> key;

    // Start the game
    if (key == "start") std::cout << ";playing;" << (gameBoard.getNowPlayer()==WHITE?"white":"black") << ";0;0;;;" << gameBoard.getBoard();
    else return 0;

    while (true) {
        std::string status, who, canUndo, canRedo, value, maskBoard, board;
        player = gameBoard.getNowPlayer();
        enemy = gameBoard.getNowEnemy();
        std::string mode;
        std::cin >> mode;

        // Exit the game
        if (mode == "exit") return 0;

        // Undo the previous move
        if (mode == "undo") value = gameBoard.undo() ? "success" : "failed";

        // Redo the previously undone move
        if (mode == "redo") value = gameBoard.redo() ? "success" : "failed";

        // Preview the valid moves for a specific chess piece
        if (mode == "preview") {
            Position point;
            std::cin >> point.x >> point.y;
            maskBoard = gameBoard.getMaskBoard(point);
            value = maskBoard.length() ? "success" : "failed";
        }

        // Move a chess piece to a new position
        if (mode == "move") {
            Position source, target;
            std::cin >> source.x >> source.y >> target.x >> target.y;
            value = gameBoard.move(player, source, target) ? "success" : "failed";
        }
        
        // Determine the game status
        if (gameBoard.isWinOrTie() == WIN) status = "win";
        else if (gameBoard.isWinOrTie() == TIE) status = "tie"; 
        else if (gameBoard.isCheck()) status = "check"; 
        else status = "playing";

        canUndo = '0' + gameBoard.canUndo();
        canRedo = '0' + gameBoard.canRedo();

        bool changeRound = value == "success" && mode != "preview";

        // Determine the current player and update the game status
        if (changeRound && gameBoard.isWinOrTie() == PLAYING) who = enemy == WHITE ? "white" : "black";
        else who = player == WHITE ? "white" : "black";

        board = Board::getBoard();
        std::cout << mode << ";" << status << ";" << who << ";" << canUndo << ";" << canRedo << ";" << value << ";" << maskBoard << ";"
                  << board << std::endl;
    }
}
