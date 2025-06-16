#ifndef VIEWMODELDTOS_H
#define VIEWMODELDTOS_H
#include <string>
#include <utility>
#include <vector>
#include <ostream>

#include "common/Common.h"

struct GridViewPosition {
    int x, y;
    explicit GridViewPosition(const int x = -1, const int y = -1) : x(x), y(y) {}

    bool operator<(const GridViewPosition& other) const {
        if (y != other.y) return y < other.y;
        return x < other.x;
    }

    friend std::ostream& operator<<(std::ostream& os, const GridViewPosition& pos) {
        os << "(" << pos.x << ", " << pos.y << ")";
        return os;
    }

    GridViewPosition operator-(const GridViewPosition & gridPosition) const {
        return GridViewPosition(x - gridPosition.x, y - gridPosition.y);
    }
};

enum class ViewColor {
    NONE, BLACK, WHITE
};

struct StoneViewDTO {
    bool isValidPosition;
    GridViewPosition pos;
    ViewColor previewColor;
};

struct GameCompleteViewDTO {
    ViewColor winnerColor;
    GameStatus status;
    std::vector<GridViewPosition> winningLine;
};

struct StatsViewDTO {
    std::string blackPlayer;
    std::string whitePlayer;
    std::string currentPlayer;
    double currentTime;
    int turn;
    ViewColor lastPlayerColor;
    GridViewPosition lastPosition;
    GameStatus gameStatus;
};

struct BoardViewDTO {
    int boardSize;
    std::vector<StoneViewDTO> stones;
    ViewColor currentTurn;
    std::string currentPlayerName;
    GameStatus gameStatus;
    int moveNumber;
    std::vector<GridViewPosition> winningLine;
};

struct MoveViewDTO {
    bool success = false;
    std::string errorMessage;
    BoardViewDTO boardView;
    StoneViewDTO stone;
    std::string currentPlayerName;

    MoveViewDTO() = default;
    MoveViewDTO(const bool cond, std::string  string, BoardViewDTO  boardViewDTO, const StoneViewDTO stoneViewDTO)
        : success(cond), errorMessage(std::move(string)), boardView(std::move(boardViewDTO)), stone(stoneViewDTO){}
};

#endif