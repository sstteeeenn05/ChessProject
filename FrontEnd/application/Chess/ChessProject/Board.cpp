#include "Board.h"

Chess Board::board[8][8] = {};

//intent: Initialize the board
//pre: none
//post: none
void Board::init() {
    isChecking = false;
    status = PLAYING;
    logIndex = 0;
    beginLogIndex = 0;
    logList.clear();
    kingsPos[0] = {4, 7};
    kingsPos[1] = {4, 0};
    Type typeList[8] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};

    // Initialize the first row of the board with black pieces
    for (int i = 0; i < 8; i++) board[0][i] = Chess(BLACK, {i, 0}, typeList[i]);

    // Initialize the second row of the board with black pawns
    for (int i = 0; i < 8; i++) board[1][i] = Chess(BLACK, {i, 1}, PAWN);

    // Initialize the middle rows of the board with empty spaces
    for (int i = 2; i < 6; i++) for (int j = 0; j < 8; j++) board[i][j] = Chess({j, i});

    // Initialize the seventh row of the board with white pawns
    for (int i = 0; i < 8; i++) board[6][i] = Chess(WHITE, {i, 6}, PAWN);

    // Initialize the eighth row of the board with white pieces
    for (int i = 0; i < 8; i++) board[7][i] = Chess(WHITE, {i, 7}, typeList[i]);
}

//intent: Initialize the board with a given board code, turn, and castling information
//pre: 
//post:
void Board::init(std::string boardCode, std::string turn, std::string castling) {
    const std::map<char, Type> typeMap = {
            {'K', KING},
            {'Q', QUEEN},
            {'B', BISHOP},
            {'N', KNIGHT},
            {'R', ROOK},
            {'P', PAWN},
    };

    isChecking = false;
    status = PLAYING;
    logIndex = 0;
    beginLogIndex = 0;
    logList.clear();

    int rowIndex = 0, colIndex = 0;

    // Parse the board code and initialize the board accordingly
    for (auto &item: boardCode) {
        // If a slash ('/') is encountered, move to the next row
        if (item == '/') {
            rowIndex++;
            colIndex = 0;
        }
        else if (std::isdigit(item)) { // If a digit is encountered, it represents empty squares
            // Fill the specified number of empty squares in the current row
            for (int i = 0; i < item - '0'; i++, colIndex++) {
                board[rowIndex][colIndex] = Chess({colIndex, rowIndex});
            }
        }
        else if (std::isupper(item)) { // If an uppercase letter is encountered, it represents a white piece
            board[rowIndex][colIndex] = Chess(WHITE, {colIndex, rowIndex}, typeMap.at(item));

            // If the piece is a king, update its position in kingsPos
            if (getChess({colIndex, rowIndex}).data.type == KING)
                kingsPos[WHITE] = {colIndex, rowIndex};

            colIndex++;
        }
        else if (std::islower(item)) { // If a lowercase letter is encountered, it represents a black piece
            board[rowIndex][colIndex] =
                    Chess(BLACK, {colIndex, rowIndex}, typeMap.at((char) std::toupper(item)));

            // If the piece is a king, update its position in kingsPos
            if (getChess({colIndex, rowIndex}).data.type == KING)
                kingsPos[BLACK] = {colIndex, rowIndex};

            colIndex++;
        }
    }

    // If it is currently black's turn, initialize the logList and update logIndex and beginLogIndex
    if (std::tolower(turn[0]) == 'b') {
        logList.emplace_back();
        logIndex = 1;
        beginLogIndex = 1;
    }

    // Update moved status for kings and rooks based on castling information
    if (castling == "-") {
        getChess(kingsPos[0]).data.moved = true;
        getChess(kingsPos[1]).data.moved = true;
    }
    const Position rookPos[4] = {{0, 0},
                                 {7, 0},
                                 {0, 7},
                                 {7, 7}};

    // Update the moved status for rooks involved in castling
    for (auto &pos: rookPos) getChess(pos).data.moved = true;

    // Parse the castling information and update the moved status for rooks involved in castling
    for (auto &item: castling) {
        // If the letter represents kingside castling
        if (std::toupper(item) == 'K') {
            // If the letter is uppercase, it represents white's kingside castling
            if (isupper(item)) {
                getChess(rookPos[3]).data.moved = false;
            }
            else {
                getChess(rookPos[1]).data.moved = false;
            }
        }
        else if (std::toupper(item) == 'Q') { // If the letter represents queenside castling
            //
            if (isupper(item)) { // If the letter is uppercase, it represents white's queenside castling
                getChess(rookPos[2]).data.moved = false;
            }
            else {
                getChess(rookPos[0]).data.moved = false;
            }
        }
    }

}

