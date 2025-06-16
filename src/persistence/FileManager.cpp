#include <iostream>
#include <filesystem>
#include <fstream>
#include "persistence/FileManager.h"
#include "persistence/json/json.hpp"

#ifdef _WIN32
    #include <windows.h>
    #include <shlobj.h> // SHGetFolderPath
#else
#include <unistd.h>
#include <pwd.h>
#endif

FileManager::FileManager() {
    get_file_path();
}


void FileManager::get_file_path() {
#ifdef _WIN32
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path))) {
        basePath = std::string(path) + "\\Gomoku\\";
    } else {
        std::cout << "[WARNING] Failed to get %LOCALAPPDATA%\n";
        return;
    }
#else
    const char* home = getenv("HOME");
    if (!home) {
        struct passwd* pw = getpwuid(getuid());
        if (!pw) {
            std::cout << "[WARNING] Failed to get home directory\n";
            return;
        }
        home = pw->pw_dir;
    }
    _file_path = std::string(home) + "/.local/share/Gomoku/";
#endif

    std::filesystem::create_directories(_file_path);

    std::string testFile = _file_path + "test.tmp";
    std::ofstream ofs(testFile);
    if (!ofs) {
        std::cout << "[WARNING] Save path is not writable: " << _file_path << "\n";
        return;
    }
    ofs << "test";
    ofs.close();

    std::ifstream ifs(testFile);
    if (!ifs) {
        std::cout << "[WARNING] Save path is not readable: " << _file_path << "\n";
        return;
    }
    std::string tmp;
    ifs >> tmp;
    ifs.close();

    std::filesystem::remove(testFile);
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

    for(int i = 0; i < (int)gameData.moveHistory.size(); i++){
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
