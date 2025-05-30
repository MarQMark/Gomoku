#ifndef PRESENTATIONTOCOMMAND_H
#define PRESENTATIONTOCOMMAND_H
#include "glm/vec2.hpp"
#include "logic/Board.h"
#include "logic/DTO/CommandDTOs.h"


// Maps from Presentation to Logic Layer
class MapPresentationToCommand {
public:
    static MouseCommandDTO toMouseCommandDTO(glm::vec2 relativePosition);
    static GridPosition relativeToGrid(glm::vec2 relativePos);

};



#endif