// intent: Retrieve the chess piece at the given position on the board
// pre: The board is initialized and the position is valid
// post: Returns a reference to the chess piece at the given position
Chess &Board::getChess(Position pos) {
    return board[pos.y][pos.x];
}

// intent: Get the current state of the chessboard as a string
// pre: The board is initialized
// post: Returns a string representation of the chessboard
std::string Board::getBoard() {
    std::string output;

    // Iterate over each row of the board
    for (auto &i: board) {
        // Iterate over each chess piece in the row
        for (auto &c: i) {
            // If the piece is empty or belongs to the white player, use the uppercase character
            if (c.data.type == EMPTY || c.data.player == WHITE) output += char(c.data.type);
            else output += char(c.data.type + ('a' - 'A'));
        }
        //output += '\n';
    }
    return output;
}

// intent: Get the mask board indicating the valid movement positions for a chess piece at the target position
// pre: The board is initialized, and the target position is valid
// post: Returns a string representation of the mask board, where '1' indicates a valid position and '0' indicates an invalid position
std::string Board::getMaskBoard(Position target) {
    std::string output;

    // Check if the chess piece at the target position belongs to the current player
    if (getChess(target).data.player != getNowPlayer()) return output;

    std::vector<Position> posList = getValidPos(target);
    checkMovement(target, posList);
    bool maskBoard[8][8] = {false};

    // Set the corresponding positions in the mask board to true for valid positions
    for (const auto &pos: posList) {
        if (pos.x != -1 && pos.y != -1) {
            maskBoard[pos.y][pos.x] = true;
        }
    }

    // Convert the mask board to a string representation
    for (auto &i: maskBoard) {
        for (bool j: i) output += char('0' + j);
        //output += '\n';
    }

    return output;
}

// intent: Check if there are moves to undo
// pre: The board is initialized
// post: Returns true if there are moves to undo, false otherwise
bool Board::canUndo() {
    return logIndex > beginLogIndex;
}

// intent: Check if there are moves to redo
// pre: The board is initialized
// post: Returns true if there are moves to redo, false otherwise
bool Board::canRedo() {
    return logIndex < logList.size();
}

// intent: Undo the last move
// pre: The board is initialized and there are moves to undo
// post: Returns true if the undo operation is successful, false otherwise
bool Board::undo() {
    // Check if there are moves to undo
    if (!canUndo()) return false;
    int tempIndex = logIndex - 1;

    // Iterate through the changes in the last move and revert them
    for (auto change = logList[tempIndex].rbegin(); change != logList[tempIndex].rend(); change++) {
        const auto &before = change->first;
        const auto &after = change->second;
        board[after.position.y][after.position.x].data = before;

        // Update kingsPos if the chess piece being reverted is a king
        if (before.type == KING) {
            kingsPos[before.player] = before.position;
        }
    }
    isChecking = calculateCheck(getNowPlayer());
    logIndex--;

    return true;
}

// intent: Redo the last undone move
// pre: The board is initialized and there are moves to redo
// post: Returns true if the redo operation is successful, false otherwise
bool Board::redo() {
    // Check if there are moves to redo
    if (!canRedo()) return false;

    // Iterate through the changes in the next move and apply them
    for (const auto &change: logList[logIndex]) {
        const auto &before = change.first;
        const auto &after = change.second;
        board[before.position.y][before.position.x].data = after;

        // Update kingsPos if the chess piece being restored is a king
        if (after.type == KING) {
            kingsPos[after.player] = after.position;
        }
    }

    isChecking = calculateCheck(getNowPlayer());
    logIndex++;

    return true;
}

// intent: Get a list of valid positions for a given chess piece
// pre: The board is initialized
// post: Returns a vector of valid positions where the chess piece at the target position can move
std::vector<Position> Board::getValidPos(Position target) {
    std::vector<Position> output;
    Chess &chess = board[target.y][target.x];

    // Determine the type of chess piece and call the appropriate check functions
    switch (chess.data.type) {
        case KING:
            chess.checkSquare(output);
            chess.checkCastling(output);
            break;
        case QUEEN:
            chess.checkStraight(output);
            chess.checkCross(output);
            break;
        case BISHOP:
            chess.checkCross(output);
            break;
        case KNIGHT:
            chess.checkL(output);
            break;
        case ROOK:
            chess.checkStraight(output);
            break;
        case PAWN:
            chess.checkPawn(output);
            break;
    }

    return output;
}

