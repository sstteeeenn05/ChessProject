#include "Chess.h"

std::vector<std::vector<std::pair<ChessData, ChessData>>> Chess::logList;
std::vector<std::pair<bool, bool>> Chess::isCheckedLogList;
int Chess::logIndex = 0;
Position Chess::kingsPos[2];
Chess Chess::board[8][8];
bool Chess::isChecked[2];
bool Chess::isChecking;
Status Chess::status;

Chess::Chess(Position pos) : data(ChessData(pos)) {}

Chess::Chess(Player p, Position pos, Type t) : data(p, pos, t) {}

void Chess::init() {
    memset(isChecked, false, sizeof(bool) * 2);
    isChecking = false;
    status = PLAYING;
    kingsPos[0] = {4, 7};
    kingsPos[1] = {4, 0};
    Type typeList[8] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};
    for (int i = 0; i < 8; i++) board[0][i] = Chess(BLACK, {i, 0}, typeList[i]);
    for (int i = 0; i < 8; i++) board[1][i] = Chess(BLACK, {i, 1}, PAWN);
    for (int i = 2; i < 6; i++) for (int j = 0; j < 8; j++) board[i][j] = Chess({j, i});
    for (int i = 0; i < 8; i++) board[6][i] = Chess(WHITE, {i, 6}, PAWN);
    for (int i = 0; i < 8; i++) board[7][i] = Chess(WHITE, {i, 7}, typeList[i]);
}

std::string Chess::getBoard() {
    std::string output;
    for (auto &i: board) {
        for (auto &c: i) {
            if (c.data.type == EMPTY || c.data.player == WHITE) output += char(c.data.type);
            else output += char(c.data.type + ('a' - 'A'));
        }
        //output += '\n';
    }
    return output;
}

std::string Chess::getMaskBoard(Position target) {
    std::string output;
    std::vector<Position> posList = getValidPos(target);
    checkMovement(target, posList);
    bool maskBoard[8][8] = {false};
    for (const auto &pos: posList) {
        if(pos.x != -1 && pos.y != -1) {
            maskBoard[pos.y][pos.x] = true;
        }
    }
    for (auto &i: maskBoard) {
        for (bool j: i) output += char('0' + j);
        //output += '\n';
    }
    return output;
}

Chess &Chess::getChess(Position pos) {
    return board[pos.y][pos.x];
}

bool Chess::canUndo() {
    return logIndex > 0;
}

bool Chess::canRedo() {
    return logIndex < logList.size();
}

bool Chess::undo() {
    if (!canUndo()) return false;
    int tempIndex = logIndex - 1;

    isChecked[0] = isCheckedLogList[tempIndex].first;
    isChecked[1] = isCheckedLogList[tempIndex].second;
    for (auto change = logList[tempIndex].rbegin(); change != logList[tempIndex].rend(); change++) {
        const auto &before = change->first;
        const auto &after = change->second;
        board[after.position.y][after.position.x].data = before;
        if (before.type == KING) {
            kingsPos[before.player] = before.position;
        }
    }
    isChecking = calculateCheck(getNowPlayer());
    logIndex--;

    return true;
}

bool Chess::redo() {
    if (!canRedo()) return false;

    isChecked[0] = isCheckedLogList[logIndex].first;
    isChecked[1] = isCheckedLogList[logIndex].second;
    for (const auto &change: logList[logIndex]) {
        const auto &before = change.first;
        const auto &after = change.second;
        board[before.position.y][before.position.x].data = after;
        if (after.type == KING) {
            kingsPos[after.player] = after.position;
        }
    }

    isChecking = calculateCheck(getNowPlayer());
    logIndex++;

    return true;
}

