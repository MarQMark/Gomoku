//
// Created by Henk on 31.05.2025.
//

#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H
#include "IPlayer.h"


class HumanPlayer final : public IPlayer {
    std::string _name;
    StoneColor _color;

public:
    HumanPlayer(std::string name, StoneColor color);
    std::string getName() const override { return _name; }
    StoneColor getColor() const override { return _color; }
    bool isAIPlayer() const override { return false; }
};



#endif
