#include "logic/player/AIPlayer.h"
#include <random>
#include <algorithm>

AIPlayer::AIPlayer(std::string name, const StoneColor color, const AIDifficulty difficulty)
    : _name(std::move(name)), _color(color), _difficulty(difficulty) {}

GridPosition AIPlayer::calculateBestMove(const Board &board) const {
    const auto emptyPositions = board.getEmptyPositions();
    if (emptyPositions.empty()) {
        return GridPosition(-1, -1);
    }

    // Always try to win immediately
    const auto winningMove = findWinningMoveFor(_color, board);
    if (winningMove.isValid()) {
        return winningMove;
    }

    // Always block opponent from winning
    const auto blockingMove = findImmediateBlock(board);
    if (blockingMove.isValid()) {
        return blockingMove;
    }

    // Otherwise: Choose strategy based on difficulty
    switch (_difficulty) {
        case RANDOM:
            return playRandomly(emptyPositions);
        case BASIC:
            return playBasic(board, emptyPositions);
        case GOOD:
            return playGood(board, emptyPositions);
        case EXPERT:
        default:
            return playExpert(board, emptyPositions);
    }
}

GridPosition AIPlayer::findImmediateBlock(const Board& board) const {
    const StoneColor opponentColor = (_color == BLACK) ? WHITE : BLACK;
    return findWinningMoveFor(opponentColor, board);
}

GridPosition AIPlayer::findWinningMoveFor(const StoneColor color, const Board& board) {
    const auto emptyPositions = board.getEmptyPositions();

    for (const auto& pos : emptyPositions) {
        // Try placing a stone here
        Board testBoard = board;
        testBoard.placeStone(pos, color);

        if (doesMoveWin(testBoard, pos, color)) {
            return pos;
        }
    }

    return GridPosition(-1, -1);
}

bool AIPlayer::doesMoveWin(const Board& board, const GridPosition& pos, const StoneColor color) {
    for (int i = 0; i < 4; i++) {
        constexpr int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
        const auto line = board.getLineInDirection(pos, color, directions[i][0], directions[i][1]);
        if (line.count >= 5) {
            return true;
        }
    }
    return false;
}

// Picks any random empty spot
GridPosition AIPlayer::playRandomly(const std::vector<GridPosition>& emptyPositions) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, emptyPositions.size() - 1);
    return emptyPositions[dis(gen)];
}

// Try to play near the center of the board or random if invalid
GridPosition AIPlayer::playBasic(const Board& board, const std::vector<GridPosition>& emptyPositions) {

    const GridPosition centerMove = findNearCenterMove(emptyPositions);
    if (centerMove.isValid()) {
        return centerMove;
    }

    return playRandomly(emptyPositions);
}

// Just try to move into the center
GridPosition AIPlayer::findNearCenterMove(const std::vector<GridPosition>& emptyPositions) {
    constexpr int center = Board::SIZE / 2;

    // Look for empty spots near the center
    for (const auto& pos : emptyPositions) {
        const int distanceFromCenter = abs(pos.x - center) + abs(pos.y - center);
        if (distanceFromCenter <= 3) {
            return pos;
        }
    }

    return GridPosition(-1, -1);
}

// Find best move without taking enemy into account
GridPosition AIPlayer::playGood(const Board& board, const std::vector<GridPosition>& emptyPositions) const {
    // Find the best move by scoring all empty positions
    GridPosition bestMove(-1, -1);
    int bestScore = -999;

    for (const auto& pos : emptyPositions) {
        const int score = scorePositionFor(board, pos, _color);
        if (score > bestScore) {
            bestScore = score;
            bestMove = pos;
        }
    }

    return bestMove.isValid() ? bestMove : playRandomly(emptyPositions);
}

// Find best move while trying to block the enemies best move
GridPosition AIPlayer::playExpert(const Board& board, const std::vector<GridPosition>& emptyPositions) const {
    GridPosition bestMove(-1, -1);
    int bestScore = -999;

    for (const auto& pos : emptyPositions) {

        // Score this position for me
        const int myScore = scorePositionFor(board, pos, _color);

        // Score this position for opponent
        // If this scores high, we want to block it, so: Increase the score!
        const StoneColor opponentColor = (_color == BLACK) ? WHITE : BLACK;
        const int opponentScore = scorePositionFor(board, pos, opponentColor);

        // Total score = my benefit + blocking opponent
        const int totalScore = myScore + (opponentScore / 2);

        if (totalScore > bestScore) {
            bestScore = totalScore;
            bestMove = pos;
        }
    }

    return bestMove.isValid() ? bestMove : playRandomly(emptyPositions);
}

// Calculate score when a stone would be placed at pos for color
int AIPlayer::scorePositionFor(const Board& board, const GridPosition& pos, const StoneColor color) {
    Board testBoard = board;
    testBoard.placeStone(pos, color);

    int score = 0;
    constexpr int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    for (const auto direction : directions) {
        const auto line = testBoard.getLineInDirection(pos, color, direction[0], direction[1]);

        // Points = How many stones are in a row?
        if (line.count == 4) score += 100;
        else if (line.count == 3) score += 20;
        else if (line.count == 2) score += 5;
        else score += 1;
    }

    // Àdd points for distance to center
    constexpr int center = Board::SIZE / 2;
    const int distanceFromCenter = abs(pos.x - center) + abs(pos.y - center);
    score += (15 - distanceFromCenter);

    return score;
}