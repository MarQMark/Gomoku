#include "presentation/ui/Sprite.h"

Sprite::Sprite(std::string name, uint16_t textureID) : IViewable(std::move(name)), _texture_id(textureID){
}

Sprite::Sprite(std::string name, uint16_t textureID, glm::vec2 pos, glm::vec2 dim) :
        IViewable(std::move(name), pos, dim), _texture_id(textureID){
}

void Sprite::render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) {
    IViewable::render(renderer, parentPos, parentDim);

    renderer->drawTextureID((uint64_t)this, _abs_pos, _abs_dim, _texture_id, 8);
}

void Sprite::setTexture(uint16_t id) {
    _texture_id = id;
}

uint16_t Sprite::getTexture() {
    return _texture_id;
}
