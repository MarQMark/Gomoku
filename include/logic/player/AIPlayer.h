//
// Created by Henk on 31.05.2025.
//

#ifndef AIPLAYER_H
#define AIPLAYER_H
#include "IPlayer.h"

class AIPlayer final : public IPlayer {
    std::string _name;
    StoneColor _color;
    AIDifficulty _difficulty;

public:
    AIPlayer(std::string name, StoneColor color, AIDifficulty difficulty);

    std::string getName() const override { return _name; }
    StoneColor getColor() const override { return _color; }
    bool isAIPlayer() const override { return true; }

    GridPosition calculateBestMove(const Board& board) const;
    AIDifficulty getDifficulty() const;

private:
    GridPosition findImmediateBlock(const Board& board) const;
    static GridPosition findWinningMoveFor(StoneColor color, const Board& board);

    static bool doesMoveWin(const Board& board, const GridPosition& pos, StoneColor color);

    static GridPosition playRandomly(const std::vector<GridPosition>& emptyPositions);
    static GridPosition playBasic(const Board& board, const std::vector<GridPosition>& emptyPositions);
    GridPosition playGood(const Board& board, const std::vector<GridPosition>& emptyPositions) const;
    GridPosition playExpert(const Board& board, const std::vector<GridPosition>& emptyPositions) const;

    static GridPosition findNearCenterMove(const std::vector<GridPosition>& emptyPositions);
    static int scorePositionFor(const Board& board, const GridPosition& pos, StoneColor color);
};


#endif
