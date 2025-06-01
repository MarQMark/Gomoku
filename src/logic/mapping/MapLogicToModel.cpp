#include "logic/mapping/MapLogicToModel.h"

#include "logic/player/AIPlayer.h"

SaveGameModel MapLogicToModel::mapToSave(
    const std::string &gameId,
    const BoardState &boardState,
    const std::vector<Move> &moveHistory,
    const IPlayer &player1,
    const IPlayer &player2,
    const GameMode &gameMode,
    const double elapsedTime) {

    SaveGameModel model(gameId);

    model.player1 = mapPlayer(player1);
    model.player2 = mapPlayer(player2);

    model.moveHistory.reserve(moveHistory.size());
    for (size_t i = 0; i < moveHistory.size(); ++i) {
        model.moveHistory.push_back(mapMove(moveHistory[i], static_cast<int>(i + 1)));
    }

    model.gameState = mapGameState(boardState, moveHistory, gameMode, elapsedTime);
    return model;
}

PlayerModel MapLogicToModel::mapPlayer(const IPlayer& player) {
    PlayerModel model;
    model.name = player.getName();
    model.color = mapStoneColorToString(player.getColor());
    model.isAI = player.isAIPlayer();

    if (model.isAI) {
        model.difficulty = mapAIDifficultyToInt(extractAIDifficulty(player));
    } else {
        model.difficulty = -1;
    }

    return model;
}

MoveModel MapLogicToModel::mapMove(const Move& move, int moveNumber) {
    MoveModel model;
    model.x = move.position.x;
    model.y = move.position.y;
    model.moveNumber = moveNumber;
    model.stoneColor = mapStoneColorToString(move.color);
    return model;
}

GameStateModel MapLogicToModel::mapGameState(
    const BoardState& boardState,
    const std::vector<Move>& moveHistory,
    const GameMode& gameMode,
    double elapsedTime) {

    GameStateModel stateModel;
    stateModel.moveCount = static_cast<int>(moveHistory.size());
    stateModel.elapsedTime = elapsedTime;
    stateModel.gameType = mapGameModeToString(gameMode);
    stateModel.currentGameStatus = mapGameStatusToString(boardState.status);

    // Set difficulty
    switch (gameMode) {
        case GameMode::HUMAN_VS_AI:
        case GameMode::AI_VS_HUMAN:
        case GameMode::AI_VS_AI:
            stateModel.aiDifficulty = static_cast<int>(AIDifficulty::EXPERT);
            break;
        default:
            stateModel.aiDifficulty = -1;
            break;
    }

    stateModel.currentPlayerColor = mapStoneColorToString(boardState.currentPlayer->getColor());

    return stateModel;
}

AIDifficulty MapLogicToModel::extractAIDifficulty(const IPlayer& player) {
    const auto aiPlayer = dynamic_cast<const AIPlayer*>(&player);
    if (aiPlayer != nullptr) {
        return aiPlayer->getDifficulty();
    }

    return AIDifficulty::RANDOM;
}

bool MapLogicToModel::isPlayerAI(const IPlayer& player) {
    return player.isAIPlayer();
}

// HELPERS
std::string MapLogicToModel::mapStoneColorToString(StoneColor color) {
    switch (color) {
        case StoneColor::BLACK: return "BLACK";
        case StoneColor::WHITE: return "WHITE";
        case StoneColor::STONE_NONE:
        default: return "NONE";
    }
}

std::string MapLogicToModel::mapGameStatusToString(GameStatus status) {
    switch (status) {
        case GameStatus::NOT_STARTED: return "NOT_STARTED";
        case GameStatus::IN_PROGRESS: return "IN_PROGRESS";
        case GameStatus::BLACK_WINS: return "BLACK_WINS";
        case GameStatus::WHITE_WINS: return "WHITE_WINS";
        case GameStatus::DRAW: return "DRAW";
        case GameStatus::PAUSED: return "PAUSED";
        default: return "NOT_STARTED";
    }
}

std::string MapLogicToModel::mapGameModeToString(GameMode mode) {
    switch (mode) {
        case GameMode::HUMAN_VS_HUMAN: return "HUMAN_VS_HUMAN";
        case GameMode::HUMAN_VS_AI: return "HUMAN_VS_AI";
        case GameMode::AI_VS_HUMAN: return "AI_VS_HUMAN";
        case GameMode::AI_VS_AI: return "AI_VS_AI";
        default: return "HUMAN_VS_HUMAN";
    }
}

std::string MapLogicToModel::mapAIDifficultyToString(AIDifficulty difficulty) {
    switch (difficulty) {
        case AIDifficulty::RANDOM: return "RANDOM";
        case AIDifficulty::BASIC: return "BASIC";
        case AIDifficulty::GOOD: return "GOOD";
        case AIDifficulty::EXPERT: return "EXPERT";
        default: return "RANDOM";
    }
}

int MapLogicToModel::mapAIDifficultyToInt(AIDifficulty difficulty) {
    return static_cast<int>(difficulty);
}