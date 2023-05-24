/*****************************************************************
* File: Datatype.h
 * Author: §d²»ºö¡B´¿±R®¦¡BÃ¹¬ý²M¡B­ð¤_µx
 * Create Date: 2023/05/05
 * Editor: ´¿±R®¦¡BÃ¹¬ý²M
 * Update Date: 2023/05/24
 * Description: the enum about data
 *********************************************************************/

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

enum Status {
    PLAYING,
    WIN,
    TIE
};

struct ChessData {
    Player player = NONE, enemy = NONE;
    Position position;
    Type type = EMPTY;
    bool enPassanting = false;
    bool moved = false;

    ChessData() = default;

    ChessData(Position pos) : position(pos) {}

    ChessData(Player p, Position pos, Type t) :
            player(p),
            enemy(p == WHITE ? BLACK : WHITE),
            position(pos),
            type(t) {}

    //Set type
    void setType(Type t) { 
        type = t;
    }

    //Set data
    void set(ChessData data) { 
        data.position = position;
        *this = data;
    }

    //Clear all the member variable
    void clear() { 
        player = NONE;
        enemy = NONE;
        type = EMPTY;
        enPassanting = false;
        moved = false;
    }

    //Creates a preview of the ChessData object 
    ChessData previewMoved() { 
        ChessData data = *this;
        data.moved = true;
        return data;
    }

    //Creates a preview of the ChessData object
    ChessData previewEnPassanting() {
        ChessData data = *this;
        data.enPassanting = true;
        return data;
    }

    //Creates a preview of the ChessData object
    ChessData previewEnPassanted() {
        ChessData data = *this;
        data.enPassanting = false;
        return data;
    }

    //Creates a preview of the ChessData object ans set type
    ChessData previewSetType(Type t) {
        ChessData data = *this;
        data.type = t;
        return data;
    }

    //Creates a preview of the ChessData object and set the position
    ChessData previewSet(ChessData data) {
        data.position = position;
        return data;
    }

    //Creates a preview of the ChessData object with a new instance of ChessData and set the position
    ChessData previewClear() {
        ChessData data;
        data.position = position;
        return data;
    }
};

typedef std::vector<std::pair<ChessData, ChessData>> Log;