std::vector<Position> Chess::getValidPos(Position target) {
    std::vector<Position> output;
    Chess &chess = board[target.y][target.x];
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

bool Chess::checkValid(Position pos, std::vector<Position> &output) const {
    const Chess &chess = board[pos.y][pos.x];
    if (chess.data.player != data.player) {
        output.push_back({pos.x, pos.y});
    }
    return chess.data.player == NONE;
}

void Chess::checkCastling(std::vector<Position> & output) {
    if(data.moved) return;
    if(isChecking) return;

    const Position checkRookPos[2] = {
            {0, data.position.y},
            {7, data.position.y}
    };

    bool isRouteEmpty[2] = {true, true};
    for(auto &chess : board[data.position.y]) {
        if(chess.data.position.x != data.position.x &&
            chess.data.position.x != 0 &&
            chess.data.position.x != 7 &&
            chess.data.type != EMPTY) {
            isRouteEmpty[chess.data.position.x > data.position.x] = false;
        }
    }

    for(auto &pos : checkRookPos) {
        auto& chess = getChess(pos);
        if(chess.data.type == ROOK && !chess.data.moved &&
            isRouteEmpty[chess.data.position.x > data.position.x]) {
            if(pos.x) output.push_back({6, data.position.y});
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

void Chess::checkPawn(std::vector<Position> &output) {
    if (getChess(generatePosByPlayer({0, 1})).data.type == EMPTY) {
        output.push_back(generatePosByPlayer({0, 1}));
        if (!data.moved && getChess(generatePosByPlayer({0, 2})).data.type == EMPTY)
            output.push_back(generatePosByPlayer({0, 2}));
    }

    Position leftUp = generatePosByPlayer({-1, 1});
    Position rightUp = generatePosByPlayer({1, 1});
    bool leftUpEatable = leftUp.valid() && getChess(leftUp).data.type != EMPTY;
    bool rightUpEatable = rightUp.valid() && getChess(rightUp).data.type != EMPTY;
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

std::pair<bool, bool> Chess::getEnPassant() {
    auto [left, right] = getSidePos();
    return {
            !data.enPassanted && left.valid() && getChess(left).data.type == PAWN &&
            getChess(left).data.player != data.player && onRiver(),
            !data.enPassanted && right.valid() && getChess(right).data.type == PAWN &&
            getChess(right).data.player != data.player && onRiver()
    };
}

bool Chess::canMove(Position source, Position target) {
    std::vector<Position> validPos = getValidPos(source);
    checkMovement(source, validPos);
    return std::any_of(validPos.begin(), validPos.end(),
                       [&](const auto &pos) { return pos == target; });
}

bool Chess::move(Player player, Position source, Position target) {
    if (player != getNowPlayer()) return false;

    if (!canMove(source, target)) return false;

    while (logList.size() > logIndex) logList.pop_back();

    std::vector<std::pair<ChessData, ChessData>> log;
    Chess &sChess = board[source.y][source.x];
    Chess &tChess = board[target.y][target.x];

    if (sChess.data.type == KING) {
        kingsPos[sChess.data.player] = tChess.data.position;
    }

    if (sChess.data.type == PAWN) {
        const auto [leftEnPassant, rightEnPassant] = sChess.getEnPassant();
        auto [left, right] = sChess.getSidePos();
        Chess &lChess = getChess(left);
        Chess &rChess = getChess(right);

        if (leftEnPassant && target == sChess.generatePosByPlayer({-1, 1})) {
            log.emplace_back(sChess.data, sChess.data.previewEnPassant());
            sChess.data.enPassanted = true;
            log.emplace_back(lChess.data, lChess.data.previewClear());
            lChess.data.clear();
        }

        if (rightEnPassant && target == sChess.generatePosByPlayer({1, 1})) {
            log.emplace_back(sChess.data, sChess.data.previewEnPassant());
            sChess.data.enPassanted = true;
            log.emplace_back(rChess.data, rChess.data.previewClear());
            rChess.data.clear();
        }
    }

    log.emplace_back(sChess.data, sChess.data.previewClear());
    log.emplace_back(tChess.data, tChess.data.previewSet(sChess.data));
    tChess.data.set(sChess.data);
    sChess.data.clear();

    if (tChess.checkPromotion()) {
        Type promotion = tChess.doPromotion();
        log.emplace_back(tChess.data, tChess.data.previewSetType(promotion));
        tChess.data.setType(promotion);
    }

    log.emplace_back(tChess.data, tChess.data.previewMoved());
    tChess.data.moved = true;

    isChecking = calculateCheck(getNowPlayer());
    isChecked[getNowEnemy()] |= isChecking;

    status = calculateWinOrTie(getNowEnemy());

    logList.push_back(log);
    isCheckedLogList.emplace_back(isChecked[0], isChecked[1]);

    logIndex = logList.size();

    return true;
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

Player Chess::getNowPlayer() {
    return (logIndex & 1) ? BLACK : WHITE;
}

Player Chess::getNowEnemy() {
    return (logIndex & 1) ? WHITE : BLACK;
}


bool Chess::isCheck() {
    return isChecking;
}

bool Chess::calculateCheck(Player player) {
    bool result = false;
    for (auto &rowElement: board) {
        for (auto &element: rowElement) {
            if (element.data.player == player) {
                std::vector<Position> validPositions = getValidPos(element.data.position);
                result |= std::any_of(validPositions.begin(), validPositions.end(),
                                          [&](auto &pos) { return pos == kingsPos[element.data.enemy]; });
                if(result) return true;
            }
        }
    }
    return false;
}

void Chess::checkMovement(const Position& target, std::vector<Position> &validPos) {
    for(auto &pos : validPos) {
        Chess sChess = board[target.y][target.x];
        Chess tChess = board[pos.y][pos.x];

        board[pos.y][pos.x] = sChess;
        board[pos.y][pos.x].data.position = pos;
        board[target.y][target.x] = Chess(target);

        if(sChess.data.type == KING) kingsPos[sChess.data.player] = tChess.data.position;

        bool result = calculateCheck(sChess.data.enemy);

        board[pos.y][pos.x] = tChess;
        board[target.y][target.x] = sChess;

        if(sChess.data.type == KING) kingsPos[sChess.data.player] = sChess.data.position;

        if(result) pos = {-1, -1};
    }
}

Status Chess::calculateWinOrTie(Player enemy) {

    for (auto &rowElement: board) {
        for (auto &element: rowElement) {
            if (element.data.player == enemy) {
                std::vector<Position> validPositions = getValidPos(element.data.position);
                checkMovement(element.data.position, validPositions);
                bool haveStep = std::any_of(validPositions.begin(), validPositions.end(),
                                      [](auto &pos) { return (pos.x != -1 || pos.y != -1); });
                if(haveStep) return PLAYING;
            }
        }
    }

    if(isChecking) return WIN;
    return TIE;
}

Status Chess::isWinOrTie() {
    return status;
}