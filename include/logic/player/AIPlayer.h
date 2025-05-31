//
// Created by Henk on 31.05.2025.
//

#ifndef AIPLAYER_H
#define AIPLAYER_H
#include "IPlayer.h"


class AIPlayer final : public IPlayer {
    std::string _name;
    StoneColor _color;
    int _difficulty;

public:
    AIPlayer(std::string name, StoneColor color, int difficulty);

    std::string getName() const override { return _name; }
    StoneColor getColor() const override { return _color; }
    bool isAIPlayer() const override { return true; }

    GridPosition calculateBestMove(const Board& board) const;

private:
    static GridPosition minimax(const Board& board, int depth) ;
};


#endif
