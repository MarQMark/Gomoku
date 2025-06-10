#include "presentation/assets/AssetManager.h"
#include <iostream>

#include "Renderer.h"
#include "presentation/renderer/stb_image/stb_image.h"

AssetManager* AssetManager::s_instance = nullptr;

void AssetManager::initialize(Renderer* renderer) {
    if (!s_instance) {
        s_instance = new AssetManager(renderer);
    }
}

AssetManager::AssetManager(Renderer* _renderer) {
    renderer = _renderer;
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

std::string AssetManager::getName(const Textures texture) {
    return std::string(magic_enum::enum_name(texture));
}

Texture2D* AssetManager::getTexture(Textures texture) {
    auto name = std::string(magic_enum::enum_name(texture));
    const auto it = _textures.find(name);
    return (it != _textures.end()) ? it->second.get() : nullptr;
}

void AssetManager::loadTexturesFromFiles() {
    for (const auto texture : magic_enum::enum_values<Textures>()) {
        auto filename = std::string(magic_enum::enum_name(texture));
        _textures[filename] = loadTextureFromFile("res/", filename, ".png");
        renderer->addTexture(_textures[filename].get(), filename);
    }
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

unsigned char *AssetManager::loadPixelsFromFile(std::string fileName, int *width, int *height) {
    std::string path = "res/" + fileName + ".png";
    int c;
    return stbi_load(path.c_str(), width, height, &c, 4);
}

void AssetManager::freeLoadedPixels(unsigned char *data) {
    stbi_image_free(data);
}
