#include "logic/GameService.h"
#include "logic/mapping/MapLogicToView.h"

GameService::GameService() {
    newGame();
}

// Initialize game state
void GameService::newGame() {
    _state.board = Board();
    _state.currentPlayerTurn = BLACK;
    _state.moveNumber = 0;
    _state.status = IN_PROGRESS;
    _state.lastMove = GridPosition(-1, -1);
    _player1Id = "black_player";
    _player2Id = "white_player";
    _moveHistory.clear();
}

BoardViewDTO GameService::getBoardState() const {
    return MapLogicToView::toBoardView(_state.board, _state, getWinningLine(_state.lastMove, _state.currentPlayerTurn));
}

MoveViewDTO GameService::processMove(const MouseCommandDTO& cmd) {

    GridPosition pos = cmd.gridPosition;

    if (!isValidGridPosition(pos)) {
        return MapLogicToView::createMoveView(
            false, getBoardState(),StoneViewDTO(false, pos, _state.currentPlayerTurn), "Invalid command"
        );
    }

    if (_state.status != IN_PROGRESS) {
        return MapLogicToView::createMoveView(
            false, getBoardState(), StoneViewDTO(false, pos, _state.currentPlayerTurn), "Game is not in progress"
        );
    }

    if (!pos.isValid()) {
        return MapLogicToView::createMoveView(
            false, getBoardState(), StoneViewDTO(false, pos, _state.currentPlayerTurn), "Invalid board position"
        );
    }

    if (_state.board.getColor(pos) != STONE_NONE) {
        return MapLogicToView::createMoveView(
            false, getBoardState(), StoneViewDTO(false, pos, _state.currentPlayerTurn), "Position already occupied"
        );
    }

    if (!_state.board.placeStone(pos, _state.currentPlayerTurn)) {
        return MapLogicToView::createMoveView(
            false, getBoardState(), StoneViewDTO(false, pos, _state.currentPlayerTurn), "Failed to place stone"
        );
    }

    _moveHistory.emplace_back(pos, _state.currentPlayerTurn);
    _state.lastMove = pos;
    _state.moveNumber++;

    // Check for win
    const StoneColor winner = checkForWin(pos, _state.currentPlayerTurn);
    if (winner != STONE_NONE) {
        const auto winningPositions = getWinningLine(_state.lastMove, _state.currentPlayerTurn);
        _state.status = (winner == BLACK) ? BLACK_WINS : WHITE_WINS;

        return MapLogicToView::createMoveView(true,
            MapLogicToView::toBoardView(_state.board, _state, winningPositions),
            StoneViewDTO(true, pos, _state.currentPlayerTurn));
    }

    if (_state.board.isFull()) {
        _state.status = DRAW;
        return MapLogicToView::createMoveView(true, getBoardState(), StoneViewDTO(true, pos, _state.currentPlayerTurn), "");
    }

    // Swap turn
    const StoneColor prevPlayerTurn = _state.currentPlayerTurn;
    if (_state.status == IN_PROGRESS) {
        _state.currentPlayerTurn = (_state.currentPlayerTurn == BLACK) ? WHITE : BLACK;
    }

    return MapLogicToView::createMoveView(
        true, getBoardState(), StoneViewDTO(true, pos, prevPlayerTurn), ""
    );
}

MouseHoverViewDTO GameService::processMouseHover(const MouseCommandDTO& hover_command_dto) const {
    if (!hover_command_dto.gridPosition.isValid() || _state.status != IN_PROGRESS) {
        return {false, GridPosition(-1, -1), STONE_NONE};
    }

    if (isPositionOccupied(hover_command_dto.gridPosition)) {
        return {false, hover_command_dto.gridPosition, STONE_NONE};
    }

    return {true, hover_command_dto.gridPosition, _state.currentPlayerTurn};
}

MoveViewDTO GameService::processMouseClick(const MouseCommandDTO& hover_command_dto) {
    return processMove(hover_command_dto);
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


bool GameService::undoLastMove() {
    if (_moveHistory.empty()) {
        return false;
    }

    const Move lastMove = _moveHistory.back();
    _moveHistory.pop_back();

    if (!_state.board.removeStone(lastMove.position)) {
        _moveHistory.push_back(lastMove);
        return false;
    }

    _state.moveNumber--;
    _state.currentPlayerTurn = lastMove.color;
    _state.status = IN_PROGRESS;

    if (!_moveHistory.empty()) {
        _state.lastMove = _moveHistory.back().position;
    } else {
        _state.lastMove = GridPosition(-1, -1);
    }

    return true;
}

void GameService::pauseGame() {
    if (_state.status == IN_PROGRESS) {
        _state.status = PAUSED;
    }
}

void GameService::resumeGame() {
    if (_state.status == PAUSED) {
        _state.status = IN_PROGRESS;
    }
}

void GameService::setPlayerIds(const std::string& player1Id, const std::string& player2Id) {
    _player1Id = player1Id;
    _player2Id = player2Id;
}

std::string GameService::getCurrentPlayerId() const {
    return (_state.currentPlayerTurn == BLACK) ? _player1Id : _player2Id;
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