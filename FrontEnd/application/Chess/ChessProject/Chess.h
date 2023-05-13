#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include "DataType.h"

class Chess {
    static std::vector<std::vector<std::pair<ChessData, ChessData>>> logList;
    static int logIndex;
    static Chess board[8][8];

    // 0-white, 1-black
    static Position kingsPos[2];
    // 0-white, 1-black
    static bool isChecked[2];
    // 0-white, 1-black
    static bool isChecking;
    static std::vector<std::vector<bool>> checkmateRoute;

    ChessData data;
public:
    static void init();

    static std::string getBoard();

    static std::string getMaskBoard(Position);

    static Chess &getChess(Position);

    static bool canUndo();

    static bool canRedo();

    static bool undo();

    static bool redo();

    static std::vector<Position> getValidPos(Position);

    bool checkValid(Position, std::vector<Position> &) const;

    void checkStraight(std::vector<Position> &);

    void checkCross(std::vector<Position> &);

    void checkSquare(std::vector<Position> &);

    void checkL(std::vector<Position> &);

    void checkPawn(std::vector<Position> &);

    Position generatePosByPlayer(Position);

    [[nodiscard]] bool onRiver() const;

    std::pair<Position, Position> getSidePos();

    std::pair<bool, bool> getEnPassant();

    static bool canMove(Position, Position);

    static bool move(Player, Position, Position);

    bool checkPromotion() const;

    Type doPromotion() const;

    //static bool checkMovement(Player);
    static void calculateCheck();
    static bool isCheck();
    //static bool checkWin(Player);

    static Player getNowPlayer();

    static Player getNowEnemy();

    Chess() = default;

    Chess(Position);

    Chess(Player, Position, Type);
};