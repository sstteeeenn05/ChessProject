/*****************************************************************
* File: main.cpp
 * Author: �d�����B���R���Bù���M�B��_�x
 * Create Date: 2023/05/05
 * Editor: ���R���Bù���M
 * Update Date: 2023/05/24
 * Description: execute the program
 *********************************************************************/

#include "Game.h"

int main(int argc, char** argv) {
    Game game;

    if(argc == 4)
        game.init(argv[1], argv[2], argv[3]);
    else
        game.init();

    return game.inputCommandAndPrint();
}