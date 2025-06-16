#include "presentation/ui/MenuButton.h"
#include "presentation/assets/AssetManager.h"

void menu_btn_pressed_clbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* btn = (MenuButton*)interactable;
    btn->setTextHeight(.25);
    btn->setTextPos(glm::vec2(0.1, 0.315));
}
void menu_btn_released_clbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* btn = (MenuButton*)interactable;
    btn->setTextHeight(.28);
    btn->setTextPos(glm::vec2(0.1, 0.3));
}


MenuButton::MenuButton(std::string name, glm::vec2 pos) : Button(std::move(name)) {
    Button::setPos(pos);
    Button::setDim(glm::vec2(.95, .15));
    Button::setTextHeight(.28);
    Button::setTextPos(glm::vec2(0.1, 0.3));
    Button::setAlignV(IViewable::Align::RIGHT);
    Button::setTexture(IInteractable::NONE, AssetManager::getName(Textures::button_none));
    Button::setTexture(IInteractable::HOVERING, AssetManager::getName(Textures::button_hover));
    Button::setTexture(IInteractable::PRESSED, AssetManager::getName(Textures::button_pressed));
    Button::setTexture(IInteractable::HOLDING, AssetManager::getName(Textures::button_pressed));
    Button::setTexture(IInteractable::RELEASED, AssetManager::getName(Textures::button_pressed));
    Button::registerCallback(menu_btn_pressed_clbk, IInteractable::PRESSED);
    Button::registerCallback(menu_btn_released_clbk, IInteractable::RELEASED);
    Button::registerCallback(menu_btn_released_clbk, IInteractable::NONE);
}

void MenuButton::render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) {
    Button::render(renderer, parentPos, parentDim);

    if(isFocused() && getState() == IInteractable::NONE){
        Renderer::Options options;
        options.layer = _layer + 0.01,
        renderer->drawTexture(_abs_pos, _abs_dim, renderer->getTexture(AssetManager::getName(Textures::button_select)), options);
    }

    if(!_text.empty()){
        Renderer::Options textOptions;
        textOptions.layer = _layer + 0.015,
        textOptions.shaderName = "font";
        glm::vec2 pos = _abs_pos + glm::vec2(_abs_dim.y * _text_pos.x, _abs_dim.y * _text_pos.y);
        pos.y += _abs_dim.y * _text_height * .15;

        Font::Options fontOptions;
        fontOptions.font = renderer->getFont("default");
        fontOptions.color = glm::vec4(0.082, 0.114, 0.157, 1);
        renderer->drawTextID((uint64_t)this + 1, _text, pos, _abs_dim.y * _text_height, textOptions, fontOptions);
    }
}

