/*****************************************************************
* File: Game.h
 * Author: §d²»ºö¡B´¿±R®¦¡BÃ¹¬ý²M¡B­ð¤_µx
 * Create Date: 2023/05/05
 * Editor: ´¿±R®¦¡BÃ¹¬ý²M
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
