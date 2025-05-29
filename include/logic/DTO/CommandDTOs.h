#ifndef COMMANDDTOS_H
#define COMMANDDTOS_H

#include <string>
#include <utility>

// Command DTOs for Presentation -> Logic communication
struct PlaceStoneCommandDTO {
    int x;
    int y;
    std::string playerId;

    PlaceStoneCommandDTO(const int x_, const int y_, std::string  playerId_)
        : x(x_), y(y_), playerId(std::move(playerId_)) {}
};

struct MouseHoverCommandDTO {
    float relativeX;  // 0.0 to 1.0 within board area
    float relativeY;  // 0.0 to 1.0 within board area
    std::string playerId;

    MouseHoverCommandDTO(const float x, const float y, std::string  playerId_)
        : relativeX(x), relativeY(y), playerId(std::move(playerId_)) {}
};

struct MouseClickCommandDTO {
    float relativeX;  // 0.0 to 1.0 within board area
    float relativeY;  // 0.0 to 1.0 within board area
    std::string playerId;

    MouseClickCommandDTO(const float x, const float y, std::string  playerId_)
        : relativeX(x), relativeY(y), playerId(std::move(playerId_)) {}
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