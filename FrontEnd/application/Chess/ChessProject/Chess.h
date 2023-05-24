/*****************************************************************
* File: Chess.h
 * Author: §d²»ºö¡B´¿±R®¦¡BÃ¹¬ý²M¡B­ð¤_µx
 * Create Date: 2023/05/05
 * Editor: ´¿±R®¦¡BÃ¹¬ý²M
 * Update Date: 2023/05/24
 * Description: the declaration of Chess
 *********************************************************************/

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

    bool checkValid(Position, std::vector<Position> &) const; //Check if the given position is a valid move for the chess piece

    void checkCastling(std::vector<Position> &); //Check if castling is a valid move for the chess piece

    void checkStraight(std::vector<Position> &); //Find all the valid positions on the straight line

    void checkCross(std::vector<Position> &) const; //Find all the valid positions on the diagonal lines

    void checkSquare(std::vector<Position> &) const; //Find all the valid positions adjacent to the chess piece

    void checkL(std::vector<Position> &) const; //Find all the valid positions for a knight move

    void checkPawn(std::vector<Position> &) const; //Find all the valid positions for a pawn move

    [[nodiscard]] Position generatePosByPlayer(Position) const; //Generate a new position based on the current position and the player's perspective

    [[nodiscard]] bool onRiver() const; //Check if the chess piece is on the river

    [[nodiscard]] bool onMyRiver() const; //Check if the chess piece is positioned on its own river

    [[nodiscard]] std::pair<Position, Position> getSidePos() const; //Get the positions adjacent to the current position on the same row

    [[nodiscard]] std::pair<bool, bool> getEnPassant() const; //Check if en passant move is possible for the current chess piece

    [[nodiscard]] bool checkPromotion() const; //Check if the pawn is at the end of the board

    [[nodiscard]] Type doPromotion() const; //Execute promotion

    Chess() = default;

    explicit Chess(Position);

    Chess(Player, Position, Type);

    friend class Board;
};