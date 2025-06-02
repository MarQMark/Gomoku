#include <iostream>
#include <fstream>
#include "persistence/FileManager.h"
#include "persistence/json/json.hpp"

FileManager::FileManager() {

}

bool FileManager::saveGame(SaveGameModel gameData) {
    nlohmann::json jsonObj;
    jsonObj[gameData.gameId]["Player1"]["Name"] = gameData.player1.name;
    jsonObj[gameData.gameId]["Player1"]["Color"] = gameData.player1.color;
    jsonObj[gameData.gameId]["Player1"]["IsAI"] = gameData.player1.isAI;
    jsonObj[gameData.gameId]["Player1"]["Difficulty"] = gameData.player1.difficulty;

    jsonObj[gameData.gameId]["Player2"]["Name"] = gameData.player2.name;
    jsonObj[gameData.gameId]["Player2"]["Color"] = gameData.player2.color;
    jsonObj[gameData.gameId]["Player2"]["IsAI"] = gameData.player2.isAI;
    jsonObj[gameData.gameId]["Player2"]["Difficulty"] = gameData.player2.difficulty;

    for(int i = 0; i < (int)gameData.moveHistory.size() - 1; i++){
        jsonObj[gameData.gameId]["History"]["X"][i] = gameData.moveHistory[i].x;
        jsonObj[gameData.gameId]["History"]["Y"][i] = gameData.moveHistory[i].y;
        jsonObj[gameData.gameId]["History"]["MoveNumber"][i] = gameData.moveHistory[i].moveNumber;
        jsonObj[gameData.gameId]["History"]["StoneColor"][i] = gameData.moveHistory[i].stoneColor;
    }

    jsonObj[gameData.gameId]["GameState"]["MoveCount"] = gameData.gameState.moveCount;
    jsonObj[gameData.gameId]["GameState"]["AiDifficulty"] = gameData.gameState.aiDifficulty;
    jsonObj[gameData.gameId]["GameState"]["ElapsedTime"] = gameData.gameState.elapsedTime;
    jsonObj[gameData.gameId]["GameState"]["GameType"] = gameData.gameState.gameType;
    jsonObj[gameData.gameId]["GameState"]["CurrentGameStatus"] = gameData.gameState.currentGameStatus;
    jsonObj[gameData.gameId]["GameState"]["CurrentPlayerColor"] = gameData.gameState.currentPlayerColor;

    std::ofstream file(_file_path + _file_name);
    if (!file)
        return false;
    file << std::setw(4) << jsonObj << std::endl;
    if (!file)
        return false;
    file.close();

    return true;
}

LoadGameResultDTO* FileManager::loadGame(std::string gameId) {
    std::ifstream file(_file_path + _file_name);
    if(!file)
        return nullptr;

    nlohmann::json jsonObj;
    try {
        jsonObj = nlohmann::json::parse(file);
    } catch (const nlohmann::json::parse_error &e) {
        printf("Error opening %s Invalid Json\n", (_file_path + _file_name).c_str());
        return nullptr;
    }

    if(!jsonObj.contains(gameId))
        return nullptr;

    // TODO: Add json validation

    auto* loadGameResultDto = new LoadGameResultDTO;
    loadGameResultDto->player1.name       = jsonObj[gameId]["Player1"]["Name"];
    loadGameResultDto->player1.color      = jsonObj[gameId]["Player1"]["Color"];
    loadGameResultDto->player1.isAI       = jsonObj[gameId]["Player1"]["IsAI"];
    loadGameResultDto->player1.difficulty = jsonObj[gameId]["Player1"]["Difficulty"];

    loadGameResultDto->player2.name       = jsonObj[gameId]["Player2"]["Name"];
    loadGameResultDto->player2.color      = jsonObj[gameId]["Player2"]["Color"];
    loadGameResultDto->player2.isAI       = jsonObj[gameId]["Player2"]["IsAI"];
    loadGameResultDto->player2.difficulty = jsonObj[gameId]["Player2"]["Difficulty"];

    const auto& xList = jsonObj[gameId]["History"]["X"];
    const auto& yList = jsonObj[gameId]["History"]["Y"];
    const auto& moveNumList = jsonObj[gameId]["History"]["MoveNumber"];
    const auto& colorList = jsonObj[gameId]["History"]["StoneColor"];

    int historySize = xList.size();
    for (int i = 0; i < historySize; ++i) {
        MoveModel move;
        move.x = xList[i];
        move.y = yList[i];
        move.moveNumber = moveNumList[i];
        move.stoneColor = colorList[i];
        loadGameResultDto->moveHistory.push_back(move);
    }

    loadGameResultDto->gameState.moveCount           = jsonObj[gameId]["GameState"]["MoveCount"];
    loadGameResultDto->gameState.aiDifficulty        = jsonObj[gameId]["GameState"]["AiDifficulty"];
    loadGameResultDto->gameState.elapsedTime         = jsonObj[gameId]["GameState"]["ElapsedTime"];
    loadGameResultDto->gameState.gameType            = jsonObj[gameId]["GameState"]["GameType"];
    loadGameResultDto->gameState.currentGameStatus   = jsonObj[gameId]["GameState"]["CurrentGameStatus"];
    loadGameResultDto->gameState.currentPlayerColor  = jsonObj[gameId]["GameState"]["CurrentPlayerColor"];

    return loadGameResultDto;
}
