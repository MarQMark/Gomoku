#include "logic/player/AIPlayer.h"
#include <random>
#include <algorithm>

AIPlayer::AIPlayer(std::string name, const StoneColor color, const int difficulty)
    : _name(std::move(name)), _color(color), _difficulty(difficulty) {}

GridPosition AIPlayer::calculateBestMove(const Board &board) const {
    const auto emptyPositions = board.getEmptyPositions();
    if (emptyPositions.empty()) {
        return GridPosition(-1, -1);
    }

    const auto winMove = findWinningMove(board, _color);
    if (winMove.isValid()) {
        return winMove;
    }

    // Always block opponent wins no random!
    const StoneColor opponentColor = (_color == BLACK) ? WHITE : BLACK;
    const auto blockMove = findWinningMove(board, opponentColor);
    if (blockMove.isValid()) {
        return blockMove;
    }

    if (_difficulty <= 2) {
        return getRandomMove(emptyPositions);
    } else {
        return getBestMoveWithRandomness(board, emptyPositions);
    }
}

GridPosition AIPlayer::getBestMoveWithRandomness(const Board& board, const std::vector<GridPosition>& emptyPositions) const {
    std::vector<std::pair<GridPosition, int>> scoredMoves;

    for (const auto& pos : emptyPositions) {
        int score = evaluatePosition(board, pos);
        scoredMoves.emplace_back(pos, score);
    }

    std::sort(scoredMoves.begin(), scoredMoves.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    const int bestScore = scoredMoves[0].second;
    const int tolerance = std::max(5, bestScore / 10);

    std::vector<GridPosition> goodMoves;
    for (const auto& [pos, score] : scoredMoves) {
        if (score >= bestScore - tolerance) {
            goodMoves.push_back(pos);
        }
    }

    return getRandomMove(goodMoves);
}

GridPosition AIPlayer::findWinningMove(const Board& board, StoneColor color) {
    const auto emptyPositions = board.getEmptyPositions();

    for (const auto& pos : emptyPositions) {
        Board testBoard = board;
        testBoard.placeStone(pos, color);

        auto line = testBoard.getLineInDirection(pos, color, 1, 0);  // Horizontal
        if (line.count >= 5) return pos;

        line = testBoard.getLineInDirection(pos, color, 0, 1);  // Vertical
        if (line.count >= 5) return pos;

        line = testBoard.getLineInDirection(pos, color, 1, 1);  // Diagonal
        if (line.count >= 5) return pos;

        line = testBoard.getLineInDirection(pos, color, 1, -1); // Diagonal
        if (line.count >= 5) return pos;
    }

    return GridPosition(-1, -1);
}

GridPosition AIPlayer::getRandomMove(const std::vector<GridPosition>& emptyPositions) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(emptyPositions.size()) - 1);
    return emptyPositions[dis(gen)];
}

GridPosition AIPlayer::getBestMove(const Board& board, const std::vector<GridPosition>& emptyPositions) const {
    GridPosition bestMove(-1, -1);
    int bestScore = -1000;

    for (const auto& pos : emptyPositions) {
        const int score = evaluatePosition(board, pos);
        if (score > bestScore) {
            bestScore = score;
            bestMove = pos;
        }
    }

    return bestMove.isValid() ? bestMove : getRandomMove(emptyPositions);
}

int AIPlayer::evaluatePosition(const Board& board, const GridPosition& pos) const {
    int score = 0;

    Board testBoard = board;
    testBoard.placeStone(pos, _color);

    constexpr int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (const auto direction : directions) {
        const auto line = testBoard.getLineInDirection(pos, _color, direction[0], direction[1]);

        if (line.count >= 4) score += 100;
        else if (line.count == 3) score += 10;
        else if (line.count == 2) score += 3;
        else score += 1;
    }

    constexpr int center = Board::SIZE / 2;
    const int distanceFromCenter = std::abs(pos.x - center) + std::abs(pos.y - center);
    score += (Board::SIZE - distanceFromCenter);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomBonus(-2, 2);
    score += randomBonus(gen);

    return score;
}