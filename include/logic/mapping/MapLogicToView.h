#ifndef PRESENTATIONMAPPER_H
#define PRESENTATIONMAPPER_H

#include "logic/Board.h"
#include "presentation/DTO/ViewModelDTOs.h"
#include <vector>

class MapLogicToView {
public:
    static BoardViewDTO mapToBoardViewDTO(const Board& board, const BoardState& state, const std::vector<GridPosition>& winningLine);
    static StatsViewDTO createStatsViewDTO(const IPlayer *player1, const IPlayer *player2, double elapsedTime, const BoardState &boardState);
    static StoneViewDTO createStoneViewDTO(bool isValid, GridPosition gridPosition, StoneColor stoneColor);
    static GameCompleteViewDTO createGameCompletedView(StoneColor winnerColor, GameStatus gameStatus, const std::vector<GridPosition> &winningLine);
    static std::vector<GridViewPosition> mapToViewPositions(const std::vector<GridPosition> &gridPositions);
    static ViewColor mapToViewColor(StoneColor stoneColor);
    static MoveViewDTO createMoveViewDTO(bool success, const BoardViewDTO &view, const StoneViewDTO &stone, const IPlayer &player, const std::string &error = "");
};

#endif