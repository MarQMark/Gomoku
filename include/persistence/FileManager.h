#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "persistence/interfaces/IPersistenceManager.h"


class FileManager : public IPersistenceManager {
public:
    FileManager();

    bool saveGame(SaveGameModel gameData) override;
    LoadGameResultDTO* loadGame(std::string gameId) override;

private:
    const std::string _file_name = "gameData.json";
    std::string _file_path = "./";

     void get_file_path();
};



#endif
