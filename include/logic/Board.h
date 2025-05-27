#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <vector>
#include "common/Common.h"

struct Position {
    int x, y;

    Position() : x(-1), y(-1) {}
    Position(const int x_, const int y_) : x(x_), y(y_) {}

    bool isValid() const {
        return x >= 0 && x < 15 && y >= 0 && y < 15;
    }

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

    // Useful for using Position as a key in maps
    bool operator<(const Position& other) const {
        if (y != other.y) return y < other.y;
        return x < other.x;
    }
};

struct Move {
    Position position;
    StoneColor color;

    Move(const Position pos, const StoneColor color) : position(pos), color(color) {}

    bool isValid() const {
        return position.isValid() && color != StoneColor::STONE_NONE;
    }

    bool operator==(const Move& other) const {
        return position == other.position && color == other.color;
    }
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
    bool placeStone(const Position& pos, StoneColor color);
    bool removeStone(const Position& pos);
    void clear();

    // Board queries
    StoneColor getColor(const Position& pos) const;
    bool isEmpty() const;
    bool isFull() const;
    int getStoneCount() const;

    // Position validation and queries
    bool isValidPosition(const Position& pos) const;
    std::vector<Position> getOccupiedPositions() const;
    std::vector<Position> getEmptyPositions() const;
    std::vector<Position> getAdjacentPositions(const Position& pos) const;

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

struct GameState {
    Board board;
    StoneColor currentPlayerTurn;
    int moveNumber;
    GameStatus status;
    Position lastMove;

    GameState() 
        : currentPlayerTurn(StoneColor::BLACK)
        , moveNumber(0)
        , status(GameStatus::NOT_STARTED)
        , lastMove(-1, -1) {}
};

#endif //BOARD_H