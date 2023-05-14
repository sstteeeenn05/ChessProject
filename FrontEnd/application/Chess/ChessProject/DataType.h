#pragma once

#include <utility>
#include <vector>
#include "Position.h"

enum Player {
    WHITE = 0,
    BLACK = 1,
    NONE
};

enum Type {
    KING = 'K',
    QUEEN = 'Q',
    BISHOP = 'B',
    KNIGHT = 'N',
    ROOK = 'R',
    PAWN = 'P',
    EMPTY = '.'
};

struct ChessData {
    Player player = NONE, enemy = NONE;
    Position position;
    Type type = EMPTY;
    bool enPassanted = false;
    bool moved = false;

    ChessData() = default;

    ChessData(Position pos) : position(pos) {}

    ChessData(Player p, Position pos, Type t) :
            player(p),
            enemy(p == WHITE ? BLACK : WHITE),
            position(pos),
            type(t) {}

    void setType(Type t) {
        type = t;
    }

    void set(ChessData data) {
        data.position = position;
        *this = data;
    }

    void clear() {
        player = NONE;
        enemy = NONE;
        type = EMPTY;
        enPassanted = false;
        moved = false;
    }

    ChessData previewMoved() {
        ChessData data = *this;
        data.moved = true;
        return data;
    }

    ChessData previewEnPassant() {
        ChessData data = *this;
        data.enPassanted = true;
        return data;
    }

    ChessData previewSetType(Type t) {
        ChessData data = *this;
        data.type = t;
        return data;
    }

    ChessData previewSet(ChessData data) {
        data.position = position;
        return data;
    }

    ChessData previewClear() {
        ChessData data;
        data.position = position;
        return data;
    }
};