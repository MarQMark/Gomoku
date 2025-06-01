
#ifndef GAMESTATEENTITY_H
#define GAMESTATEENTITY_H
#include <string>

struct GameStateEntity {
    int moveCount;
    int aiDifficulty;
    double elapsedTime;
    std::string gameType;
};

#endif //GAMESTATEENTITY_H
