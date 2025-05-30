#ifndef COMMANDDTOS_H
#define COMMANDDTOS_H

#include <string>
#include <utility>

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