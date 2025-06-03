#ifndef VIEWMODELDTOS_H
#define VIEWMODELDTOS_H
#include <string>
#include <utility>
#include <vector>
#include <ostream>

#include "common/Common.h"

struct GridViewPosition {
    int x, y;
    explicit GridViewPosition(const int x_ = -1, const int y_ = -1) : x(x_), y(y_) {}

    bool operator<(const GridViewPosition& other) const {
        if (y != other.y) return y < other.y;
        return x < other.x;
    }

    friend std::ostream& operator<<(std::ostream& os, const GridViewPosition& pos) {
        os << "(" << pos.x << ", " << pos.y << ")";
        return os;
    }

    GridViewPosition operator-(const GridViewPosition & grid_position) const {
        return GridViewPosition(x - grid_position.x, y - grid_position.y);
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
    MoveViewDTO(const bool cond, std::string  string, BoardViewDTO  board_view_dto, const StoneViewDTO stone_view_dto)
        : success(cond), errorMessage(std::move(string)), boardView(std::move(board_view_dto)), stone(stone_view_dto){}
};

#endif