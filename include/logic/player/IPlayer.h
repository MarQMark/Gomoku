#ifndef IPLAYER_H
#define IPLAYER_H
#include "logic/Board.h"

enum AIDifficulty{
    RANDOM,
    BASIC,
    GOOD,
    EXPERT
};

class IPlayer {
public:
    virtual ~IPlayer() = default;
    virtual std::string getName() const = 0;
    virtual StoneColor getColor() const = 0;
    virtual bool isAIPlayer() const = 0;
};

#endif
