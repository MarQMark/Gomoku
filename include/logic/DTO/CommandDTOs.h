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

    explicit GameSetupCommandDTO(const GameMode game_mode)
        : gameMode(game_mode), player1Name("Player 1"), player2Name("Player 2"), aiDifficulty(EXPERT) {}
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