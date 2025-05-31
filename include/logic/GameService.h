#ifndef GAMESERVICE_H
#define GAMESERVICE_H

#include "IGameService.h"
#include "Board.h"

class GameService final : public IGameService {
private:
    GameState _state;
    std::vector<Move> _moveHistory;
    std::string _player1Id;
    std::string _player2Id;

    StoneColor checkForWin(const GridPosition& lastMove, StoneColor color) const;
    static bool isPlayerValid(const std::string &playerId);

public:
    GameService();
    ~GameService() override = default;

    StoneViewDTO processMouseHover(const MouseCommandDTO& hover_command_dto) const override;
    MoveViewDTO processMouseClick(const MouseCommandDTO& click_command_dto) override;
    BoardViewDTO getBoardState() const override;
    int getBoardSize() const override { return Board::SIZE; }

    void newGame() override;
    bool undoLastMove() override;
    const std::vector<Move>& getMoveHistory() const override { return _moveHistory; }

private:
    MoveViewDTO processMove(const MouseCommandDTO& cmd);
    std::vector<GridPosition> getWinningLine(const GridPosition& lastMove, StoneColor color) const;
    void pauseGame();
    void resumeGame();
    void setPlayerIds(const std::string& player1Id, const std::string& player2Id);
    std::string getCurrentPlayerId() const;

    static bool isValidGridPosition(const GridPosition& pos);
    bool isPositionOccupied(const GridPosition& pos) const;
    static constexpr int WINNING_LENGTH = 5;
};

#endif