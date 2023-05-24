/*****************************************************************
* File: Game.h
 * Author: �d�����B���R���Bù���M�B��_�x
 * Create Date: 2023/05/05
 * Editor: ���R���Bù���M
 * Update Date: 2023/05/24
 * Description: the declaration of Game
 *********************************************************************/

#include <iostream>
#include "DataType.h"
#include "Board.h"

#pragma once

class Game {
    Board gameBoard;
    Player player, enemy;
public:
    Game() = default;

    void init(); //Initialize the game with default settings

    void init(std::string boardCode, std::string turn, std::string castling); //Initialize the game with custom settings

    int inputCommandAndPrint(); // Handle user input and print game information

};
