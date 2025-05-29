#include "presentation/assets/AssetManager.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "presentation/renderer/stb_image/stb_image.h"

AssetManager* AssetManager::s_instance = nullptr;

void AssetManager::initialize() {
    if (!s_instance) {
        s_instance = new AssetManager();
    }
}

AssetManager* AssetManager::instance() {
    if (!s_instance) {
        throw std::runtime_error("AssetManager not initialized! Call AssetManager::initialize() first.");
    }
    return s_instance;
}

void AssetManager::cleanup() {
    delete s_instance;
    s_instance = nullptr;
}

void AssetManager::loadGameAssets() {
    if (tryLoadFromFiles()) {
        std::cout << "Assets loaded from files" << std::endl;
    } else {
        std::cout << "Asset files not found." << std::endl;
    }
}

Texture2D* AssetManager::getTexture(const std::string& name) {
    const auto it = _textures.find(name);
    return (it != _textures.end()) ? it->second.get() : nullptr;
}

bool AssetManager::tryLoadFromFiles() {
    auto boardGridTexture = loadTextureFromFile("res/board_grid.png");
    auto boardTexture = loadTextureFromFile("res/board_background.png");
    auto blackStone = loadTextureFromFile("res/black_stone.png");
    auto whiteStone = loadTextureFromFile("res/white_stone.png");
    auto blackHover = loadTextureFromFile("res/black_stone_hover.png");
    auto whiteHover = loadTextureFromFile("res/white_stone_hover.png");

    if (boardGridTexture && boardTexture && blackStone && whiteStone && blackHover && whiteHover) {
        _textures[GameTextures::BOARD_GRID] = std::move(boardGridTexture);
        _textures[GameTextures::BOARD_BACKGROUND] = std::move(boardTexture);
        _textures[GameTextures::BLACK_STONE] = std::move(blackStone);
        _textures[GameTextures::WHITE_STONE] = std::move(whiteStone);
        _textures[GameTextures::BLACK_STONE_HOVER] = std::move(blackHover);
        _textures[GameTextures::WHITE_STONE_HOVER] = std::move(whiteHover);
        return true;
    }
    
    return false;
}

std::unique_ptr<Texture2D> AssetManager::loadTextureFromFile(const std::string& filepath) {
    int w, h, c;
    unsigned char* buf = stbi_load(filepath.c_str(), &w, &h, &c, 4);

    if (!buf) {
        std::cout << "Failed to load texture: " << filepath << std::endl;
        return nullptr;
    }

    auto texture = std::make_unique<Texture2D>(w, h, buf);
    stbi_image_free(buf);
    
    return texture;
}