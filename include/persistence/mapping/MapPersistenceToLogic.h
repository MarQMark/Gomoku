#ifndef MAPPERSISTENCETOLOGIC_H
#define MAPPERSISTENCETOLOGIC_H

#include "logic/Board.h"
#include "logic/player/IPlayer.h"
#include "persistence/DTO/PersistenceResultDTOs.h"
#include "persistence/Models.h"
#include "logic/DTO/CommandDTOs.h"
#include "common/Common.h"
#include <memory>
#include <vector>

struct GameReconstructionResult {
    bool success;
    std::string errorMessage;
    BoardState boardState;
    std::vector<Move> moveHistory;
    std::unique_ptr<IPlayer> player1;
    std::unique_ptr<IPlayer> player2;
    GameMode gameMode;
    double elapsedTime;
};

class MapPersistenceToLogic {
public:
    static GameReconstructionResult reconstructGame(LoadGameResultDTO* loadResult);

    static std::unique_ptr<IPlayer> createPlayer(const PlayerModel& playerDto);
    static Move createMove(const MoveModel& moveDto);
    static BoardState createBoardState(
        const GameStateModel& gameStateDto,
        const std::vector<Move>& moveHistory,
        IPlayer* player1,
        IPlayer* player2
    );

    // Helper
    static StoneColor mapStringToStoneColor(const std::string& colorString);
    static GameStatus mapStringToGameStatus(const std::string& statusString);
    static GameMode mapStringToGameMode(const std::string& modeString);
    static AIDifficulty mapIntToAIDifficulty(int difficulty);

    // Board
    static Board reconstructBoard(const std::vector<Move>& moveHistory);
    static GridPosition getLastMovePosition(const std::vector<Move>& moveHistory);

private:
    static bool isValidStoneColor(const std::string& colorString);
    static bool isValidGameStatus(const std::string& statusString);
    static bool isValidGameMode(const std::string& modeString);
    static bool isValidPosition(int x, int y);
    static IPlayer* getCurrentPlayer(
        const GameStateModel& gameState,
        IPlayer* player1,
        IPlayer* player2
    );
};

#endif // MAPPERSISTENCETOLOGIC_H