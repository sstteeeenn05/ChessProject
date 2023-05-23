#include "Chess.h"

bool Chess::isChecking = false;

Chess::Chess(Position pos) : data(ChessData(pos)) {}

Chess::Chess(Player p, Position pos, Type t) : data(p, pos, t) {}


//intent: Check if the given position is a valid move for the chess piece
//pre: None
//post: Returns a bool indicating if the move is valid and updates the 'output' vector with valid positions
bool Chess::checkValid(Position pos, std::vector<Position> &output) const {
    const Chess &chess = Board::board[pos.y][pos.x];

    //Check if it is empty or enermy
    if (chess.data.player != data.player) {
        output.push_back({pos.x, pos.y});
    }

    return chess.data.player == NONE;
}

// Intent: Check if castling is a valid move for the chess piece
// Pre: A vector to store the valid positions
// Post: Updates the 'output' vector with valid castling positions
void Chess::checkCastling(std::vector<Position> &output) {

    //If the check has moved then do nothing and return
    if (data.moved) return;

    //If is already check then do nothing and return
    if (isChecking) return;

    //Set up the rook pos
    const Position checkRookPos[2] = {
            {0, data.position.y},
            {7, data.position.y}
    };

    bool isRouteEmpty[2] = {true, true};

    // Loop through the chess pieces on the same row as the king
    for (auto &chess: Board::board[data.position.y]) {

        // Check if the chess piece is not at the king's position, not at the edge of the board and not an empty space
        if (chess.data.position.x != data.position.x &&
            chess.data.position.x != 0 &&
            chess.data.position.x != 7 &&
            chess.data.type != EMPTY) {
            isRouteEmpty[chess.data.position.x > data.position.x] = false;
        }
    }

    // Check for potential castling moves with each rook
    for (auto &pos: checkRookPos) {
        auto &chess = Board::getChess(pos);

        // If the chess piece is a rook, has not moved, and the route for castling is empty
        if (chess.data.type == ROOK && !chess.data.moved &&
            isRouteEmpty[chess.data.position.x > data.position.x]) {

            // Add the corresponding castling position to the output vector
            if (pos.x) output.push_back({6, data.position.y});
            else output.push_back({2, data.position.y});
        }
    }
}

//intent: Find all the valid positions on the straight line
//pre: The board and the vector to save all the valid positions
//post: None
void Chess::checkStraight(std::vector<Position> &output) {
    // Check valid positions in the positive x direction
    for (int nowX = data.position.x + 1; nowX < 8; nowX++) {
        // If the position is not valid or the move is invalid, stop checking in this direction
        if (!checkValid({nowX, data.position.y}, output)) break;
    }

    // Check valid positions in the negative x directio
    for (int nowX = data.position.x - 1; nowX >= 0; nowX--) {
        // If the position is not valid or the move is invalid, stop checking in this direction
        if (!checkValid({nowX, data.position.y}, output)) break;
    }

    // Check valid positions in the positive y direction
    for (int nowY = data.position.y + 1; nowY < 8; nowY++) {
        // If the position is not valid or the move is invalid, stop checking in this direction
        if (!checkValid({data.position.x, nowY}, output)) break;
    }

    // Check valid positions in the negative y direction
    for (int nowY = data.position.y - 1; nowY >= 0; nowY--) {
        // If the position is not valid or the move is invalid, stop checking in this direction
        if (!checkValid({data.position.x, nowY}, output)) break;
    }
}

// Intent: Find all the valid positions on the diagonal lines
// Pre: The board and the vector to save all the valid positions
// Post: Updates the 'output' vector with valid positions on the diagonal lines
void Chess::checkCross(std::vector<Position> &output) const {
    // Check valid positions in the diagonal line with positive slope in the first quadrant
    for (int nowX = data.position.x + 1, nowY = data.position.y + 1; nowX < 8 && nowY < 8; nowX++, nowY++) {
        // If the position is not valid or the move is invalid, stop checking in this direction
        if (!checkValid({nowX, nowY}, output)) break;
    }

    // Check valid positions in the diagonal line with negative slope in the second quadrant
    for (int nowX = data.position.x - 1, nowY = data.position.y + 1; nowX >= 0 && nowY < 8; nowX--, nowY++) {
        // If the position is not valid or the move is invalid, stop checking in this direction
        if (!checkValid({nowX, nowY}, output)) break;
    }

    // Check valid positions in the diagonal line with positive slope in the third quadrant
    for (int nowX = data.position.x - 1, nowY = data.position.y - 1; nowX >= 0 && nowY >= 0; nowX--, nowY--) {
        // If the position is not valid or the move is invalid, stop checking in this direction
        if (!checkValid({nowX, nowY}, output)) break;
    }

    // Check valid positions in the diagonal line with negative slope in the fourth quadrant
    for (int nowX = data.position.x + 1, nowY = data.position.y - 1; nowX < 8 && nowY >= 0; nowX++, nowY--) {
        // If the position is not valid or the move is invalid, stop checking in this direction
        if (!checkValid({nowX, nowY}, output)) break;
    }
}

