#ifndef GAMESERVICE_H
#define GAMESERVICE_H
#include <string>
#include <vector>

#include "Board.h"
#include "DTO/CommandDTOs.h"
#include "presentation/DTO/ViewModelDTOs.h"

class GameService {
private:
    GameState _state;
    std::vector<Move> _moveHistory;
    std::string _player1Id;
    std::string _player2Id;

    StoneColor checkForWin(const Position& lastMove, StoneColor color) const;
    std::vector<Position> getWinningLine(const Position& lastMove, StoneColor color);

public:
    GameService();
    ~GameService() = default;

    GameService(const GameService&) = default;
    GameService& operator=(const GameService&) = default;
    GameService(GameService&&) = default;
    GameService& operator=(GameService&&) = default;

    // Handle move command from presentation layer
    MoveResultDTO processMove(const PlaceStoneCommandDTO& cmd);

    // Game state queries
    const GameState& getCurrentState() const { return _state; }
    const std::vector<Move>& getMoveHistory() const { return _moveHistory; }

    // Game control methods
    void newGame();
    bool undoLastMove();
    void pauseGame();
    void resumeGame();

    // Player management
    void setPlayerIds(const std::string& player1Id, const std::string& player2Id);
    std::string getCurrentPlayerId() const;
};

#endif // GAMESERVICE_H