#include "presentation/ui/Sprite.h"

#include <iostream>
#include <utility>

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

    Renderer::Options options;
    options.layer = _layer;
    if(_animator.isActive()){
        options.shaderName = "stone";
        options.animator = &_animator;
        options.animationID = (uint64_t)this;
    }
    renderer->drawTextureID((uint64_t)this, _abs_pos, _abs_dim, renderer->getTexture(_texture_name), options);
}

void Sprite::setTexture(std::string name) {
    _texture_name = std::move(name);
}

std::string Sprite::getTexture() {
    return _texture_name;
}

Animator *Sprite::getAnimator() {
    return &_animator;
}
