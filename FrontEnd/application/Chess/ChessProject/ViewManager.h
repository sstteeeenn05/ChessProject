#include <iostream>
#include "DataType.h"
#include "Board.h"

#pragma once

class ViewManager {
    Board gameBoard;
    Player player, enemy;
public:
    ViewManager() = default;

    void init();

    void init(std::string boardCode, std::string turn, std::string castling);

    int inputCommandAndPrint();

};
