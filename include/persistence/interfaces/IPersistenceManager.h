#ifndef IPERSISTENCEMANAGER_H
#define IPERSISTENCEMANAGER_H
#include <string>

#include "persistence/Models.h"
#include "persistence/DTO/PersistenceResultDTOs.h"

class IPersistenceManager {
public:
    virtual ~IPersistenceManager() = default;

    virtual bool saveGame(SaveGameModel gameData) = 0;
    virtual LoadGameResultDTO* loadGame(std::string gameId) = 0;
};



#endif //IPERSISTENCEMANAGER_H
