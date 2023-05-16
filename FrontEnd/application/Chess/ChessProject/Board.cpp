#include "Board.h"

Chess Board::board[8][8] = {};

void Board::init() {
    isChecking = false;
    status = PLAYING;
    logIndex = 0;
    beginLogIndex = 0;
    logList.clear();
    kingsPos[0] = {4, 7};
    kingsPos[1] = {4, 0};
    Type typeList[8] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};
    for (int i = 0; i < 8; i++) board[0][i] = Chess(BLACK, {i, 0}, typeList[i]);
    for (int i = 0; i < 8; i++) board[1][i] = Chess(BLACK, {i, 1}, PAWN);
    for (int i = 2; i < 6; i++) for (int j = 0; j < 8; j++) board[i][j] = Chess({j, i});
    for (int i = 0; i < 8; i++) board[6][i] = Chess(WHITE, {i, 6}, PAWN);
    for (int i = 0; i < 8; i++) board[7][i] = Chess(WHITE, {i, 7}, typeList[i]);
}


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

    for(auto &item : boardCode) {
        if(item == '/') {
            rowIndex++;
            colIndex = 0;
        }
        else if(std::isdigit(item)) {
            for(int i = 0; i < item - '0'; i++, colIndex++) {
                board[rowIndex][colIndex] = Chess({colIndex, rowIndex});
            }
        }
        else if(std::isupper(item)) {
            board[rowIndex][colIndex] = Chess(WHITE, {colIndex, rowIndex}, typeMap.at(item));
            if(getChess({colIndex, rowIndex}).data.type == KING) kingsPos[0] = {colIndex, rowIndex};
            colIndex++;
        }
        else if(std::islower(item)) {
            board[rowIndex][colIndex] =
                    Chess(WHITE, {colIndex, rowIndex}, typeMap.at((char)std::toupper(item)));
            if(getChess({colIndex, rowIndex}).data.type == KING) kingsPos[1] = {colIndex, rowIndex};
            colIndex++;
        }
    }

    if(turn == "b") {
        logList.assign(1, {});
        beginLogIndex = 1;
    }

    const Position rookPos[4] = {{0, 0}, {7, 0}, {0, 7}, {7, 7}};
    for(auto &pos : rookPos) getChess(pos).data.moved = true;

    for(auto &item : castling) {
        if(std::toupper(item) == 'K') {
            if(isupper(item)) {
                getChess(rookPos[3]).data.moved = false;
            }
            else {
                getChess(rookPos[1]).data.moved = false;
            }
        }
        else if(std::toupper(item) == 'Q') {
            if(isupper(item)) {
                getChess(rookPos[2]).data.moved = false;
            }
            else {
                getChess(rookPos[0]).data.moved = false;
            }
        }
    }

}

Chess& Board::getChess(Position pos) {
    return board[pos.y][pos.x];
}

std::string Board::getBoard() {
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

std::string Board::getMaskBoard(Position target) {
    std::string output;
    std::vector<Position> posList = getValidPos(target);
    checkMovement(target, posList);
    bool maskBoard[8][8] = {false};
    for (const auto &pos: posList) {
        if (pos.x != -1 && pos.y != -1) {
            maskBoard[pos.y][pos.x] = true;
        }
    }
    for (auto &i: maskBoard) {
        for (bool j: i) output += char('0' + j);
        //output += '\n';
    }
    return output;
}

bool Board::canUndo() {
    return logIndex > beginLogIndex ;
}

bool Board::canRedo() {
    return logIndex < logList.size();
}

bool Board::undo() {
    if (!canUndo()) return false;
    int tempIndex = logIndex - 1;

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

bool Board::redo() {
    if (!canRedo()) return false;

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

std::vector<Position> Board::getValidPos(Position target) {
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

bool Board::canMove(Position source, Position target) {
    std::vector<Position> validPos = getValidPos(source);
    checkMovement(source, validPos);
    return std::any_of(validPos.begin(), validPos.end(),
                       [&](const auto &pos) { return pos == target; });
}

bool Board::move(Player player, Position source, Position target) {
    if (player != getNowPlayer()) return false;

    if (!canMove(source, target)) return false;

    while (logList.size() > logIndex) logList.pop_back();

    std::vector<std::pair<ChessData, ChessData>> log;
    Chess &sChess = board[source.y][source.x];
    Chess &tChess = board[target.y][target.x];

    if (sChess.data.type == KING) {
        kingsPos[sChess.data.player] = tChess.data.position;
        if (tChess.data.position.x == 2 || tChess.data.position.x == 6) {
            Chess &sRook = getChess({(tChess.data.position.x == 2) ? 0 : 7, sChess.data.position.y});
            Chess &tRook = getChess({(tChess.data.position.x == 2) ? 3 : 5, sChess.data.position.y});
            log.emplace_back(sRook.data, sRook.data.previewMoved());
            sRook.data.moved = true;

            log.emplace_back(sRook.data, sRook.data.previewClear());
            log.emplace_back(tRook.data, tRook.data.previewSet(sRook.data));
            tRook.data.set(sRook.data);
            sRook.data.clear();
        }
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
    Chess::isChecking = isChecking;

    status = calculateWinOrTie(getNowEnemy());

    logList.push_back(log);

    logIndex = logList.size();

    return true;
}

Player Board::getNowPlayer() const {
    return (logIndex & 1) ? BLACK : WHITE;
}

Player Board::getNowEnemy() const {
    return (logIndex & 1) ? WHITE : BLACK;
}

bool Board::isCheck() const {
    return isChecking;
}

bool Board::calculateCheck(Player player) {
    bool result = false;
    for (auto &rowElement: board) {
        for (auto &element: rowElement) {
            if (element.data.player == player) {
                std::vector<Position> validPositions = getValidPos(element.data.position);
                result |= std::any_of(validPositions.begin(), validPositions.end(),
                                      [&](auto &pos) { return pos == kingsPos[element.data.enemy]; });
                if (result) return true;
            }
        }
    }
    return false;
}

void Board::checkMovement(const Position &target, std::vector<Position> &validPos) {
    for (auto &pos: validPos) {
        Chess sChess = board[target.y][target.x];
        Chess tChess = board[pos.y][pos.x];

        board[pos.y][pos.x] = sChess;
        board[pos.y][pos.x].data.position = pos;
        board[target.y][target.x] = Chess(target);

        if (sChess.data.type == KING) kingsPos[sChess.data.player] = tChess.data.position;

        bool result = calculateCheck(sChess.data.enemy);

        board[pos.y][pos.x] = tChess;
        board[target.y][target.x] = sChess;

        if (sChess.data.type == KING) kingsPos[sChess.data.player] = sChess.data.position;

        if (result) pos = {-1, -1};
    }
}

Status Board::calculateWinOrTie(Player enemy) {

    for (auto &rowElement: board) {
        for (auto &element: rowElement) {
            if (element.data.player == enemy) {
                std::vector<Position> validPositions = getValidPos(element.data.position);
                checkMovement(element.data.position, validPositions);
                bool haveStep = std::any_of(validPositions.begin(), validPositions.end(),
                                            [](auto &pos) { return (pos.x != -1 || pos.y != -1); });
                if (haveStep) return PLAYING;
            }
        }
    }

    if (isChecking) return WIN;
    return TIE;
}

Status Board::isWinOrTie() {
    return status;
}
