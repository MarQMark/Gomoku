#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <memory>
#include <unordered_map>
#include <string>
#include "presentation/renderer/Texture2D.h"

class AssetManager {
public:
    static void initialize();
    static AssetManager* instance();
    static void cleanup();
    
    // Core functionality
    void loadGameAssets();
    Texture2D* getTexture(const std::string& name);

    // Game-specific asset names
    struct GameTextures {
        static constexpr auto BOARD_BACKGROUND = "board_background";
        static constexpr auto BOARD_GRID = "board_grid";
        static constexpr auto BLACK_STONE = "black_stone";
        static constexpr auto WHITE_STONE = "white_stone";
        static constexpr auto BLACK_STONE_HOVER = "black_stone_hover";
        static constexpr auto WHITE_STONE_HOVER = "white_stone_hover";
    };

private:
    AssetManager() = default;
    static AssetManager* s_instance;

    std::unordered_map<std::string, std::unique_ptr<Texture2D>> _textures;
    bool tryLoadFromFiles();
    static std::unique_ptr<Texture2D> loadTextureFromFile(const std::string& folderPath, const std::string& filepath, const std::string& fileEnding);
};

#endif