// Intent: Find all the valid positions adjacent to the chess piece
// Pre: The board and the vector to save all the valid positions
// Post: Updates the 'output' vector with valid adjacent positions
void Chess::checkSquare(std::vector<Position> &output) const {
    // Iterate over the adjacent positions in a 3x3 square centered around the chess piece
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            // Skip the center position (0, 0)
            if (dy || dx) {
                Position p = data.position + Position{dx, dy};

                // Check if the calculated position is valid
                if (p.valid()) checkValid(p, output);
            }
        }
    }
}

// Intent: Find all the valid positions for a knight move
// Pre: The board and the vector to save all the valid positions
// Post: Updates the 'output' vector with valid knight move positions
void Chess::checkL(std::vector<Position> &output) const {
    Position posList[8] = { // List of possible relative positions for a knight move
            {1,  2},
            {-1, 2},
            {-1, -2},
            {1,  -2},
            {2,  1},
            {-2, 1},
            {-2, -1},
            {2,  -1}
    };

    // Iterate over the possible relative positions
    for (const auto &pos: posList) {
        Position p = data.position + pos;

        // Check if the calculated position is valid
        if (p.valid()) checkValid(p, output);
    }
}

// Intent: Find all the valid positions for a pawn move
// Pre: The board and the vector to save all the valid positions
// Post: Updates the 'output' vector with valid pawn move positions
void Chess::checkPawn(std::vector<Position> &output) const {
    // Check if the square in front of the pawn is empty
    if (Board::getChess(generatePosByPlayer({0, 1})).data.type == EMPTY) {
        output.push_back(generatePosByPlayer({0, 1}));

        // Check if the pawn hasn't moved yet and the two squares in front of it are also empty
        if (!data.moved && Board::getChess(generatePosByPlayer({0, 2})).data.type == EMPTY)
            output.push_back(generatePosByPlayer({0, 2}));
    }

    Position leftUp = generatePosByPlayer({-1, 1});
    Position rightUp = generatePosByPlayer({1, 1});
    bool leftUpEatable = leftUp.valid() && Board::getChess(leftUp).data.type != EMPTY;// Check if there is a piece that can be captured diagonally to the left
    bool rightUpEatable = rightUp.valid() && Board::getChess(rightUp).data.type != EMPTY;// Check if there is a piece that can be captured diagonally to the right
    const auto [leftEnPassant, rightEnPassant] = getEnPassant();

    // Check if capturing moves or en passant captures are valid and add them to the output vector
    if (leftUpEatable || leftEnPassant) checkValid(leftUp, output); 
    if (rightUpEatable || rightEnPassant) checkValid(rightUp, output); 
}

// Intent: Generate a new position based on the current position and the player's perspective
// Pre: The position to be added relative to the current position
// Post: Returns the generated position based on the player's perspective
Position Chess::generatePosByPlayer(Position pos) const {
    return data.position + Position{pos.x, data.player == WHITE ? -pos.y : pos.y};
}

// Intent: Check if the chess piece is on the river
// Pre: None
// Post: Returns a bool indicating whether the chess piece is on the river
bool Chess::onRiver() const {
    return (data.player == WHITE && data.position.y == 3) || (data.player == BLACK && data.position.y == 4);
}

bool Chess::onMyRiver() const {
    return (data.player == WHITE && data.position.y == 4) || (data.player == BLACK && data.position.y == 3);
}

// Intent: Get the positions adjacent to the current position on the same row
// Pre: None
// Post: Returns a pair of positions, representing the left and right adjacent positions 
std::pair<Position, Position> Chess::getSidePos() const {
    return {
            data.position - Position{1, 0},
            data.position + Position{1, 0}
    };
}

// Intent: Check if en passant move is possible for the current chess piece
// Pre: None
// Post: Returns a pair of bool values indicating whether the left and right en passant moves are possible
std::pair<bool, bool> Chess::getEnPassant() const {
    auto [left, right] = getSidePos();
    return {
            left.valid() && Board::getChess(left).data.type == PAWN && Board::getChess(left).data.enPassanting &&
            Board::getChess(left).data.player != data.player && onRiver(),
            right.valid() && Board::getChess(right).data.type == PAWN && Board::getChess(right).data.enPassanting &&
            Board::getChess(right).data.player != data.player && onRiver()
    };
}

// Intent: Check if the pawn is at the end of the board
// Pre: None
// Post: Returns a bool indicating whether the pawn is at the end of the board
bool Chess::checkPromotion() const {
    return data.type == PAWN &&
           (data.player == WHITE && data.position.y == 0 ||
            data.player == BLACK && data.position.y == 7);
}

// Intent: Execute promotion
// Pre: None
// Post: Returns the type of the chess the pawn is going to change into
Type Chess::doPromotion() const {
    int choose = 0;

    // Prompt the player to choose the type of chess piece for promotion
    do {
        std::cout << "move;playing;" << (data.player == WHITE ? "white" : "black") << ";;;promotion;;";
        std::cin >> choose;

        if (choose == 1) return QUEEN;

        if (choose == 2) return BISHOP;

        if (choose == 3) return KNIGHT;

        if (choose == 4) return ROOK;

        std::cout << "promotion;playing;" << (data.player == WHITE ? "white" : "black") << ";;;failed;;";
    } while (choose < 1 || choose > 4); //if the input isn't valid
}