#include "logic/GameService.h"
#include "common/PresentationMapper.h"

GameService::GameService() {
    newGame();
}

// Initialize game state
void GameService::newGame() {
    _state.board = Board();
    _state.currentPlayerTurn = StoneColor::BLACK;
    _state.moveNumber = 0;
    _state.status = GameStatus::IN_PROGRESS;
    _state.lastMove = GridPosition(-1, -1);
    _player1Id = "black_player";
    _player2Id = "white_player";
    _moveHistory.clear();
}

MoveResultDTO GameService::processMove(const PlaceStoneCommandDTO& cmd) {
    if (!PresentationMapper::validateCommand(cmd)) {
        return PresentationMapper::createMoveResult(
            false, _state.board, _state, "Invalid command"
        );
    }

    if (_state.status != GameStatus::IN_PROGRESS) {
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

    if (_state.board.getColor(pos) != StoneColor::STONE_NONE) {
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
    StoneColor winner = checkForWin(pos, _state.currentPlayerTurn);
    if (winner != StoneColor::STONE_NONE) {
        _state.status = (winner == StoneColor::BLACK) ? GameStatus::BLACK_WINS : GameStatus::WHITE_WINS;
        winningLine = getWinningLine(pos, _state.currentPlayerTurn); // Simplified
    } else if (_state.board.isFull()) {
        _state.status = GameStatus::DRAW;
    }


    if (_state.status == GameStatus::IN_PROGRESS) {
        _state.currentPlayerTurn = (_state.currentPlayerTurn == StoneColor::BLACK)
                                  ? StoneColor::WHITE : StoneColor::BLACK;
    }

    return PresentationMapper::createMoveResult(
        true, _state.board, _state, "", winningLine
    );
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

    return StoneColor::STONE_NONE;
}

bool GameService::undoLastMove() {
    if (_moveHistory.empty()) {
        return false;
    }

    Move lastMove = _moveHistory.back();
    _moveHistory.pop_back();

    if (!_state.board.removeStone(lastMove.position)) {
        _moveHistory.push_back(lastMove);
        return false;
    }

    _state.moveNumber--;
    _state.currentPlayerTurn = lastMove.color;
    _state.status = GameStatus::IN_PROGRESS;

    if (!_moveHistory.empty()) {
        _state.lastMove = _moveHistory.back().position;
    } else {
        _state.lastMove = GridPosition(-1, -1);
    }

    return true;
}

void GameService::pauseGame() {
    if (_state.status == GameStatus::IN_PROGRESS) {
        _state.status = GameStatus::PAUSED;
    }
}

void GameService::resumeGame() {
    if (_state.status == GameStatus::PAUSED) {
        _state.status = GameStatus::IN_PROGRESS;
    }
}

void GameService::setPlayerIds(const std::string& player1Id, const std::string& player2Id) {
    _player1Id = player1Id;
    _player2Id = player2Id;
}

std::string GameService::getCurrentPlayerId() const {
    return (_state.currentPlayerTurn == StoneColor::BLACK) ? _player1Id : _player2Id;
}

std::vector<GridPosition> GameService::getWinningLine(const GridPosition& lastMove, StoneColor color) {
    std::vector<GridPosition> winningLine;

    winningLine.push_back(lastMove);

    return winningLine;
}