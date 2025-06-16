#ifndef IGAMESERVICE_H
#define IGAMESERVICE_H

#include "IMenuEventListener.h"
#include "logic/Board.h"
#include "logic/DTO/CommandDTOs.h"
#include "presentation/DTO/ViewModelDTOs.h"

class IBoardEventListener;

class IGameService {
public:
    virtual ~IGameService() = default;

    virtual bool loadGame() = 0;
    virtual void addListener(IBoardEventListener* listener) = 0;
    virtual void addMenuListener(IMenuEventListener* listener) = 0;
    virtual void update(double deltaTime) = 0;
    virtual bool isCurrentPlayerAI() const = 0;

    virtual void startGame(const GameSetupCommandDTO& setupCommand) = 0;
    virtual GameStatus changeGameStatus() = 0;
    virtual StoneViewDTO processMouseHover(const MouseCommandDTO& hover_command_dto) const = 0;
    virtual MoveViewDTO processMouseClick(const MouseCommandDTO& click_command_dto) = 0;
    virtual BoardViewDTO getBoardState() const = 0;
    virtual int getBoardSize() const = 0;
    virtual void pauseGame() = 0;

    virtual void initialize() = 0;
    virtual const std::vector<Move>& getMoveHistory() const = 0;
};

#endif