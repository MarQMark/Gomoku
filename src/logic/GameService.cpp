#include "logic/GameService.h"

#include "common/Common.h"
#include "logic/mapping/MapLogicToView.h"
#include "logic/player/AIPlayer.h"
#include "logic/player/HumanPlayer.h"
#include "logic/mapping/MapLogicToModel.h"
#include "persistence/mapping/MapPersistenceToLogic.h"

GameService::GameService(IPersistenceManager* persistenceManager) : _persistence_manager(persistenceManager) {
    initialize();
}

// Initialize game state
void GameService::initialize() {
    _state.board = Board();
    _state.moveNumber = 0;
    _state.status = NOT_STARTED;
    _state.latestMove = GridPosition(-1, -1);
    _player1 = std::make_unique<HumanPlayer>("Player 1", BLACK);;
    _player2 = std::make_unique<HumanPlayer>("Player 2", WHITE);;
    _state.currentPlayer = _player1.get();
    _move_history.clear();
}

void GameService::restartSameGame() {
    resetGameState();
    _elapsed_time = 0.0;
    _state.moveNumber = 0;
    _state.status = IN_PROGRESS;
    _state.latestMove = GridPosition(-1, -1);
    _move_history.clear();
    notify_game_started();
    _state.currentPlayer = _player1.get();
}

void GameService::startGame(const GameSetupCommandDTO &setupCommand) {

    // Clear previous game
    resetGameState();
    createPlayers(setupCommand);

    _active_game_mode = setupCommand.gameMode;

    // Initialize game state
    _elapsed_time = 0.0;
    _state.currentPlayer = _player1.get();
    _state.moveNumber = 0;
    _state.status = IN_PROGRESS;
    _state.latestMove = GridPosition(-1, -1);
    _move_history.clear();
    notify_game_started();
}

void GameService::resetGameState() {
    _state.board.clear();
}

void GameService::createPlayers(const GameSetupCommandDTO &setupCommand) {
    switch (setupCommand.gameMode) {
        case HUMAN_VS_HUMAN:
            _player1 = std::make_unique<HumanPlayer>(setupCommand.player1Name, BLACK);
            _player2 = std::make_unique<HumanPlayer>(setupCommand.player2Name, WHITE);
            break;

        case HUMAN_VS_AI:
            _player1 = std::make_unique<HumanPlayer>(setupCommand.player1Name, BLACK);
            _player2 = std::make_unique<AIPlayer>(setupCommand.player2Name, WHITE, setupCommand.aiDifficulty);
            break;

        case AI_VS_HUMAN:
            _player1 = std::make_unique<AIPlayer>(setupCommand.player2Name, WHITE, setupCommand.aiDifficulty);
            _player2 = std::make_unique<HumanPlayer>(setupCommand.player1Name, BLACK);
            break;

        case AI_VS_AI:
            _player1 = std::make_unique<AIPlayer>(setupCommand.player1Name, BLACK, setupCommand.aiDifficulty);
            _player2 = std::make_unique<AIPlayer>(setupCommand.player2Name, WHITE, setupCommand.aiDifficulty);
            break;
        default: ;
    }
}

bool GameService::loadGame() {
    LoadGameResultDTO* dto = _persistence_manager->loadGame("GameID");
    GameReconstructionResult result = MapPersistenceToLogic::reconstructGame(dto);
    if(!result.success)
        return false;

    resetGameState();
    _player1 = std::move(result.player1);
    _player2 = std::move(result.player2);
    _state = result.boardState;
    _move_history.clear();
    _move_history = result.moveHistory;
    _active_game_mode = result.gameMode;
    _elapsed_time = result.elapsedTime;
    notify_game_started();
    for (const Move move : result.moveHistory) {
        MoveViewDTO moveViewDTO = MapLogicToView::createMoveViewDTO(true, getBoardState(), MapLogicToView::createStoneViewDTO(true, move.position, move.color), *_state.currentPlayer, "");
        notify_move_completed(moveViewDTO);
    }

    if (_state.status == BLACK_WINS || _state.status == WHITE_WINS) {
        const StoneColor winner = check_for_win(_state.latestMove, _state.currentPlayer->getColor());
        const auto winningPositions = get_winning_line(_state.latestMove, _state.currentPlayer->getColor());
        notify_game_completed(MapLogicToView::createGameCompletedView(winner, _state.status, winningPositions));
    }

    if (_state.status == DRAW) {
        notify_game_completed(
                MapLogicToView::createGameCompletedView(STONE_NONE, _state.status, std::vector<GridPosition>()));
    }

    return true;
}

BoardViewDTO GameService::getBoardState() const {
    return MapLogicToView::mapToBoardViewDTO(_state.board, _state,
                                             get_winning_line(_state.latestMove, _state.currentPlayer->getColor()));
}

