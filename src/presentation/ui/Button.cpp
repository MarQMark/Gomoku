#include "presentation/ui/Button.h"

#include <utility>

Button::Button(std::string name) : IInteractable(std::move(name)) {
}

Button::Button(std::string name, glm::vec2 pos, glm::vec2 dim) : IInteractable(std::move(name), pos, dim) {
}

void Button::render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) {
    glm::vec2 pos = parentPos + _pos / parentDim;
    glm::vec2 dim = parentDim * _dim;
    if(_texture_id >= 0){
        renderer->drawTexture(pos, dim, _texture_id, 0, _shader_id);
    }
    else{
        glm::vec3 color;
        switch (getState()) {
            case State::NONE:
                color = glm::vec3(.5);
                break;
            case State::HOVERED:
                color = glm::vec3(.7);
                break;
            case State::PRESSED:
                color = glm::vec3(.3);
                break;
            case State::HELD:
                color = glm::vec3(.15);
                break;
            case State::RELEASED:
                color = glm::vec3(.8);
                break;
        }

        renderer->drawQuad(pos, dim, glm::vec4(color, 1), 5, _shader_id);
    }
}

void Button::setTexture(uint16_t textureID) {
    _texture_id = textureID;
}

void Button::setShader(uint16_t shaderID) {
    _shader_id = shaderID;
}
