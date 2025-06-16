#include "presentation/assets/AssetManager.h"
#include <iostream>

#include "Renderer.h"
#include "presentation/renderer/stb_image/stb_image.h"

#ifdef USE_EMBEDDED
// Files get generated with CMake build
#include "embedded_assets.h"
#endif

AssetManager* AssetManager::s_instance = nullptr;

void AssetManager::initialize(Renderer* renderer) {
    if (!s_instance) {
        s_instance = new AssetManager(renderer);
    }
}

AssetManager::AssetManager(Renderer* renderer) {
    _renderer = renderer;
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
    load_textures();
}

std::string AssetManager::getName(const Textures texture) {
    return std::string(magic_enum::enum_name(texture));
}

Texture2D* AssetManager::getTexture(Textures texture) {
    auto name = std::string(magic_enum::enum_name(texture));
    const auto it = _textures.find(name);
    return (it != _textures.end()) ? it->second.get() : nullptr;
}

void AssetManager::load_textures() {
    for (const auto texture : magic_enum::enum_values<Textures>()) {
        auto filename = std::string(magic_enum::enum_name(texture));
#ifndef USE_EMBEDDED
        _textures[filename] = load_texture_from_file("res/", filename, ".png");
#else
        _textures[filename] = load_texture_from_memory(filename);
#endif
        _renderer->addTexture(_textures[filename].get(), filename);
    }
}

#ifndef USE_EMBEDDED
std::unique_ptr<Texture2D> AssetManager::load_texture_from_file(const std::string& folderPath, const std::string& filepath, const std::string& fileEnding) {
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
#else
std::unique_ptr<Texture2D> AssetManager::load_texture_from_memory(std::string name) {
    int w, h, c;
    const auto& [rawBuf, len] = embedded_assets.at(name);
    unsigned char* buf = stbi_load_from_memory(rawBuf, len, &w, &h, &c, 4);

    auto texture = std::make_unique<Texture2D>(w, h, buf);
    stbi_image_free(buf);

    return texture;
}
#endif


unsigned char *AssetManager::loadPixels(std::string name, int *width, int *height) {
    int c;

#ifndef USE_EMBEDDED
    std::string path = "res/" + name + ".png";
    return stbi_load(path.c_str(), width, height, &c, 4);
#else
    const auto& [rawBuf, len] = embedded_assets.at(name);
    return stbi_load_from_memory(rawBuf, len, width, height, &c, 4);
#endif
}

void AssetManager::freeLoadedPixels(unsigned char *data) {
    stbi_image_free(data);
}