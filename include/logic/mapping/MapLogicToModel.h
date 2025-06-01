#ifndef MAPLOGICTOPERSISTENCE_H
#define MAPLOGICTOPERSISTENCE_H

#include "logic/Board.h"
#include "logic/GameService.h"
#include "logic/player/IPlayer.h"
#include "persistence/Models.h"
#include "common/Common.h"

class MapLogicToModel {
public:
    static SaveGameModel mapToSave(
        const std::string &gameId,
        const BoardState &boardState,
        const std::vector<Move> &moveHistory,
        const IPlayer &player1,
        const IPlayer &player2,
        const GameMode &gameMode,
        double elapsedTime = 0.0
    );

    static PlayerModel mapPlayer(const IPlayer& player);
    static MoveModel mapMove(const Move& move, int moveNumber);
    static GameStateModel mapGameState(
        const BoardState& boardState,
        const std::vector<Move>& moveHistory,
        const GameMode& gameMode,
        double elapsedTime
    );

    // Helper
    static std::string mapStoneColorToString(StoneColor color);
    static std::string mapGameStatusToString(GameStatus status);
    static std::string mapGameModeToString(GameMode mode);
    static std::string mapAIDifficultyToString(AIDifficulty difficulty);
    static int mapAIDifficultyToInt(AIDifficulty difficulty);

private:
    // Utility
    static AIDifficulty extractAIDifficulty(const IPlayer& player);
    static bool isPlayerAI(const IPlayer& player);
};



#endif // MAPLOGICTOPERSISTENCE_H