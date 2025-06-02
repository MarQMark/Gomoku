#include "persistence/mapping/MapPersistenceToLogic.h"
#include <algorithm>

#include "logic/player/AIPlayer.h"
#include "logic/player/HumanPlayer.h"

GameReconstructionResult MapPersistenceToLogic::reconstructGame(LoadGameResultDTO* loadResult) {
    GameReconstructionResult result;
    result.success = false;
    if(!loadResult) {
        result.errorMessage = "Failed to create players from loaded data";
        return result;
    }

    result.elapsedTime = loadResult->gameState.elapsedTime;

    // Create players
    result.player1 = createPlayer(loadResult->player1);
    result.player2 = createPlayer(loadResult->player2);

    if (!result.player1 || !result.player2) {
        result.errorMessage = "Failed to create players from loaded data";
        return result;
    }

    // Create move history
    result.moveHistory.reserve(loadResult->moveHistory.size());
    for (const auto& moveDto : loadResult->moveHistory) {
        result.moveHistory.push_back(createMove(moveDto));
    }

    // Create board state
    result.boardState = createBoardState(
        loadResult->gameState,
        result.moveHistory,
        result.player1.get(),
        result.player2.get()
    );

    // Set game mode
    result.gameMode = mapStringToGameMode(loadResult->gameState.gameType);
    result.success = true;

    delete loadResult;
    return result;
}

std::unique_ptr<IPlayer> MapPersistenceToLogic::createPlayer(const PlayerModel& playerDto) {
    StoneColor color = mapStringToStoneColor(playerDto.color);

    if (playerDto.isAI) {
        AIDifficulty difficulty = mapIntToAIDifficulty(playerDto.difficulty);
        return std::make_unique<AIPlayer>(playerDto.name, color, difficulty);
    } else {
        return std::make_unique<HumanPlayer>(playerDto.name, color);
    }
}

Move MapPersistenceToLogic::createMove(const MoveModel& moveDto) {
    GridPosition position(moveDto.x, moveDto.y);
    StoneColor color = mapStringToStoneColor(moveDto.stoneColor);
    return Move(position, color);
}

BoardState MapPersistenceToLogic::createBoardState(
    const GameStateModel& gameStateDto,
    const std::vector<Move>& moveHistory,
    IPlayer* player1,
    IPlayer* player2) {

    BoardState state;
    state.board = reconstructBoard(moveHistory);
    state.currentPlayer = getCurrentPlayer(gameStateDto, player1, player2);
    state.moveNumber = gameStateDto.moveCount;
    state.status = mapStringToGameStatus(gameStateDto.currentGameStatus);
    state.latestMove = getLastMovePosition(moveHistory);

    return state;
}

Board MapPersistenceToLogic::reconstructBoard(const std::vector<Move>& moveHistory) {
    Board board;

    for (const auto& move : moveHistory) {
        if (move.isValid()) {
            board.placeStone(move.position, move.color);
        }
    }

    return board;
}

GridPosition MapPersistenceToLogic::getLastMovePosition(const std::vector<Move>& moveHistory) {
    if (moveHistory.empty()) {
        return GridPosition();
    }

    return moveHistory.back().position;
}

IPlayer* MapPersistenceToLogic::getCurrentPlayer(
    const GameStateModel& gameState,
    IPlayer* player1,
    IPlayer* player2) {

    if (player1->getColor() == mapStringToStoneColor(gameState.currentPlayerColor)) {
        return player1;
    }
    return player2;
}

// HELPERS
StoneColor MapPersistenceToLogic::mapStringToStoneColor(const std::string& colorString) {
    if (colorString == "BLACK") return StoneColor::BLACK;
    if (colorString == "WHITE") return StoneColor::WHITE;
    return StoneColor::STONE_NONE;
}

GameStatus MapPersistenceToLogic::mapStringToGameStatus(const std::string& statusString) {
    if (statusString == "NOT_STARTED") return GameStatus::NOT_STARTED;
    if (statusString == "IN_PROGRESS") return GameStatus::IN_PROGRESS;
    if (statusString == "BLACK_WINS") return GameStatus::BLACK_WINS;
    if (statusString == "WHITE_WINS") return GameStatus::WHITE_WINS;
    if (statusString == "DRAW") return GameStatus::DRAW;
    if (statusString == "PAUSED") return GameStatus::PAUSED;
    return GameStatus::NOT_STARTED;
}

GameMode MapPersistenceToLogic::mapStringToGameMode(const std::string& modeString) {
    if (modeString == "HUMAN_VS_HUMAN") return GameMode::HUMAN_VS_HUMAN;
    if (modeString == "HUMAN_VS_AI") return GameMode::HUMAN_VS_AI;
    if (modeString == "AI_VS_HUMAN") return GameMode::AI_VS_HUMAN;
    if (modeString == "AI_VS_AI") return GameMode::AI_VS_AI;
    return GameMode::HUMAN_VS_HUMAN;
}

AIDifficulty MapPersistenceToLogic::mapIntToAIDifficulty(int difficulty) {
    switch (difficulty) {
        case 0: return AIDifficulty::RANDOM;
        case 1: return AIDifficulty::BASIC;
        case 2: return AIDifficulty::GOOD;
        case 3: return AIDifficulty::EXPERT;
        default: return AIDifficulty::RANDOM;
    }
}

bool MapPersistenceToLogic::isValidStoneColor(const std::string& colorString) {
    return colorString == "BLACK" || colorString == "WHITE" || colorString == "NONE";
}

bool MapPersistenceToLogic::isValidGameStatus(const std::string& statusString) {
    return statusString == "NOT_STARTED" || statusString == "IN_PROGRESS" ||
           statusString == "BLACK_WINS" || statusString == "WHITE_WINS" ||
           statusString == "DRAW" || statusString == "PAUSED";
}

bool MapPersistenceToLogic::isValidGameMode(const std::string& modeString) {
    return modeString == "HUMAN_VS_HUMAN" || modeString == "HUMAN_VS_AI" ||
           modeString == "AI_VS_HUMAN" || modeString == "AI_VS_AI";
}

bool MapPersistenceToLogic::isValidPosition(int x, int y) {
    return x >= 0 && x < Board::SIZE && y >= 0 && y < Board::SIZE;
}