#include "presentation/ui/Button.h"

#include <utility>

Button::Button(std::string name) : IInteractable(std::move(name)) {
    init_maps();
}

Button::Button(std::string name, glm::vec2 pos, glm::vec2 dim) : IInteractable(std::move(name), pos, dim) {
    init_maps();
}


void Button::init_maps() {
    _texture_ids[State::NONE] = -1;
    _texture_ids[State::HOVERING] = -1;
    _texture_ids[State::PRESSED] = -1;
    _texture_ids[State::HOLDING] = -1;
    _texture_ids[State::RELEASED] = -1;

    _shader_ids[State::NONE] = 0;
    _shader_ids[State::HOVERING] = 0;
    _shader_ids[State::PRESSED] = 0;
    _shader_ids[State::HOLDING] = 0;
    _shader_ids[State::RELEASED] = 0;
}

void Button::render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) {
    IInteractable::render(renderer, parentPos, parentDim);

    if(_texture_ids[getState()] >= 0){
        renderer->drawTextureID((uint64_t)this, _abs_pos, _abs_dim, _texture_ids[getState()], 0, _shader_ids[getState()]);
    }
    else{
        glm::vec3 color;
        switch (getState()) {
            case State::NONE:
                color = glm::vec3(.5);
                break;
            case State::HOVERING:
                color = glm::vec3(.7);
                break;
            case State::PRESSED:
                color = glm::vec3(.3);
                break;
            case State::HOLDING:
                color = glm::vec3(.15);
                break;
            case State::RELEASED:
                color = glm::vec3(.8);
                break;
        }

        renderer->drawQuadID((uint64_t)this, _abs_pos, _abs_dim, glm::vec4(color, 1), 5, _shader_ids[getState()]);

        if(isFocused())
            renderer->drawQuad(_abs_pos, glm::vec2(_abs_dim.x * .02, _abs_dim.y), glm::vec4(.3, .3, .3, 1), 6, _shader_ids[getState()]);
    }
}

void Button::setTexture(IInteractable::State state, uint16_t textureID) {
    _texture_ids[state] = textureID;
}

void Button::setShader(IInteractable::State state, uint16_t shaderID) {
    _shader_ids[state] = shaderID;
}

