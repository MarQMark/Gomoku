#include "presentation/ui/Button.h"

#include <utility>

Button::Button(std::string name) : IInteractable(std::move(name)) {
    init_maps();
}

Button::Button(std::string name, const glm::vec2 pos, const glm::vec2 dim) : IInteractable(std::move(name), pos, dim) {
    init_maps();
}


void Button::init_maps() {
    _layer = 5;

    _texture_names[State::NONE] = "";
    _texture_names[State::HOVERING] = "";
    _texture_names[State::PRESSED] = "";
    _texture_names[State::HOLDING] = "";
    _texture_names[State::RELEASED] = "";

    _shader_names[State::NONE] = "default";
    _shader_names[State::HOVERING] = "default";
    _shader_names[State::PRESSED] = "default";
    _shader_names[State::HOLDING] = "default";
    _shader_names[State::RELEASED] = "default";
}

void Button::render(Renderer* renderer, const glm::vec2 parentPos, const glm::vec2 parentDim) {
    IInteractable::render(renderer, parentPos, parentDim);
    if(!isVisible())
        return;

    if(!_texture_names[getState()].empty()){
        renderer->drawTextureID((uint64_t)this, _abs_pos, _abs_dim, renderer->getTexture(_texture_names[getState()]), _layer, _shader_names[getState()]);
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

        renderer->drawQuadID((uint64_t)this, _abs_pos, _abs_dim, glm::vec4(color, 1), _layer, _shader_names[getState()]);

        if(isFocused())
            renderer->drawQuad(_abs_pos, glm::vec2(_abs_dim.x * .02, _abs_dim.y), glm::vec4(.3, .3, .3, 1), _layer + 0.01, _shader_names[getState()]);
    }
}

void Button::setTexture(const IInteractable::State state, std::string name) {
    _texture_names[state] = name;
}

void Button::setShader(const IInteractable::State state, std::string name) {
    _shader_names[state] = name;
}

