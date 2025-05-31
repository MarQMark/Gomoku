#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <ostream>
#include <vector>

#include "common/Common.h"

class IPlayer;
struct Move;
struct LineInfo;
struct GridPosition;

enum StoneColor {
    STONE_NONE,
    BLACK,
    WHITE,
};

class Board {
public:
    static constexpr int SIZE = 15;

    Board();
    ~Board() = default;
    Board(const Board&) = default;
    Board& operator=(const Board&) = default;
    Board(Board&&) = default;
    Board& operator=(Board&&) = default;

    // Core board operations
    bool placeStone(const GridPosition& pos, StoneColor color);
    bool removeStone(const GridPosition& pos);
    void clear();

    // Board queries
    StoneColor getColor(const GridPosition& pos) const;
    bool isEmpty() const;
    bool isFull() const;
    int getStoneCount() const;

    // Position validation and queries
    static bool isValidPosition(const GridPosition& pos);
    std::vector<Move> getOccupiedPositions() const;
    std::vector<GridPosition> getEmptyPositions() const;

    static std::vector<GridPosition> getAdjacentPositions(const GridPosition& pos);
    LineInfo getLineInDirection(const GridPosition& pos, StoneColor color, int dx, int dy) const;

    // Comparison operators
    bool operator==(const Board& other) const;
    bool operator!=(const Board& other) const { return !(*this == other); }

    // Create a copy (explicit method for clarity)
    Board copy() const { return *this; }

private:
    std::array<std::array<StoneColor, SIZE>, SIZE> _grid{};
    int _stoneCount;

    void initialize_empty();
};

struct GridPosition {
    int x, y;

    GridPosition() : x(-1), y(-1) {}
    GridPosition(const int x_, const int y_) : x(x_), y(y_) {}

    bool isValid() const {
        return x >= 0 && x < Board::SIZE && y >= 0 && y < Board::SIZE;
    }

    bool operator==(const GridPosition& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const GridPosition& other) const {
        return !(*this == other);
    }

    bool operator<(const GridPosition& other) const {
        if (y != other.y) return y < other.y;
        return x < other.x;
    }

    friend std::ostream& operator<<(std::ostream& os, const GridPosition& pos) {
        os << "(" << pos.x << ", " << pos.y << ")";
        return os;
    }
};

struct LineInfo {
    std::vector<GridPosition> positions;
    int count;
};

struct Move {
    GridPosition position;
    StoneColor color;

    Move(const GridPosition pos, const StoneColor color) : position(pos), color(color) {}

    bool isValid() const {
        return position.isValid() && color != StoneColor::STONE_NONE;
    }

    bool operator==(const Move& other) const {
        return position == other.position && color == other.color;
    }
};

struct BoardState {
    Board board;
    IPlayer* currentPlayer;
    int moveNumber;
    GameStatus status;
    GridPosition latestMove;
};




#endif