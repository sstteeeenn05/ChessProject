#include "Chess.h"

bool Chess::isChecking = false;

Chess::Chess(Position pos) : data(ChessData(pos)) {}

Chess::Chess(Player p, Position pos, Type t) : data(p, pos, t) {}

bool Chess::checkValid(Position pos, std::vector<Position> &output) const {
    const Chess &chess = Board::board[pos.y][pos.x];
    if (chess.data.player != data.player) {
        output.push_back({pos.x, pos.y});
    }
    return chess.data.player == NONE;
}

void Chess::checkCastling(std::vector<Position> &output) {
    if (data.moved) return;
    if (isChecking) return;

    const Position checkRookPos[2] = {
            {0, data.position.y},
            {7, data.position.y}
    };

    bool isRouteEmpty[2] = {true, true};
    for (auto &chess: Board::board[data.position.y]) {
        if (chess.data.position.x != data.position.x &&
            chess.data.position.x != 0 &&
            chess.data.position.x != 7 &&
            chess.data.type != EMPTY) {
            isRouteEmpty[chess.data.position.x > data.position.x] = false;
        }
    }

    for (auto &pos: checkRookPos) {
        auto &chess = Board::getChess(pos);
        if (chess.data.type == ROOK && !chess.data.moved &&
            isRouteEmpty[chess.data.position.x > data.position.x]) {
            if (pos.x) output.push_back({6, data.position.y});
            else output.push_back({2, data.position.y});
        }
    }

}

void Chess::checkStraight(std::vector<Position> &output) {
    for (int nowX = data.position.x + 1; nowX < 8; nowX++) {
        if (!checkValid({nowX, data.position.y}, output)) break;
    }
    for (int nowX = data.position.x - 1; nowX >= 0; nowX--) {
        if (!checkValid({nowX, data.position.y}, output)) break;
    }
    for (int nowY = data.position.y + 1; nowY < 8; nowY++) {
        if (!checkValid({data.position.x, nowY}, output)) break;
    }
    for (int nowY = data.position.y - 1; nowY >= 0; nowY--) {
        if (!checkValid({data.position.x, nowY}, output)) break;
    }
}

void Chess::checkCross(std::vector<Position> &output) const {
    for (int nowX = data.position.x + 1, nowY = data.position.y + 1; nowX < 8 && nowY < 8; nowX++, nowY++) {
        if (!checkValid({nowX, nowY}, output)) break;
    }
    for (int nowX = data.position.x - 1, nowY = data.position.y + 1; nowX >= 0 && nowY < 8; nowX--, nowY++) {
        if (!checkValid({nowX, nowY}, output)) break;
    }
    for (int nowX = data.position.x - 1, nowY = data.position.y - 1; nowX >= 0 && nowY >= 0; nowX--, nowY--) {
        if (!checkValid({nowX, nowY}, output)) break;
    }
    for (int nowX = data.position.x + 1, nowY = data.position.y - 1; nowX < 8 && nowY >= 0; nowX++, nowY--) {
        if (!checkValid({nowX, nowY}, output)) break;
    }
}

void Chess::checkSquare(std::vector<Position> &output) const {
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dy || dx) {
                Position p = data.position + Position{dx, dy};
                if (p.valid()) checkValid(p, output);
            }
        }
    }
}

void Chess::checkL(std::vector<Position> &output) const {
    Position posList[8] = {
            {1,  2},
            {-1, 2},
            {-1, -2},
            {1,  -2},
            {2,  1},
            {-2, 1},
            {-2, -1},
            {2,  -1}
    };
    for (const auto &pos: posList) {
        Position p = data.position + pos;
        if (p.valid()) checkValid(p, output);
    }
}

void Chess::checkPawn(std::vector<Position> &output) const {
    if (Board::getChess(generatePosByPlayer({0, 1})).data.type == EMPTY) {
        output.push_back(generatePosByPlayer({0, 1}));
        if (!data.moved && Board::getChess(generatePosByPlayer({0, 2})).data.type == EMPTY)
            output.push_back(generatePosByPlayer({0, 2}));
    }

    Position leftUp = generatePosByPlayer({-1, 1});
    Position rightUp = generatePosByPlayer({1, 1});
    bool leftUpEatable = leftUp.valid() && Board::getChess(leftUp).data.type != EMPTY;
    bool rightUpEatable = rightUp.valid() && Board::getChess(rightUp).data.type != EMPTY;
    const auto [leftEnPassant, rightEnPassant] = getEnPassant();

    if (leftUpEatable || leftEnPassant) checkValid(leftUp, output);
    if (rightUpEatable || rightEnPassant) checkValid(rightUp, output);
}

Position Chess::generatePosByPlayer(Position pos) const {
    return data.position + Position{pos.x, data.player == WHITE ? -pos.y : pos.y};
}

bool Chess::onRiver() const {
    return (data.player == WHITE && data.position.y == 3) || (data.player == BLACK && data.position.y == 4);
}

std::pair<Position, Position> Chess::getSidePos() const {
    return {
            data.position - Position{1, 0},
            data.position + Position{1, 0}
    };
}

std::pair<bool, bool> Chess::getEnPassant() const {
    auto [left, right] = getSidePos();
    return {
            !data.enPassanted && left.valid() && Board::getChess(left).data.type == PAWN &&
                    Board::getChess(left).data.player != data.player && onRiver(),
            !data.enPassanted && right.valid() && Board::getChess(right).data.type == PAWN &&
                    Board::getChess(right).data.player != data.player && onRiver()
    };
}

bool Chess::checkPromotion() const {
    return data.type == PAWN &&
           (data.player == WHITE && data.position.y == 0 ||
            data.player == BLACK && data.position.y == 7);
}

Type Chess::doPromotion() const {
    int choose = 0;
    do {
        std::cout << "playing;" << (data.player == WHITE ? "white" : "black") << ";;;promotion;;";
        std::cin >> choose;
        if (choose == 1) return QUEEN;

        if (choose == 2) return BISHOP;

        if (choose == 3) return KNIGHT;

        if (choose == 4) return ROOK;

        std::cout << "playing;" << (data.player == WHITE ? "white" : "black") << ";;;failed;;";
    } while (choose < 1 || choose > 4);
}