#include "logic/player/AIPlayer.h"

AIPlayer::AIPlayer(std::string name, const StoneColor color, const int difficulty)
    : _name(std::move(name)), _color(color), _difficulty(difficulty) {}

GridPosition AIPlayer::calculateBestMove(const Board &board) const {
    return minimax(board, _difficulty);
}

GridPosition AIPlayer::minimax(const Board &board, int depth) {
    return GridPosition(-1, -1);
}
