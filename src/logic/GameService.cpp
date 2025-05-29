#include "logic/GameService.h"
#include "common/PresentationMapper.h"
#include <cmath>

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

MoveResultDTO GameService::processMove(const MouseCommandDTO& cmd) {
    if (!PresentationMapper::validatePlaceStoneCommand(cmd)) {
        return PresentationMapper::createMoveResult(
            false, _state.board, _state, "Invalid command"
        );
    }

    if (_state.status != IN_PROGRESS) {
        return PresentationMapper::createMoveResult(
            false, _state.board, _state, "Game is not in progress"
        );
    }

    GridPosition pos = PresentationMapper::commandToPosition(cmd);
    if (!pos.isValid()) {
        return PresentationMapper::createMoveResult(
            false, _state.board, _state, "Invalid board position"
        );
    }

    if (_state.board.getColor(pos) != STONE_NONE) {
        return PresentationMapper::createMoveResult(
            false, _state.board, _state, "Position already occupied"
        );
    }

    if (!_state.board.placeStone(pos, _state.currentPlayerTurn)) {
        return PresentationMapper::createMoveResult(
            false, _state.board, _state, "Failed to place stone"
        );
    }

    _moveHistory.emplace_back(pos, _state.currentPlayerTurn);
    _state.lastMove = pos;
    _state.moveNumber++;

    // Check for win
    std::vector<GridPosition> winningLine;
    const StoneColor winner = checkForWin(pos, _state.currentPlayerTurn);
    if (winner != STONE_NONE) {
        _state.status = (winner == BLACK) ? BLACK_WINS : WHITE_WINS;
        winningLine = getWinningLine(pos, _state.currentPlayerTurn);
    } else if (_state.board.isFull()) {
        _state.status = DRAW;
    }

    if (_state.status == IN_PROGRESS) {
        _state.currentPlayerTurn = (_state.currentPlayerTurn == BLACK)
                                  ? WHITE : BLACK;
    }

    return PresentationMapper::createMoveResult(
        true, _state.board, _state, "", winningLine
    );
}

GridHoverResultDTO GameService::processMouseHover(const MouseCommandDTO& hover_command_dto) const {
    if (!hover_command_dto.gridPosition.isValid() || _state.status != IN_PROGRESS) {
        return {false, GridPosition(-1, -1), STONE_NONE};
    }

    if (isPositionOccupied(hover_command_dto.gridPosition)) {
        return {false, hover_command_dto.gridPosition, STONE_NONE};
    }

    return {true, hover_command_dto.gridPosition, _state.currentPlayerTurn};
}

MoveResultDTO GameService::processMouseClick(const MouseCommandDTO& hover_command_dto) {
    return processMove(hover_command_dto);
}


StoneColor GameService::checkForWin(const GridPosition& lastMove, const StoneColor color) const {
    for (int dir = 0; dir < 4; dir++) {
        constexpr int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
        int count = 1;
        const int dx = directions[dir][0];
        const int dy = directions[dir][1];

        GridPosition check(lastMove.x + dx, lastMove.y + dy);
        while (check.isValid() && _state.board.getColor(check) == color) {
            count++;
            check.x += dx;
            check.y += dy;
        }

        check = GridPosition(lastMove.x - dx, lastMove.y - dy);
        while (check.isValid() && _state.board.getColor(check) == color) {
            count++;
            check.x -= dx;
            check.y -= dy;
        }

        if (count >= 5) {
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

std::vector<GridPosition> GameService::getWinningLine(const GridPosition& lastMove, StoneColor color) {
    std::vector<GridPosition> winningLine;
    winningLine.push_back(lastMove);
    return winningLine;
}

bool GameService::isValidGridPosition(const GridPosition& pos) {
    return pos.isValid();
}

bool GameService::isPositionOccupied(const GridPosition& pos) const {
    return _state.board.getColor(pos) != STONE_NONE;
}