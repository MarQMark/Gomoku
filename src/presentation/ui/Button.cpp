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

    _texture_names[NONE] = "";
    _texture_names[HOVERING] = "";
    _texture_names[PRESSED] = "";
    _texture_names[HOLDING] = "";
    _texture_names[RELEASED] = "";

    _shader_names[NONE] = "default";
    _shader_names[HOVERING] = "default";
    _shader_names[PRESSED] = "default";
    _shader_names[HOLDING] = "default";
    _shader_names[RELEASED] = "default";
}

void Button::render(Renderer* renderer, const glm::vec2 parentPos, const glm::vec2 parentDim) {
    IInteractable::render(renderer, parentPos, parentDim);
    if(!isVisible())
        return;

    Renderer::Options options;
    options.layer = _layer;
    options.shaderName = _shader_names[getState()];

    if(!_texture_names[getState()].empty()){
        renderer->drawTextureID((uint64_t)this, _abs_pos, _abs_dim, renderer->getTexture(_texture_names[getState()]), options);
    }
    else{
        glm::vec3 color;
        switch (getState()) {
            case NONE:
                color = glm::vec3(.5);
                break;
            case HOVERING:
                color = glm::vec3(.7);
                break;
            case PRESSED:
                color = glm::vec3(.3);
                break;
            case HOLDING:
                color = glm::vec3(.15);
                break;
            case RELEASED:
                color = glm::vec3(.8);
                break;
        }

        renderer->drawQuadID((uint64_t)this, _abs_pos, _abs_dim, glm::vec4(color, 1), options);

        if(isFocused()){
            options.layer = _layer + 0.01,
            renderer->drawQuad(_abs_pos, glm::vec2(_abs_dim.x * .02, _abs_dim.y), glm::vec4(.3, .3, .3, 1), options);
        }
    }

    if(!_text.empty()){
        Renderer::Options textOptions;
        textOptions.layer = _layer + 0.02,
        textOptions.shaderName = "font";
        renderer->drawTextID((uint64_t)this, _text, _abs_pos + _abs_dim.y * 0.2f, _abs_dim.y * 0.6f, textOptions);
    }
}

void Button::setTexture(const State state, std::string name) {
    _texture_names[state] = name;
}

void Button::setShader(const State state, std::string name) {
    _shader_names[state] = name;
}

void Button::setText(std::string text) {
    _text = text;
}

