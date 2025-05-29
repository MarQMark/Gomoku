#ifndef COMMANDDTOS_H
#define COMMANDDTOS_H

#include <string>
#include <utility>

#include "glm/vec2.hpp"

// Command DTOs for Presentation -> Logic communication
struct PlaceStoneCommandDTO {
    int x;
    int y;
    std::string playerId;

    PlaceStoneCommandDTO(const int x_, const int y_, std::string  playerId_)
        : x(x_), y(y_), playerId(std::move(playerId_)) {}
};

struct MouseCommandDTO {
    float relativeBoardX;  // 0.0 to 1.0 within board area
    float relativeBoardY;  // 0.0 to 1.0 within board area
    std::string playerId;

    MouseCommandDTO(const float x, const float y, std::string  playerId_)
        : relativeBoardX(x), relativeBoardY(y), playerId(std::move(playerId_)) {}

    MouseCommandDTO(const glm::vec2 relativeMousePos, std::string  playerId_)
    : relativeBoardX(relativeMousePos.x), relativeBoardY(relativeMousePos.y), playerId(std::move(playerId_)) {}
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