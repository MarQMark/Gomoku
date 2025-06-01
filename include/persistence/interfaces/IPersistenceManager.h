#ifndef IPERSISTENCEMANAGER_H
#define IPERSISTENCEMANAGER_H
#include <string>


struct GameEntity;

class IPersistenceManager {
public:
    virtual ~IPersistenceManager() = default;

    virtual bool saveGame(GameEntity gameData) = 0;
    virtual bool loadGame(std::string gameId) = 0;
};



#endif //IPERSISTENCEMANAGER_H
