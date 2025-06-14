#include "Game.h"

int main() {
    Game game;

    while(game.shouldRun()) {
        game.update();
    }

    return 0;
}