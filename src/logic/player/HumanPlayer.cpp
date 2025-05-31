#include "logic/player/HumanPlayer.h"

HumanPlayer::HumanPlayer(std::string name, const StoneColor color)
    : _name(std::move(name)), _color(color) {}
