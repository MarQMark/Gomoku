#ifndef COMMANDDTOS_H
#define COMMANDDTOS_H

#include <string>
#include <utility>

struct MouseCommandDTO {
    GridPosition gridPosition;
    std::string playerId;

    MouseCommandDTO(const GridPosition gridPosition, std::string  playerId_)
    : gridPosition(gridPosition), playerId(std::move(playerId_)) {}
};

struct UndoMoveCommandDTO {
    std::string playerId;

    explicit UndoMoveCommandDTO(std::string  playerId_) : playerId(std::move(playerId_)) {}
};

struct NewGameCommandDTO {
    std::string player1Id;
    std::string player2Id;

    NewGameCommandDTO(std::string  p1, std::string  p2)
        : player1Id(std::move(p1)), player2Id(std::move(p2)) {}
};

struct SaveGameCommandDTO {
    std::string filename;
    std::string playerId;

    SaveGameCommandDTO(std::string  file, std::string  player)
        : filename(std::move(file)), playerId(std::move(player)) {}
};

struct LoadGameCommandDTO {
    std::string filename;
    std::string playerId;

    LoadGameCommandDTO(std::string  file, std::string  player)
        : filename(std::move(file)), playerId(std::move(player)) {}
};

#endif // COMMANDDTOS_H