#include "presentation/ui/MenuButton.h"
#include "presentation/assets/AssetManager.h"

void menuBtnPressedClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* btn = (MenuButton*)interactable;
    btn->setTextHeight(.25);
    btn->setTextPos(glm::vec2(0.1, 0.315));
}
void menuBtnReleasedClbk(IInteractable* interactable, IInteractable::State state, void* data) {
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
    Button::registerCallback(menuBtnPressedClbk, IInteractable::PRESSED);
    Button::registerCallback(menuBtnReleasedClbk, IInteractable::RELEASED);
    Button::registerCallback(menuBtnReleasedClbk, IInteractable::NONE);
}

