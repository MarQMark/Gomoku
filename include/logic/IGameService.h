#ifndef IGAMESERVICE_H
#define IGAMESERVICE_H

#include "Board.h"
#include "DTO/CommandDTOs.h"
#include "presentation/DTO/ViewModelDTOs.h"

class IGameService {
public:
    virtual ~IGameService() = default;

    virtual StoneViewDTO processMouseHover(const MouseCommandDTO& hover_command_dto) const = 0;
    virtual MoveViewDTO processMouseClick(const MouseCommandDTO& click_command_dto) = 0;
    virtual BoardViewDTO getBoardState() const = 0;
    virtual int getBoardSize() const = 0;

    virtual void initialize() = 0;
    virtual const std::vector<Move>& getMoveHistory() const = 0;
};

#endif