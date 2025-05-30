#ifndef VIEWMODELDTOS_H
#define VIEWMODELDTOS_H
#include <string>
#include <vector>
#include <ostream>

#include "common/Common.h"

struct ViewPosition {
    int x, y;
    explicit ViewPosition(const int x_ = -1, const int y_ = -1) : x(x_), y(y_) {}

    friend std::ostream& operator<<(std::ostream& os, const ViewPosition& pos) {
        os << "(" << pos.x << ", " << pos.y << ")";
        return os;
    }
};

enum class ViewColor {
    NONE, BLACK, WHITE
};

struct StoneViewDTO {
    bool isValidPosition;
    ViewPosition pos;
    ViewColor previewColor;
};

struct BoardViewDTO {
    int boardSize;
    std::vector<StoneViewDTO> stones;
    ViewColor currentTurn;
    std::string currentPlayerName;
    GameStatus gameStatus;
    int moveNumber;
    std::vector<ViewPosition> winningLine;
};

struct MoveViewDTO {
    bool success;
    std::string errorMessage;
    BoardViewDTO boardView;
    StoneViewDTO stone;
};

#endif