bool GameService::validateMouseCommand(GridPosition pos, MoveViewDTO &moveViewDTO) {
    if (!is_valid_grid_position(pos)) {
        moveViewDTO = MapLogicToView::createMoveViewDTO(
            false, getBoardState(),
            MapLogicToView::createStoneViewDTO(false, pos, _state.currentPlayer->getColor()), *_state.currentPlayer, "Invalid command"
        );
        return true;
    }

    if (_state.status != IN_PROGRESS) {
        moveViewDTO = MapLogicToView::createMoveViewDTO(
            false, getBoardState(),
            MapLogicToView::createStoneViewDTO(false, pos, _state.currentPlayer->getColor()), *_state.currentPlayer, "Game is not in progress"
        );
        return true;
    }

    if (!pos.isValid()) {
        moveViewDTO = MapLogicToView::createMoveViewDTO(
            false, getBoardState(),
            MapLogicToView::createStoneViewDTO(false, pos, _state.currentPlayer->getColor()), *_state.currentPlayer, "Invalid board position"
        );
        return true;
    }

    if (_state.board.getColor(pos) != STONE_NONE) {
        moveViewDTO = MapLogicToView::createMoveViewDTO(
            false, getBoardState(),
            MapLogicToView::createStoneViewDTO(false, pos, _state.currentPlayer->getColor()), *_state.currentPlayer, "Position already occupied"
        );
        return true;
    }

    if (!_state.board.placeStone(pos, _state.currentPlayer->getColor())) {
        moveViewDTO = MapLogicToView::createMoveViewDTO(
            false, getBoardState(),
            MapLogicToView::createStoneViewDTO(false, pos, _state.currentPlayer->getColor()), *_state.currentPlayer, "Failed to place stone"
        );
        return true;
    }
    return false;
}

MoveViewDTO GameService::process_move(const MouseCommandDTO& cmd) {
    GridPosition pos = cmd.gridPosition;

    MoveViewDTO invalidMoveView;
    if (validateMouseCommand(pos, invalidMoveView))
        return invalidMoveView;

    _move_history.emplace_back(pos, _state.currentPlayer->getColor());
    _state.latestMove = pos;
    _state.moveNumber++;

    // Check for win
    const StoneColor winner = check_for_win(_state.latestMove, _state.currentPlayer->getColor());
    if (winner != STONE_NONE) {
        const auto winningPositions = get_winning_line(_state.latestMove, _state.currentPlayer->getColor());
        _state.status = (winner == BLACK) ? BLACK_WINS : WHITE_WINS;
        notify_game_completed(MapLogicToView::createGameCompletedView(winner, _state.status, winningPositions));
        save_game();
        return MapLogicToView::createMoveViewDTO(true,
                                                 MapLogicToView::mapToBoardViewDTO(_state.board, _state, winningPositions),
                                                 MapLogicToView::createStoneViewDTO(true, pos, _state.currentPlayer->getColor()), *_state.currentPlayer);
    }

    if (_state.board.isFull()) {
        _state.status = DRAW;
        notify_game_completed(
                MapLogicToView::createGameCompletedView(winner, _state.status, std::vector<GridPosition>()));
        save_game();
        return MapLogicToView::createMoveViewDTO(true,
            getBoardState(), MapLogicToView::createStoneViewDTO(true, pos,  _state.currentPlayer->getColor()), *_state.currentPlayer, "");
    }


    // Swap turn
    const StoneColor prevPlayerTurn = _state.currentPlayer->getColor();
    if (_state.status == IN_PROGRESS) {
        _state.currentPlayer = (_state.currentPlayer == _player1.get()) ? _player2.get() : _player1.get();
    }

    save_game();

    return MapLogicToView::createMoveViewDTO(
        true, getBoardState(), MapLogicToView::createStoneViewDTO(true, pos, prevPlayerTurn), *_state.currentPlayer, ""
    );
}

void GameService::save_game() const {
    _persistence_manager->saveGame(MapLogicToModel::mapToSave(
            "GameID",
            _state,
            _move_history,
            _player1.get(),
            _player2.get(),
            _active_game_mode,
            _elapsed_time
            ));
}

StoneViewDTO GameService::processMouseHover(const MouseCommandDTO& hoverCommandDTO) const {
    if (!hoverCommandDTO.gridPosition.isValid() || _state.status != IN_PROGRESS) {
        return MapLogicToView::createStoneViewDTO(false, GridPosition(-1, -1), STONE_NONE);
    }

    if (is_position_occupied(hoverCommandDTO.gridPosition)) {
        return MapLogicToView::createStoneViewDTO(false, hoverCommandDTO.gridPosition, STONE_NONE);
    }

    return MapLogicToView::createStoneViewDTO(true, hoverCommandDTO.gridPosition, _state.currentPlayer->getColor());
}

