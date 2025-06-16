
#ifndef PERSISTENCEDTOS_H
#define PERSISTENCEDTOS_H


#include <string>
#include <vector>
#include <utility>

struct MoveModel {
    int x;
    int y;
    int moveNumber;
    std::string stoneColor;

    MoveModel() : x(0), y(0), moveNumber(0), stoneColor("NONE") {}
    MoveModel(const int x, const int y, const int moveNumber, std::string stoneColor)
        : x(x), y(y), moveNumber(moveNumber), stoneColor(std::move(stoneColor)) {}
};

struct PlayerModel {
    std::string name;
    std::string color;
    bool isAI;
    int difficulty;

    PlayerModel() : name(""), color("NONE"), isAI(false), difficulty(-1) {}
    PlayerModel(std::string name, std::string color, const bool isAI, const int difficulty = -1)
        : name(std::move(name)), color(std::move(color)), isAI(isAI), difficulty(difficulty) {}
};

struct GameStateModel {
    int moveCount;
    int aiDifficulty;
    double elapsedTime;
    std::string gameType;
    std::string currentGameStatus;
    std::string currentPlayerColor;

    GameStateModel()
        : moveCount(0), aiDifficulty(-1), elapsedTime(0.0),
          gameType("HUMAN_VS_HUMAN"), currentGameStatus("NOT_STARTED"), currentPlayerColor("BLACK") {}
};

struct SaveGameModel {
    std::string gameId;
    PlayerModel player1;
    PlayerModel player2;
    std::vector<MoveModel> moveHistory;
    GameStateModel gameState;

    explicit SaveGameModel(std::string gameId)
        : gameId(std::move(gameId)) {}
};

#endif
