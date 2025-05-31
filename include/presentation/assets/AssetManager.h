#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <memory>
#include <unordered_map>
#include <string>

#include "MagicEnum.h"
#include "Renderer.h"
#include "presentation/renderer/buffer/Texture2D.h"

enum class Textures {
    board_background,
    board_grid,
    black_stone,
    white_stone,
    black_stone_hover,
    white_stone_hover,
};

class AssetManager {
public:
    static void initialize(Renderer *renderer);
    static AssetManager* instance();
    static void cleanup();
    
    void loadGameAssets();

    static std::string getName(Textures texture);
    Texture2D* getTexture(Textures texture);

private:
    Renderer *renderer;
    explicit AssetManager(Renderer* _renderer);
    static AssetManager* s_instance;

    std::unordered_map<std::string, std::unique_ptr<Texture2D>> _textures;
    void loadTexturesFromFiles();
    static std::unique_ptr<Texture2D> loadTextureFromFile(const std::string& folderPath, const std::string& filepath, const std::string& fileEnding);
};

#endif