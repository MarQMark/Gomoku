#ifndef GAMESERVICE_H
#define GAMESERVICE_H

#include <memory>

#include "interfaces/IGameService.h"
#include "Board.h"
#include "interfaces/IBoardEventListener.h"
#include "player/IPlayer.h"

class GameService final : public IGameService {
    BoardState _state;
    std::vector<Move> _moveHistory;
    std::unique_ptr<IPlayer> _player1;
    std::unique_ptr<IPlayer> _player2;
    std::vector<IBoardEventListener*> _listeners;
    GameMode _activeGameMode;
    double _aiMoveTimer = 0.0f;
    double _aiMoveDelay = 0.05f;

public:
    GameService();
    ~GameService() override = default;

    void startNewGame(const GameSetupCommandDTO& setupCommand) override;
    void resetGameState();
    void createPlayers(const GameSetupCommandDTO &setupCommand);

    // BOARD
    StoneViewDTO processMouseHover(const MouseCommandDTO& hover_command_dto) const override;
    MoveViewDTO processMouseClick(const MouseCommandDTO& click_command_dto) override;
    BoardViewDTO getBoardState() const override;
    int getBoardSize() const override { return Board::SIZE; }

    void initialize() override;
    const std::vector<Move>& getMoveHistory() const override { return _moveHistory; }
    void addListener(IBoardEventListener* listener) override;
    void update(double deltaTime) override;
    bool isCurrentPlayerAI() const override;

private:
    void notifyMoveCompleted(const MoveViewDTO& move) const;
    MoveViewDTO executeAIMove();

    StoneColor checkForWin(const GridPosition& lastMove, StoneColor color) const;
    static bool isPlayerValid(const std::string &playerId);
    MoveViewDTO processMove(const MouseCommandDTO& cmd);
    std::vector<GridPosition> getWinningLine(const GridPosition& lastMove, StoneColor color) const;
    void pauseGame();
    void resumeGame();

    static bool isValidGridPosition(const GridPosition& pos);
    bool isPositionOccupied(const GridPosition& pos) const;
    static constexpr int WINNING_LENGTH = 5;

};

#endif