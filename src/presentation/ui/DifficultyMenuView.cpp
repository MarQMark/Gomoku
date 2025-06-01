#include "presentation/ui/DifficultyMenuView.h"
#include "presentation/ui/MenuButton.h"
#include <utility>

void easyModeBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Game);
}

void mediumModeBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Game);
}

void hardModeBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Game);
}

void randomModeBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Game);
}

void backModeBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Mode);
}

DifficultyMenuView::DifficultyMenuView(std::string name, MenuController *menuController) : MenuView(std::move(name)){
    menuController->addMenuView(MenuController::Difficulty, this);

    MenuView::setVisible(false);
    MenuView::setHidden(true);
    MenuView::setDim(glm::vec2(.25, .7));
    MenuView::setMargin(0.15, 0, 0.01, 0);
    MenuView::setAlignH(IViewable::Align::TOP);

    auto* randomBtn = new MenuButton("randomDifficultyBtn");
    randomBtn->setText("Journalist");
    randomBtn->setAlignH(IViewable::Align::TOP);
    randomBtn->registerCallback(randomModeBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(randomBtn);

    auto* easyBtn = new MenuButton("easyDifficultyBtn", glm::vec2(0, .15 + 0.0625));
    easyBtn->setText("Easy");
    easyBtn->registerCallback(easyModeBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(easyBtn);

    auto* mediumBtn = new MenuButton("mediumBtn", glm::vec2(0, .3 + 0.125));
    mediumBtn->setText("Medium");
    mediumBtn->registerCallback(mediumModeBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(mediumBtn);

    auto* hardBtn = new MenuButton("hardDifficultyBtn", glm::vec2(0, .45 + 0.1875));
    hardBtn->setText("Hard");
    hardBtn->registerCallback(hardModeBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(hardBtn);

    auto* backModeBtn = new MenuButton("backModeBtn");
    backModeBtn->setText("Back");
    backModeBtn->setAlignH(IViewable::Align::BOTTOM);
    backModeBtn->registerCallback(backModeBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(backModeBtn);
}

DifficultyMenuView::~DifficultyMenuView() {

}