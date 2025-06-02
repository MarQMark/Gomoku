#include <cmath>
#include "presentation/mapping/MapViewToCommand.h"

GridPosition MapViewToCommand::relativeToGrid(const glm::vec2 relativePos, const int boardSize) {
    int gridX = std::round((relativePos.x * (boardSize - 1)));
    int gridY = std::round((relativePos.y * (boardSize - 1)));
    return {gridX, gridY};
}

MouseCommandDTO MapViewToCommand::toMouseCommandDTO(const glm::vec2 relativePosition, const int boardSize) {
    return MouseCommandDTO(relativeToGrid(relativePosition, boardSize));
}

GameSetupCommandDTO MapViewToCommand::toGameSetupCommandDTO(const GameMode gameMode, const AIDifficulty difficulty) {
    GameSetupCommandDTO dto;
    dto.gameMode = gameMode;
    const std::pair<std::string, std::string> namePair = mapGameModeToPlayerName(gameMode, difficulty);
    dto.player1Name = namePair.first;
    dto.player2Name = namePair.second;
    dto.aiDifficulty = difficulty;
    return dto;
}

std::pair<std::string, std::string> MapViewToCommand::mapGameModeToPlayerName(const GameMode mode, AIDifficulty difficulty) {

    std::string difficultyStr;
    switch (difficulty) {
        case RANDOM: difficultyStr = "Journalist"; break;
        case BASIC: difficultyStr = "Basic"; break;
        case GOOD: difficultyStr = "Good"; break;
        case EXPERT: difficultyStr = "Expert"; break;
    }

    switch (mode) {
        case HUMAN_VS_HUMAN: return std::pair("Player 1", "Player 2");
        case HUMAN_VS_AI: return std::pair("Player 1", difficultyStr + " AI");
        case AI_VS_HUMAN: return std::pair(difficultyStr + " AI", "Player 2");
        case AI_VS_AI: return std::pair(difficultyStr + " AI", difficultyStr + " AI");
        default: return std::pair("Player 1", "Player 2");
    }
}

