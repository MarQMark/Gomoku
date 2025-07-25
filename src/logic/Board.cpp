#include "logic/Board.h"

Board::Board() : _stone_count(0) {
    initialize_empty();
}

void Board::initialize_empty() {
    for (auto& row : _grid) {
        row.fill(STONE_NONE);
    }
    _stone_count = 0;
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
    _stone_count++;

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
    _stone_count--;

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
    return _stone_count == 0;
}

bool Board::isFull() const {
    return _stone_count >= SIZE * SIZE;
}

int Board::getStoneCount() const {
    return _stone_count;
}

bool Board::isValidPosition(const GridPosition& pos) {
    return pos.isValid();
}

std::vector<Move> Board::getOccupiedPositions() const {
    std::vector<Move> moves;
    moves.reserve(_stone_count);

    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            if (_grid[y][x] != STONE_NONE) {
                moves.emplace_back(Move(GridPosition(x, y), _grid[y][x]));
            }
        }
    }

    return moves;
}

std::vector<GridPosition> Board::getEmptyPositions() const {
    std::vector<GridPosition> positions;
    positions.reserve(SIZE * SIZE - _stone_count);

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

LineInfo Board::getLineInDirection(const GridPosition& pos, const StoneColor color, const int dx, const int dy) const {
    LineInfo info;
    info.positions.push_back(pos);
    info.count = 1;

    // Check positive direction
    GridPosition check(pos.x + dx, pos.y + dy);
    while (check.isValid() && getColor(check) == color) {
        info.positions.push_back(check);
        info.count++;
        check.x += dx;
        check.y += dy;
    }

    // Check negative direction
    check = GridPosition(pos.x - dx, pos.y - dy);
    while (check.isValid() && getColor(check) == color) {
        info.positions.insert(info.positions.begin(), check);
        info.count++;
        check.x -= dx;
        check.y -= dy;
    }

    return info;
}


bool Board::operator==(const Board& other) const {
    // Quick check first
    if (_stone_count != other._stone_count) {
        return false;
    }

    // Compare grids
    return _grid == other._grid;
}
