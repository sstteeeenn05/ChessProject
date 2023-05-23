#pragma once

#include "DataType.h"
#include "Chess.h"
#include <cctype>
#include <map>

class Chess;

class Board {
    std::vector<Log> logList;
    int logIndex;
    int beginLogIndex;
    static Chess board[8][8];
    // 0-white, 1-black
    Position kingsPos[2];
    bool isChecking;

    Status status;
public:
    void init();

    void init(std::string boardCode, std::string turn, std::string castling);

    static Chess &getChess(Position);

    static std::string getBoard();

    std::string getMaskBoard(Position);

    bool canUndo();

    bool canRedo();

    bool undo();

    bool redo();

    std::vector<Position> getValidPos(Position);

    bool canMove(Position, Position);

    bool move(Player, Position, Position);

    void checkCastling(Chess&, Chess&, Log&);

    void checkEnPassant(Chess&, Chess&, Log&);

    void clearEnPassanting(Log&);

    void checkNextEnPassant(Chess&, Log&);

    void checkMovement(const Position &target, std::vector<Position> &validPos);

    bool isCheck() const;

    Status isWinOrTie();

    Player getNowPlayer() const;

    Player getNowEnemy() const;

    [[nodiscard]] bool calculateCheck(Player);

    [[nodiscard]] Status calculateWinOrTie(Player);

    friend class Chess;
};
