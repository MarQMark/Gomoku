#include "logic/Board.h"

Board::Board() : _stoneCount(0) {
    initialize_empty();
}

void Board::initialize_empty() {
    for (auto& row : _grid) {
        row.fill(StoneColor::STONE_NONE);
    }
    _stoneCount = 0;
}

bool Board::placeStone(const Position& pos, StoneColor color) {
    if (!isValidPosition(pos) || color == StoneColor::STONE_NONE) {
        return false;
    }

    // Check if position is already occupied
    if (_grid[pos.y][pos.x] != StoneColor::STONE_NONE) {
        return false;
    }

    // Place the stone
    _grid[pos.y][pos.x] = color;
    _stoneCount++;

    return true;
}

bool Board::removeStone(const Position& pos) {
    if (!isValidPosition(pos)) {
        return false;
    }

    // Check if a stone to remove
    if (_grid[pos.y][pos.x] == StoneColor::STONE_NONE) {
        return false;
    }

    // Remove the stone
    _grid[pos.y][pos.x] = StoneColor::STONE_NONE;
    _stoneCount--;

    return true;
}

void Board::clear() {
    initialize_empty();
}

StoneColor Board::getColor(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return StoneColor::STONE_NONE;
    }
    return _grid[pos.y][pos.x];
}

bool Board::isEmpty() const {
    return _stoneCount == 0;
}

bool Board::isFull() const {
    return _stoneCount >= SIZE * SIZE;
}

int Board::getStoneCount() const {
    return _stoneCount;
}

bool Board::isValidPosition(const Position& pos) const {
    return pos.x >= 0 && pos.x < SIZE && pos.y >= 0 && pos.y < SIZE;
}

std::vector<Position> Board::getOccupiedPositions() const {
    std::vector<Position> positions;
    positions.reserve(_stoneCount);

    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            if (_grid[y][x] != StoneColor::STONE_NONE) {
                positions.emplace_back(x, y);
            }
        }
    }

    return positions;
}

std::vector<Position> Board::getEmptyPositions() const {
    std::vector<Position> positions;
    positions.reserve(SIZE * SIZE - _stoneCount);

    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            if (_grid[y][x] == StoneColor::STONE_NONE) {
                positions.emplace_back(x, y);
            }
        }
    }

    return positions;
}

std::vector<Position> Board::getAdjacentPositions(const Position& pos) const {
    std::vector<Position> adjacent;
    adjacent.reserve(8);

    // All 8 directions: up, down, left, right, and 4 diagonals
    static const int dx[] = {-1, -1, -1,  0,  0,  1,  1,  1};
    static const int dy[] = {-1,  0,  1, -1,  1, -1,  0,  1};

    for (int i = 0; i < 8; ++i) {
        Position adj(pos.x + dx[i], pos.y + dy[i]);
        if (isValidPosition(adj)) {
            adjacent.push_back(adj);
        }
    }

    return adjacent;
}

bool Board::operator==(const Board& other) const {
    // Quick check first
    if (_stoneCount != other._stoneCount) {
        return false;
    }

    // Compare grids
    return _grid == other._grid;
}