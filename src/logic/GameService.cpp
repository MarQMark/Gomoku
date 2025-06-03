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
    _moveHistory.clear();
}

void GameService::restartSameGame() {
    resetGameState();
    _elapsedTime = 0.0;
    _state.moveNumber = 0;
    _state.status = IN_PROGRESS;
    _state.latestMove = GridPosition(-1, -1);
    _moveHistory.clear();
    notifyGameStarted();
    _state.currentPlayer = _player1.get();
}

void GameService::startGame(const GameSetupCommandDTO &setupCommand) {

    // Clear previous game
    resetGameState();
    createPlayers(setupCommand);

    _activeGameMode = setupCommand.gameMode;

    // Initialize game state
    _elapsedTime = 0.0;
    _state.currentPlayer = _player1.get();
    _state.moveNumber = 0;
    _state.status = IN_PROGRESS;
    _state.latestMove = GridPosition(-1, -1);
    _moveHistory.clear();
    notifyGameStarted();
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

void GameService::loadGame() {
    LoadGameResultDTO* dto = _persistence_manager->loadGame("GameID");
    GameReconstructionResult result = MapPersistenceToLogic::reconstructGame(dto);
    resetGameState();
    _player1 = std::move(result.player1);
    _player2 = std::move(result.player2);
    _state = result.boardState;
    _moveHistory.clear();
    _moveHistory = result.moveHistory;
    _activeGameMode = result.gameMode;
    _elapsedTime = result.elapsedTime;
    notifyGameStarted();
    for (const Move move : result.moveHistory) {
        MoveViewDTO moveViewDTO = MapLogicToView::createMoveViewDTO(true, getBoardState(), MapLogicToView::createStoneViewDTO(true, move.position, move.color), *_state.currentPlayer, "");
        notifyMoveCompleted(moveViewDTO);
    }

    if (_state.status == BLACK_WINS || _state.status == WHITE_WINS) {
        const StoneColor winner = checkForWin(_state.latestMove, _state.currentPlayer->getColor());
        const auto winningPositions = getWinningLine(_state.latestMove, _state.currentPlayer->getColor());
        notifyGameCompleted(MapLogicToView::createGameCompletedView(winner, _state.status, winningPositions));
    }

    if (_state.status == DRAW) {
        notifyGameCompleted(MapLogicToView::createGameCompletedView(STONE_NONE, _state.status, std::vector<GridPosition>()));
    }
}

BoardViewDTO GameService::getBoardState() const {
    return MapLogicToView::mapToBoardViewDTO(_state.board, _state, getWinningLine(_state.latestMove, _state.currentPlayer->getColor()));
}

bool GameService::validateMouseCommand(GridPosition pos, MoveViewDTO &moveViewDTO) {
    if (!isValidGridPosition(pos)) {
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

MoveViewDTO GameService::processMove(const MouseCommandDTO& cmd) {
    GridPosition pos = cmd.gridPosition;

    MoveViewDTO invalidMoveView;
    if (validateMouseCommand(pos, invalidMoveView))
        return invalidMoveView;

    _moveHistory.emplace_back(pos, _state.currentPlayer->getColor());
    _state.latestMove = pos;
    _state.moveNumber++;

    // Check for win
    const StoneColor winner = checkForWin(_state.latestMove, _state.currentPlayer->getColor());
    if (winner != STONE_NONE) {
        const auto winningPositions = getWinningLine(_state.latestMove, _state.currentPlayer->getColor());
        _state.status = (winner == BLACK) ? BLACK_WINS : WHITE_WINS;
        notifyGameCompleted(MapLogicToView::createGameCompletedView(winner, _state.status, winningPositions));
        saveGame();
        return MapLogicToView::createMoveViewDTO(true,
                                                 MapLogicToView::mapToBoardViewDTO(_state.board, _state, winningPositions),
                                                 MapLogicToView::createStoneViewDTO(true, pos, _state.currentPlayer->getColor()), *_state.currentPlayer);
    }

    if (_state.board.isFull()) {
        _state.status = DRAW;
        notifyGameCompleted(MapLogicToView::createGameCompletedView(winner, _state.status, std::vector<GridPosition>()));
        saveGame();
        return MapLogicToView::createMoveViewDTO(true,
            getBoardState(), MapLogicToView::createStoneViewDTO(true, pos,  _state.currentPlayer->getColor()), *_state.currentPlayer, "");
    }


    // Swap turn
    const StoneColor prevPlayerTurn = _state.currentPlayer->getColor();
    if (_state.status == IN_PROGRESS) {
        _state.currentPlayer = (_state.currentPlayer == _player1.get()) ? _player2.get() : _player1.get();
    }

    saveGame();

    return MapLogicToView::createMoveViewDTO(
        true, getBoardState(), MapLogicToView::createStoneViewDTO(true, pos, prevPlayerTurn), *_state.currentPlayer, ""
    );
}

void GameService::saveGame() const {
    _persistence_manager->saveGame(MapLogicToModel::mapToSave(
                "GameID",
                _state,
                _moveHistory,
                _player1.get(),
                _player2.get(),
                _activeGameMode,
                _elapsedTime
            ));
}

StoneViewDTO GameService::processMouseHover(const MouseCommandDTO& hover_command_dto) const {
    if (!hover_command_dto.gridPosition.isValid() || _state.status != IN_PROGRESS) {
        return MapLogicToView::createStoneViewDTO(false, GridPosition(-1, -1), STONE_NONE);
    }

    if (isPositionOccupied(hover_command_dto.gridPosition)) {
        return MapLogicToView::createStoneViewDTO(false, hover_command_dto.gridPosition, STONE_NONE);
    }

    return MapLogicToView::createStoneViewDTO(true, hover_command_dto.gridPosition, _state.currentPlayer->getColor());
}

std::vector<GridPosition> GameService::getWinningLine(const GridPosition &lastMove, const StoneColor color) const {
    for (int dir = 0; dir < 4; dir++) {
        constexpr int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
        auto line = _state.board.getLineInDirection(lastMove, color, directions[dir][0], directions[dir][1]);
        if (line.count >= 5) {
            return line.positions;
        }
    }

    return {};
}

StoneColor GameService::checkForWin(const GridPosition& lastMove, const StoneColor color) const {
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

bool GameService::isPlayerValid(const std::string &playerId) {
    return !playerId.empty();
}

bool GameService::isValidGridPosition(const GridPosition& pos) {
    return pos.isValid();
}

bool GameService::isPositionOccupied(const GridPosition& pos) const {
    return _state.board.getColor(pos) != STONE_NONE;
}

void GameService::addListener(IBoardEventListener* listener) {
    _boardListeners.push_back(listener);
}

void GameService::addMenuListener(IMenuEventListener *listener) {
    _menuListeners.push_back(listener);
}

void GameService::update(const double deltaTime) {
    if (_state.status != IN_PROGRESS) return;

    _elapsedTime += deltaTime;
    if (isCurrentPlayerAI()) {
        _aiMoveTimer += deltaTime;
        if (_aiMoveTimer >= _aiMoveDelay) {
            const auto result = executeAIMove();
            if (result.success) {
                notifyMoveCompleted(result);
            }
            _aiMoveTimer = 0.0f;
        }
    }
    notifyStatsChanged();
}

bool GameService::isCurrentPlayerAI() const {
    return _state.currentPlayer->isAIPlayer();
}

MoveViewDTO GameService::executeAIMove() {
    if (!isCurrentPlayerAI()) {
        return MoveViewDTO(false, std::string("Not AI turn"), getBoardState(), MapLogicToView::createStoneViewDTO(false, GridPosition(), STONE_NONE));
    }

    const auto* aiPlayer = dynamic_cast<AIPlayer*>(_state.currentPlayer);
    const GridPosition aiMove = aiPlayer->calculateBestMove(_state.board);

    MouseCommandDTO aiCommand;
    aiCommand.gridPosition = aiMove;
    return processMove(aiCommand);
}

void GameService::notifyGameStarted() const {
    for (auto* listener : _boardListeners) {
        listener->onGameStarted();
    }
}

void GameService::notifyStatsChanged() const {
    for (auto* menuListener : _menuListeners) {
        menuListener->onStatsChanged(MapLogicToView::createStatsViewDTO(_player1.get(), _player2.get(), _elapsedTime, _state));
    }
}

void GameService::notifyMoveCompleted(const MoveViewDTO& move) const {
    for (auto* listener : _boardListeners) {
        listener->onMoveCompleted(move);
    }
}

void GameService::notifyGameCompleted(const GameCompleteViewDTO &completeView) const {
    for (auto* listener : _boardListeners) {
        listener->onGameCompleted(completeView);
    }

    for (auto* menuListener : _menuListeners) {
        menuListener->onGameCompleted();
        menuListener->onStatsChanged(MapLogicToView::createStatsViewDTO(_player1.get(), _player2.get(), _elapsedTime, _state));
    }
}

MoveViewDTO GameService::processMouseClick(const MouseCommandDTO& cmd) {
    // Don't process if the AI is currently playing!
    if (isCurrentPlayerAI()) {
        auto result = MapLogicToView::createMoveViewDTO(false, getBoardState(), MapLogicToView::createStoneViewDTO(false, cmd.gridPosition, _state.currentPlayer->getColor()), *_state.currentPlayer, "Can't Move cause AI is Moving!");
        return result;
    }

    auto result = processMove(cmd);
    if (result.success) {
        notifyMoveCompleted(result);
    }
    return result;
}
