#ifndef COMMANDDTOS_H
#define COMMANDDTOS_H

#include <string>

// Command DTOs for Presentation -> Logic communication
struct PlaceStoneCommandDTO {
    int x;
    int y;
    std::string playerId;

    PlaceStoneCommandDTO() : x(-1), y(-1), playerId("") {}
    PlaceStoneCommandDTO(int x_, int y_, const std::string& playerId_)
        : x(x_), y(y_), playerId(playerId_) {}
};

struct UndoMoveCommandDTO {
    std::string playerId;

    UndoMoveCommandDTO() : playerId("") {}
    UndoMoveCommandDTO(const std::string& playerId_) : playerId(playerId_) {}
};

struct NewGameCommandDTO {
    std::string player1Id;
    std::string player2Id;

    NewGameCommandDTO() : player1Id(""), player2Id("") {}
    NewGameCommandDTO(const std::string& p1, const std::string& p2)
        : player1Id(p1), player2Id(p2) {}
};

struct SaveGameCommandDTO {
    std::string filename;
    std::string playerId;

    SaveGameCommandDTO() : filename(""), playerId("") {}
    SaveGameCommandDTO(const std::string& file, const std::string& player)
        : filename(file), playerId(player) {}
};

struct LoadGameCommandDTO {
    std::string filename;
    std::string playerId;

    LoadGameCommandDTO() : filename(""), playerId("") {}
    LoadGameCommandDTO(const std::string& file, const std::string& player)
        : filename(file), playerId(player) {}
};

#endif // COMMANDDTOS_H