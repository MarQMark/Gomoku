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
    button_none,
    button_hover,
    button_pressed,
    button_select,
    nav_toggle,
    line_bottom_left_top_right,
    line_top_left_bottom_right,
    line_horizontal,
    line_vertical,
    stats_background,
};

class AssetManager {
public:
    static void initialize(Renderer *renderer);
    static AssetManager* instance();
    static void cleanup();
    
    void loadGameAssets();

    static std::string getName(Textures texture);
    Texture2D* getTexture(Textures texture);

    unsigned char* loadPixels(std::string name, int* width, int* height);
    void freeLoadedPixels(unsigned char* data);

private:
    Renderer *_renderer;
    explicit AssetManager(Renderer* renderer);
    static AssetManager* s_instance;

    std::unordered_map<std::string, std::unique_ptr<Texture2D>> _textures;
    void load_textures();
#ifndef USE_EMBEDDED
    static std::unique_ptr<Texture2D> load_texture_from_file(const std::string& folderPath, const std::string& filepath, const std::string& fileEnding);
#else
    static std::unique_ptr<Texture2D> load_texture_from_memory(std::string name);
#endif
};

#endif