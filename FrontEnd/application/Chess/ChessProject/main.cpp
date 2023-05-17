#include "ViewManager.h"

int main(int argc, char** argv) {
    ViewManager game;
    if(argc == 4)
        game.init(argv[1], argv[2], argv[3]);
    else
        game.init();

    return game.inputCommandAndPrint();
}