// intent: Check if a move from the source position to the target position is valid
// pre: The board is initialized, source and target positions are valid positions on the board
// post: Returns true if the move is valid, false otherwise
bool Board::canMove(Position source, Position target) {
    std::vector<Position> validPos = getValidPos(source);
    checkMovement(source, validPos);
    return std::any_of(validPos.begin(), validPos.end(),
                       [&](const auto &pos) { return pos == target; });
}

// intent: Move a chess piece from the source position to the target position
// pre: The board is initialized, player is the current player, source and target positions are valid positions on the board
// post: Returns true if the move is successful, false otherwise
bool Board::move(Player player, Position source, Position target) {
    // Check if it is the player's turn
    if (player != getNowPlayer()) return false;

    // Check if the move is valid
    if (!canMove(source, target)) return false;

    // Remove future moves from the log if there are any
    while (logList.size() > logIndex) logList.pop_back();

    Log log;
    Chess &sChess = getChess(source);
    Chess &tChess = getChess(target);

    // Update kingsPos if the moved chess piece is a king
    if (sChess.data.type == KING) {
        kingsPos[sChess.data.player] = tChess.data.position;
        checkCastling(sChess, tChess, log);
    }

    // Check en passant and update the log
    if (sChess.data.type == PAWN) checkEnPassant(sChess, tChess, log);

    clearEnPassanting(log);

    log.emplace_back(sChess.data, sChess.data.previewClear());
    log.emplace_back(tChess.data, tChess.data.previewSet(sChess.data));
    tChess.data.set(sChess.data);
    sChess.data.clear();
    
    // Check if the moved pawn is eligible for en passant capture in the next move
    if (tChess.data.type == PAWN) checkNextEnPassant(tChess, log);

    // Check if the pawn is eligible for promotion and perform promotion if necessary
    if (tChess.checkPromotion()) {
        Type promotion = tChess.doPromotion();
        log.emplace_back(tChess.data, tChess.data.previewSetType(promotion));
        tChess.data.setType(promotion);
    }

    log.emplace_back(tChess.data, tChess.data.previewMoved());
    tChess.data.moved = true;

    isChecking = calculateCheck(getNowPlayer());
    Chess::isChecking = isChecking;

    status = calculateWinOrTie(getNowEnemy());

    logList.push_back(log);

    logIndex = logList.size();

    return true;
}

// intent: Check and handle castling move if applicable
// pre: The board is initialized, sChess and tChess are valid Chess objects representing the source and target positions
// post: If castling is possible, performs the castling move and updates the log accordingly
void Board::checkCastling(Chess& sChess, Chess& tChess, Log& log) {
    // Check if the target position is one of the castling positions and the source chess piece (king) has not moved
    if ((tChess.data.position.x == 2 || tChess.data.position.x == 6) && !sChess.data.moved) {
        Chess& sRook = getChess({ (tChess.data.position.x == 2) ? 0 : 7, sChess.data.position.y });
        Chess& tRook = getChess({ (tChess.data.position.x == 2) ? 3 : 5, sChess.data.position.y });
        log.emplace_back(sRook.data, sRook.data.previewMoved());
        sRook.data.moved = true;

        log.emplace_back(sRook.data, sRook.data.previewClear());
        log.emplace_back(tRook.data, tRook.data.previewSet(sRook.data));
        tRook.data.set(sRook.data);
        sRook.data.clear();
    }
}

// intent: Check and handle en passant move if applicable
// pre: The board is initialized, sChess and tChess are valid Chess objects representing the source and target positions
// post: If en passant capture is possible, performs the capture and updates the log accordingly
void Board::checkEnPassant(Chess& sChess, Chess& tChess, Log& log) {
    const auto [leftEnPassant, rightEnPassant] = sChess.getEnPassant();
    auto [left, right] = sChess.getSidePos();
    Chess& lChess = getChess(left);
    Chess& rChess = getChess(right);
    
    // Check if left en passant capture is possible and the target position matches the generated position
    if (leftEnPassant && tChess.data.position == sChess.generatePosByPlayer({ -1, 1 })) {
        log.emplace_back(lChess.data, lChess.data.previewClear());
        lChess.data.clear();
    }

    // Check if right en passant capture is possible and the target position matches the generated position
    if (rightEnPassant && tChess.data.position == sChess.generatePosByPlayer({ 1, 1 })) {
        log.emplace_back(rChess.data, rChess.data.previewClear());
        rChess.data.clear();
    }
}

// intent: Clear the en passant status of all pawns on the board
// pre: The board is initialized
// post: Clears the en passant status of all pawns on the board and updates the log accordingly
void Board::clearEnPassanting(Log& log) {
    // Iterate through each chess piece on the board
    for (auto& row : board) {
        for (Chess& chess : row) {
            // Check if the chess piece has en passant status
            if (chess.data.enPassanting) {
                log.emplace_back(chess.data, chess.data.previewEnPassanted());
                chess.data.enPassanting = false;
            }
        }
    }
}

