#ifndef COMMANDDTOS_H
#define COMMANDDTOS_H

#include <string>
#include <utility>

#include "logic/player/IPlayer.h"

enum GameMode {
    HUMAN_VS_HUMAN,
    HUMAN_VS_AI,
    AI_VS_HUMAN,
    AI_VS_AI,
};

struct GameSetupCommandDTO {
    GameMode gameMode;
    std::string player1Name;
    std::string player2Name;
    AIDifficulty aiDifficulty;
};

struct MouseCommandDTO {
    GridPosition gridPosition;

    MouseCommandDTO() : gridPosition(GridPosition()) {}
    explicit MouseCommandDTO(const GridPosition gridPosition)
        : gridPosition(gridPosition) {}
};

struct NewGameCommandDTO {
    // TODO: GAME MODE
    // DIFFICULTY
    // BLACK ALWAYS BEGINS

};

#endif