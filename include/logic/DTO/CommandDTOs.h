#ifndef COMMANDDTOS_H
#define COMMANDDTOS_H

#include <string>
#include <utility>

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
    int aiDifficulty;

    explicit GameSetupCommandDTO(const GameMode game_mode)
        : gameMode(game_mode), player1Name("Player 1"), player2Name("Player 2"), aiDifficulty(3) {}
};

struct MouseCommandDTO {
    GridPosition gridPosition;
    explicit MouseCommandDTO(const GridPosition gridPosition)
        : gridPosition(gridPosition) {}
};

struct UndoMoveCommandDTO {
};

struct NewGameCommandDTO {

};

struct SaveGameCommandDTO {
    std::string filename;

    explicit SaveGameCommandDTO(std::string  file)
        : filename(std::move(file)) {}
};

struct LoadGameCommandDTO {
    std::string filename;

    explicit LoadGameCommandDTO(std::string  file)
        : filename(std::move(file)) {}
};

#endif // COMMANDDTOS_H