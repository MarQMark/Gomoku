#ifndef VIEWMODELDTOS_H
#define VIEWMODELDTOS_H
#include <optional>
#include <string>
#include <vector>

#include "common/Common.h"

struct GridPosition;

struct StoneViewDTO {
    int x, y;
    StoneColor color;
    bool isLastMove;
    bool isWinningStone;
};

struct BoardViewDTO {
    int boardSize = 15;
    std::vector<StoneViewDTO> stones;
    StoneColor currentTurn;
    std::string currentPlayerName;
    GameStatus gameStatus;
    int moveNumber;
    std::optional<std::vector<GridPosition>> winningLine;
};

struct MoveResultDTO {
    bool success;
    std::string errorMessage;
    BoardViewDTO boardState;
    std::optional<StoneColor> winner;
};

struct GridHoverResultDTO {
    bool isValidPosition;
    GridPosition gridPosition;
    StoneColor previewColor;

    GridHoverResultDTO() : isValidPosition(false), gridPosition(-1, -1), previewColor(StoneColor::STONE_NONE) {}
    GridHoverResultDTO(const bool valid, const GridPosition pos, const StoneColor color)
        : isValidPosition(valid), gridPosition(pos), previewColor(color) {}
};

#endif