#ifndef PRESENTATIONTOCOMMAND_H
#define PRESENTATIONTOCOMMAND_H
#include "glm/vec2.hpp"
#include "logic/Board.h"
#include "logic/DTO/CommandDTOs.h"

class MapPresentationToCommand {
public:
    static MouseCommandDTO toMouseCommandDTO(glm::vec2 relativePosition, int boardSize);
    static GridPosition relativeToGrid(glm::vec2 relativePos, int boardSize);

};

#endif
