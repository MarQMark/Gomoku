#ifndef VIEWMODELDTOS_H
#define VIEWMODELDTOS_H
#include <optional>
#include <string>
#include <vector>

#include "common/Common.h"

struct MouseHoverViewDTO {
    bool isValidPosition;
    GridPosition stonePosition;
    StoneColor previewColor;
};

struct StoneViewDTO {
    bool isValidPosition;
    GridPosition pos;
    StoneColor previewColor;

    StoneViewDTO() : isValidPosition(false), pos(-1, -1),
                     previewColor(STONE_NONE) {
    }

    StoneViewDTO(const bool valid, const GridPosition pos, const StoneColor color)
        : isValidPosition(valid), pos(pos), previewColor(color) {
    }
};
struct BoardViewDTO {
    int boardSize;
    std::vector<StoneViewDTO> stones;
    StoneColor currentTurn;
    std::string currentPlayerName;
    GameStatus gameStatus;
    int moveNumber;
    std::vector<GridPosition> winningLine;
};

struct MoveViewDTO {
    bool success;
    std::string errorMessage;
    BoardViewDTO boardView;
    StoneViewDTO stone;
};

#endif