std::vector<GridPosition> GameService::get_winning_line(const GridPosition &lastMove, StoneColor color) const {
    for (int dir = 0; dir < 4; dir++) {
        constexpr int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
        auto line = _state.board.getLineInDirection(lastMove, color, directions[dir][0], directions[dir][1]);
        if (line.count >= 5) {
            return line.positions;
        }
    }

    return {};
}

StoneColor GameService::check_for_win(const GridPosition& lastMove, StoneColor color) const {
    for (int dir = 0; dir < 4; dir++) {
        constexpr int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
        const auto line = _state.board.getLineInDirection(lastMove, color, directions[dir][0], directions[dir][1]);
        if (line.count >= 5) {
            return color;
        }
    }

    return STONE_NONE;
}

GameStatus GameService::changeGameStatus() {
    if (_state.status == IN_PROGRESS) {
        _state.status = PAUSED;
    }
    else if (_state.status == PAUSED) {
        _state.status = IN_PROGRESS;
    }
    else if (_state.status == DRAW || _state.status == WHITE_WINS || _state.status == BLACK_WINS) {
        restartSameGame();
    }


    return _state.status;
}

void GameService::pauseGame() {
    _state.status = PAUSED;
}

bool GameService::is_player_valid(const std::string &playerId) {
    return !playerId.empty();
}

bool GameService::is_valid_grid_position(const GridPosition& pos) {
    return pos.isValid();
}

bool GameService::is_position_occupied(const GridPosition& pos) const {
    return _state.board.getColor(pos) != STONE_NONE;
}

void GameService::addListener(IBoardEventListener* listener) {
    _board_listeners.push_back(listener);
}

void GameService::addMenuListener(IMenuEventListener *listener) {
    _menu_listeners.push_back(listener);
}

void GameService::update(const double deltaTime) {
    if (_state.status != IN_PROGRESS) return;

    _elapsed_time += deltaTime;
    if (isCurrentPlayerAI()) {
        _ai_move_timer += deltaTime;
        if (_ai_move_timer >= _ai_move_delay) {
            const auto result = execute_ai_move();
            if (result.success) {
                notify_move_completed(result);
            }
            _ai_move_timer = 0.0f;
        }
    }
    notify_stats_changed();
}

bool GameService::isCurrentPlayerAI() const {
    return _state.currentPlayer->isAIPlayer();
}

MoveViewDTO GameService::execute_ai_move() {
    if (!isCurrentPlayerAI()) {
        return MoveViewDTO(false, std::string("Not AI turn"), getBoardState(), MapLogicToView::createStoneViewDTO(false, GridPosition(), STONE_NONE));
    }

    const auto* aiPlayer = dynamic_cast<AIPlayer*>(_state.currentPlayer);
    const GridPosition aiMove = aiPlayer->calculateBestMove(_state.board);

    MouseCommandDTO aiCommand;
    aiCommand.gridPosition = aiMove;
    return process_move(aiCommand);
}

void GameService::notify_game_started() const {
    for (auto* listener : _board_listeners) {
        listener->onGameStarted();
    }
}

void GameService::notify_stats_changed() const {
    for (auto* menuListener : _menu_listeners) {
        menuListener->onStatsChanged(MapLogicToView::createStatsViewDTO(_player1.get(), _player2.get(), _elapsed_time, _state));
    }
}

void GameService::notify_move_completed(const MoveViewDTO& move) const {
    for (auto* listener : _board_listeners) {
        listener->onMoveCompleted(move);
    }
}

void GameService::notify_game_completed(const GameCompleteViewDTO &completeView) const {
    for (auto* listener : _board_listeners) {
        listener->onGameCompleted(completeView);
    }

    for (auto* menuListener : _menu_listeners) {
        menuListener->onGameCompleted();
        menuListener->onStatsChanged(MapLogicToView::createStatsViewDTO(_player1.get(), _player2.get(), _elapsed_time, _state));
    }
}

MoveViewDTO GameService::processMouseClick(const MouseCommandDTO& clickCommandDto) {
    // Don't process if the AI is currently playing!
    if (isCurrentPlayerAI()) {
        auto result = MapLogicToView::createMoveViewDTO(false, getBoardState(), MapLogicToView::createStoneViewDTO(false, clickCommandDto.gridPosition, _state.currentPlayer->getColor()), *_state.currentPlayer, "Can't Move cause AI is Moving!");
        return result;
    }

    auto result = process_move(clickCommandDto);
    if (result.success) {
        notify_move_completed(result);
    }
    return result;
}
