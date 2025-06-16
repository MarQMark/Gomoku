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
    GridPosition find_immediate_block(const Board& board) const;
    static GridPosition find_winning_move_for(StoneColor color, const Board& board);

    static bool does_move_win(const Board& board, const GridPosition& pos, StoneColor color);

    static GridPosition play_randomly(const std::vector<GridPosition>& emptyPositions);
    static GridPosition play_basic(const Board& board, const std::vector<GridPosition>& emptyPositions);
    GridPosition play_good(const Board& board, const std::vector<GridPosition>& emptyPositions) const;
    GridPosition play_expert(const Board& board, const std::vector<GridPosition>& emptyPositions) const;

    static GridPosition find_near_center_move(const std::vector<GridPosition>& emptyPositions);
    static int score_position_for(const Board& board, const GridPosition& pos, StoneColor color);
};


#endif
