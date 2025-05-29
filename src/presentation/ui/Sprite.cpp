#include "presentation/ui/Sprite.h"

#include <iostream>

Sprite::Sprite(std::string name, std::string textureName) : IViewable(std::move(name)), _texture_name(std::move(textureName)){
    _layer = 4;
}

Sprite::Sprite(std::string name, std::string textureName, glm::vec2 pos, glm::vec2 dim) :
        IViewable(std::move(name), pos, dim), _texture_name(std::move(textureName)){
    _layer = 4;
}

void Sprite::render(Renderer *renderer, const glm::vec2 parentPos, const glm::vec2 parentDim) {
    IViewable::render(renderer, parentPos, parentDim);
    if(!isVisible())
        return;

    renderer->drawTextureID((uint64_t)this, _abs_pos, _abs_dim, renderer->getTexture(_texture_name), _layer);
}

void Sprite::setTexture(std::string name) {
    _texture_name = name;
}

std::string Sprite::getTexture() {
    return _texture_name;
}
