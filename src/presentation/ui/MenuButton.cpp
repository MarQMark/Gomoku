#include "presentation/ui/MenuButton.h"
#include "presentation/assets/AssetManager.h"

MenuButton::MenuButton(std::string name, glm::vec2 pos) : Button(std::move(name)) {
    Button::setPos(pos);
    Button::setDim(glm::vec2(.95, .15));
    Button::setTextHeight(.3);
    Button::setTextPos(glm::vec2(0.08, 0.3));
    Button::setAlignV(IViewable::Align::RIGHT);
    Button::setTexture(IInteractable::NONE, AssetManager::getName(Textures::button_none));
    Button::setTexture(IInteractable::HOVERING, AssetManager::getName(Textures::button_hover));
    Button::setTexture(IInteractable::PRESSED, AssetManager::getName(Textures::button_pressed));
    Button::setTexture(IInteractable::HOLDING, AssetManager::getName(Textures::button_pressed));
    Button::setTexture(IInteractable::RELEASED, AssetManager::getName(Textures::button_pressed));
}

