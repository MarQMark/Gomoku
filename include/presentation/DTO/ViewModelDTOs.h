#ifndef VIEWMODELDTOS_H
#define VIEWMODELDTOS_H
#include <optional>
#include <string>
#include <vector>

#include "common/Common.h"

struct Position;

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
    std::optional<std::vector<Position>> winningLine;
};

struct MoveResultDTO {
    bool success;
    std::string errorMessage;
    BoardViewDTO boardState;
    std::optional<StoneColor> winner;
};

#endif
