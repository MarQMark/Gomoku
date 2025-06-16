#ifndef GAMESERVICE_H
#define GAMESERVICE_H

#include <memory>

#include "interfaces/IGameService.h"
#include "Board.h"
#include "interfaces/IBoardEventListener.h"
#include "interfaces/IMenuEventListener.h"
#include "player/IPlayer.h"
#include "persistence/interfaces/IPersistenceManager.h"

class GameService final : public IGameService {
    BoardState _state;
    std::vector<Move> _move_history;
    std::unique_ptr<IPlayer> _player1;
    std::unique_ptr<IPlayer> _player2;
    std::vector<IBoardEventListener*> _board_listeners;
    std::vector<IMenuEventListener*> _menu_listeners;
    GameMode _active_game_mode;
    double _elapsed_time = 0.0;
    double _ai_move_timer = 0.0f;
    double _ai_move_delay = 0.5f;
    IPersistenceManager* _persistence_manager{};

public:
    GameService(IPersistenceManager* persistenceManager);
    ~GameService() override = default;

    void startGame(const GameSetupCommandDTO& setupCommand) override;
    void resetGameState();
    void createPlayers(const GameSetupCommandDTO &setupCommand);
    bool loadGame() override;

    // BOARD
    StoneViewDTO processMouseHover(const MouseCommandDTO& hoverCommandDTO) const override;
    MoveViewDTO processMouseClick(const MouseCommandDTO& clickCommandDTO) override;
    BoardViewDTO getBoardState() const override;

    bool validateMouseCommand(GridPosition pos, MoveViewDTO &moveViewDTO);

    int getBoardSize() const override { return Board::SIZE; }

    void initialize() override;

    void restartSameGame();

    const std::vector<Move>& getMoveHistory() const override { return _move_history; }
    void addListener(IBoardEventListener* listener) override;
    void addMenuListener(IMenuEventListener* listener) override;
    void update(double deltaTime) override;
    bool isCurrentPlayerAI() const override;

private:
    void notify_game_started() const;
    void notify_stats_changed() const;
    void notify_move_completed(const MoveViewDTO& move) const;
    void notify_game_completed(const GameCompleteViewDTO &completeView) const;
    MoveViewDTO execute_ai_move();

    StoneColor check_for_win(const GridPosition& lastMove, StoneColor color) const;
    static bool is_player_valid(const std::string &playerId);
    MoveViewDTO process_move(const MouseCommandDTO& cmd);

    void save_game() const;

    std::vector<GridPosition> get_winning_line(const GridPosition& lastMove, StoneColor color) const;
    GameStatus changeGameStatus() override;
    void pauseGame() override;

    static bool is_valid_grid_position(const GridPosition& pos);
    bool is_position_occupied(const GridPosition& pos) const;
    static constexpr int WINNING_LENGTH = 5;

};

#endif