#ifndef GAMESERVICE_H
#define GAMESERVICE_H
#include <string>
#include <vector>

#include "Board.h"
#include "DTO/CommandDTOs.h"
#include "glm/vec2.hpp"
#include "presentation/DTO/ViewModelDTOs.h"

class GameService {
private:
    GameState _state;
    std::vector<Move> _moveHistory;
    std::string _player1Id;
    std::string _player2Id;

    StoneColor checkForWin(const GridPosition& lastMove, StoneColor color) const;

    static bool isPlayerValid(const std::string &playerId);

public:
    GameService();
    ~GameService() = default;

    GameService(const GameService&) = default;
    GameService& operator=(const GameService&) = default;
    GameService(GameService&&) = default;
    GameService& operator=(GameService&&) = default;

    // Handle move command from presentation layer
    MoveViewDTO processMove(const MouseCommandDTO& cmd);
    MouseHoverViewDTO processMouseHover(const MouseCommandDTO& hover_command_dto) const;
    MoveViewDTO processMouseClick(const MouseCommandDTO& hover_command_dto);
    BoardViewDTO getBoardState() const;
    std::vector<GridPosition> getWinningLine(const GridPosition& lastMove, StoneColor color) const;
    const std::vector<Move>& getMoveHistory() const { return _moveHistory; }
    static int getBoardSize() { return Board::SIZE; }

    // Game control methods
    void newGame();
    bool undoLastMove();
    void pauseGame();
    void resumeGame();

    // Player management
    void setPlayerIds(const std::string& player1Id, const std::string& player2Id);
    std::string getCurrentPlayerId() const;

private:
    static bool isValidGridPosition(const GridPosition& pos);
    bool isPositionOccupied(const GridPosition& pos) const;
    static constexpr int WINNING_LENGTH = 5;
};

#endif