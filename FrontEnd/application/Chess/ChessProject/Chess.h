#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include "DataType.h"
#include "Board.h"

class Chess {
    ChessData data;

    static bool isChecking;

public:

    bool checkValid(Position, std::vector<Position> &) const;

    void checkCastling(std::vector<Position> &);

    void checkStraight(std::vector<Position> &);

    void checkCross(std::vector<Position> &) const;

    void checkSquare(std::vector<Position> &) const;

    void checkL(std::vector<Position> &) const;

    void checkPawn(std::vector<Position> &) const;

    [[nodiscard]] Position generatePosByPlayer(Position) const;

    [[nodiscard]] bool onRiver() const;

    [[nodiscard]] std::pair<Position, Position> getSidePos() const;

    [[nodiscard]] std::pair<bool, bool> getEnPassant() const;

    [[nodiscard]] bool checkPromotion() const;

    [[nodiscard]] Type doPromotion() const;

    Chess() = default;

    explicit Chess(Position);

    Chess(Player, Position, Type);

    friend class Board;
};