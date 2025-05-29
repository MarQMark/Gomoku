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
    auto boardGridTexture = loadTextureFromFile("res/", GameTextures::BOARD_GRID, ".png");
    auto boardTexture = loadTextureFromFile("res/", GameTextures::BOARD_BACKGROUND, ".png");
    auto blackStone = loadTextureFromFile("res/", GameTextures::BLACK_STONE, ".png");
    auto whiteStone = loadTextureFromFile("res/", GameTextures::WHITE_STONE, ".png");
    auto blackHover = loadTextureFromFile("res/", GameTextures::BLACK_STONE_HOVER, ".png");
    auto whiteHover = loadTextureFromFile("res/", GameTextures::WHITE_STONE_HOVER, ".png");

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

std::unique_ptr<Texture2D> AssetManager::loadTextureFromFile(const std::string& folderPath, const std::string& filepath, const std::string& fileEnding) {
    const std::string fullPath = folderPath + filepath + fileEnding;
    int w, h, c;
    unsigned char* buf = stbi_load(fullPath.c_str(), &w, &h, &c, 4);

    if (!buf) {
        std::cout << "Failed to load texture: " << fullPath << std::endl;
        return nullptr;
    }

    auto texture = std::make_unique<Texture2D>(w, h, buf);
    stbi_image_free(buf);
    
    return texture;
}