// intent: Check if the next pawn move can be captured en passant
// pre: The board is initialized, tChess is a valid Chess object representing the target position
// post: If the target position is on the river and there is a neighboring pawn, set en passant status and update the log
void Board::checkNextEnPassant(Chess& tChess, Log& log) {
    auto [left, right] = tChess.getSidePos();
    Chess& lChess = getChess(left);
    Chess& rChess = getChess(right);

    // Check if the target chess piece is on the river
    if (tChess.onMyRiver()) {
        // Check if there is a neighboring pawn
        if (lChess.data.type == PAWN || rChess.data.type == PAWN) {
            log.emplace_back(tChess.data, tChess.data.previewEnPassanting());
            tChess.data.enPassanting = true;
        }
    }
}

// intent: Get the player who is currently making a move
// pre: The board is initialized
// post: Returns the player who is currently making a move based on the log index
Player Board::getNowPlayer() const {
    return (logIndex & 1) ? BLACK : WHITE;
}

// intent: Get the enemy player of the player who is currently making a move
// pre: The board is initialized
// post: Returns the player who is the enemy of the player currently making a move based on the log index
Player Board::getNowEnemy() const {
    return (logIndex & 1) ? WHITE : BLACK;
}

// intent: Check if the current board state is in a check position
// pre: The board is initialized
// post: Returns true if the current board state is in a check position, false otherwise
bool Board::isCheck() const {
    return isChecking;
}

// intent: Calculate if the specified player is in a check position
// pre: The board is initialized, player is a valid player (WHITE or BLACK)
// post: Returns true if the specified player is in a check position, false otherwise
bool Board::calculateCheck(Player player) {
    bool result = false;

    // Iterate through all chess pieces on the board
    for (auto &rowElement: board) {
        for (auto &element: rowElement) {
            // Check if the chess piece belongs to the specified player
            if (element.data.player == player) {
                std::vector<Position> validPositions = getValidPos(element.data.position);
                result |= std::any_of(validPositions.begin(), validPositions.end(),
                                      [&](auto &pos) { return pos == kingsPos[element.data.enemy]; });
                if (result) return true; // If a check position is found, return true
            }
        }
    }

    return false;
}

// intent: Check the movement of a chess piece and update valid positions accordingly
// pre: The board is initialized, target is a valid position, validPos is a vector of valid positions
// post: Updates validPos based on the movement of the chess piece at the target position
void Board::checkMovement(const Position &target, std::vector<Position> &validPos) {
    // Iterate through valid positions
    for (auto &pos: validPos) {
        Chess sChess = board[target.y][target.x];
        Chess tChess = board[pos.y][pos.x];

        board[pos.y][pos.x] = sChess;
        board[pos.y][pos.x].data.position = pos;
        board[target.y][target.x] = Chess(target);

        // Update the king's position if the moving chess piece is a king
        if (sChess.data.type == KING) kingsPos[sChess.data.player] = tChess.data.position;

        bool result = calculateCheck(sChess.data.enemy);

        board[pos.y][pos.x] = tChess;
        board[target.y][target.x] = sChess;

        // Restore the king's position if the moving chess piece is a king
        if (sChess.data.type == KING) kingsPos[sChess.data.player] = sChess.data.position;

        // If the player is in a check position, invalidate the current valid position
        if (result) pos = {-1, -1};
    }
}

// intent: Calculate the game status (WIN, TIE, or PLAYING)
// pre: The board is initialized, enemy is the enemy player of the player currently making a move
// post: Returns the game status based on the current board state
Status Board::calculateWinOrTie(Player enemy) {
    // Iterate through all chess pieces on the board
    for (auto &rowElement: board) {
        for (auto &element: rowElement) {
            // Check if the chess piece belongs to the enemy player
            if (element.data.player == enemy) {
                std::vector<Position> validPositions = getValidPos(element.data.position);
                checkMovement(element.data.position, validPositions);
                bool haveStep = std::any_of(validPositions.begin(), validPositions.end(),
                                            [](auto &pos) { return (pos.x != -1 || pos.y != -1); });
                if (haveStep) return PLAYING;// If there are valid positions available, the game is still in progress
            }
        }
    }

    // If the player is in a check position, it's a win, otherwise, it's a tle
    if (isChecking) return WIN;

    return TIE;
}

// intent: Get the current game status (WIN, TIE, or PLAYING)
// pre: The board is initialized
// post: Returns the current game status based on the board state
Status Board::isWinOrTie() {
    return status;
}