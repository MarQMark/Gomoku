#include "logic/Board.h"

Board::Board() : _stoneCount(0) {
    initialize_empty();
}

void Board::initialize_empty() {
    for (auto& row : _grid) {
        row.fill(STONE_NONE);
    }
    _stoneCount = 0;
}

bool Board::placeStone(const GridPosition& pos, const StoneColor color) {
    if (!isValidPosition(pos) || color == STONE_NONE) {
        return false;
    }

    // Check if position is already occupied
    if (_grid[pos.y][pos.x] != STONE_NONE) {
        return false;
    }

    // Place the stone
    _grid[pos.y][pos.x] = color;
    _stoneCount++;

    return true;
}

bool Board::removeStone(const GridPosition& pos) {
    if (!isValidPosition(pos)) {
        return false;
    }

    // Check if a stone to remove
    if (_grid[pos.y][pos.x] == STONE_NONE) {
        return false;
    }

    // Remove the stone
    _grid[pos.y][pos.x] = STONE_NONE;
    _stoneCount--;

    return true;
}

void Board::clear() {
    initialize_empty();
}

StoneColor Board::getColor(const GridPosition& pos) const {
    if (!isValidPosition(pos)) {
        return STONE_NONE;
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

bool Board::isValidPosition(const GridPosition& pos) {
    return pos.x >= 0 && pos.x < SIZE && pos.y >= 0 && pos.y < SIZE;
}

std::vector<GridPosition> Board::getOccupiedPositions() const {
    std::vector<GridPosition> positions;
    positions.reserve(_stoneCount);

    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            if (_grid[y][x] != STONE_NONE) {
                positions.emplace_back(x, y);
            }
        }
    }

    return positions;
}

std::vector<GridPosition> Board::getEmptyPositions() const {
    std::vector<GridPosition> positions;
    positions.reserve(SIZE * SIZE - _stoneCount);

    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            if (_grid[y][x] == STONE_NONE) {
                positions.emplace_back(x, y);
            }
        }
    }

    return positions;
}

std::vector<GridPosition> Board::getAdjacentPositions(const GridPosition& pos) {
    std::vector<GridPosition> adjacent;
    adjacent.reserve(8);

    static constexpr int dx[] = {-1, -1, -1,  0,  0,  1,  1,  1};
    static constexpr int dy[] = {-1,  0,  1, -1,  1, -1,  0,  1};

    for (int i = 0; i < 8; ++i) {
        GridPosition adj(pos.x + dx[i], pos.y + dy[i]);
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