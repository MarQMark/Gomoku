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
    loadTexturesFromFiles();
}

Texture2D* AssetManager::getTexture(const std::string& name) {
    const auto it = _textures.find(name);
    return (it != _textures.end()) ? it->second.get() : nullptr;
}

void AssetManager::loadTexturesFromFiles() {
    _textures[GameTextures::BOARD_GRID] = loadTextureFromFile("res/", GameTextures::BOARD_GRID, ".png");
    _textures[GameTextures::BOARD_BACKGROUND] = loadTextureFromFile("res/", GameTextures::BOARD_BACKGROUND, ".png");
    _textures[GameTextures::BLACK_STONE] = loadTextureFromFile("res/", GameTextures::BLACK_STONE, ".png");
    _textures[GameTextures::WHITE_STONE] = loadTextureFromFile("res/", GameTextures::WHITE_STONE, ".png");
    _textures[GameTextures::BLACK_STONE_HOVER] = loadTextureFromFile("res/", GameTextures::BLACK_STONE_HOVER, ".png");
    _textures[GameTextures::WHITE_STONE_HOVER] = loadTextureFromFile("res/", GameTextures::WHITE_STONE_HOVER, ".png");
}

std::unique_ptr<Texture2D> AssetManager::loadTextureFromFile(const std::string& folderPath, const std::string& filepath, const std::string& fileEnding) {
    const std::string fullPath = folderPath + filepath + fileEnding;
    int w, h, c;
    unsigned char* buf = stbi_load(fullPath.c_str(), &w, &h, &c, 4);

    if (!buf) {
        throw std::runtime_error("Failed to load texture: " + fullPath);
    }

    auto texture = std::make_unique<Texture2D>(w, h, buf);
    stbi_image_free(buf);
    
    return texture;
}