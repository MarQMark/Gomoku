
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
    MoveModel(const int x_, const int y_, const int moveNumber_, std::string stoneColor_)
        : x(x_), y(y_), moveNumber(moveNumber_), stoneColor(std::move(stoneColor_)) {}
};

struct PlayerModel {
    std::string name;
    std::string color;
    bool isAI;
    int difficulty; // -1 if not AI

    PlayerModel() : name(""), color("NONE"), isAI(false), difficulty(-1) {}
    PlayerModel(std::string name_, std::string color_, const bool isAI_, const int difficulty_ = -1)
        : name(std::move(name_)), color(std::move(color_)), isAI(isAI_), difficulty(difficulty_) {}
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

    explicit SaveGameModel(std::string gameId_)
        : gameId(std::move(gameId_)) {}
};

#endif
