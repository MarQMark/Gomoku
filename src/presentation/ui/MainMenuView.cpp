#include "presentation/ui/MainMenuView.h"
#include "presentation/ui/MenuButton.h"
#include <utility>

void newGameBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Mode);
}

void continueBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Game);
}

void exitBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    exit(0);
}

MainMenuView::MainMenuView(std::string name, MenuController* menuController) : MenuView(std::move(name)) {
    menuController->addMenuView(MenuController::Main, this);

    MenuView::setDim(glm::vec2(.25, .7));
    MenuView::setMargin(0.15, 0, 0.01, 0);
    MenuView::setAlignH(IViewable::Align::TOP);

    auto* newGameBtn = new MenuButton("newGameBtn");
    newGameBtn->setText("New Game");
    newGameBtn->setAlignH(IViewable::Align::TOP);
    newGameBtn->setMargin(0.15, 0, 0, 0);
    newGameBtn->registerCallback(newGameBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(newGameBtn);

    auto* continueBtn = new MenuButton("continueBtn");
    continueBtn->setText("Continue");
    continueBtn->setAlignH(IViewable::Align::CENTER);
    continueBtn->registerCallback(continueBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(continueBtn);

    auto* exitBtn = new MenuButton("exitBtn");
    exitBtn->setText("Exit");
    exitBtn->setAlignH(IViewable::Align::BOTTOM);
    exitBtn->setMargin(0, 0.15, 0, 0);
    exitBtn->registerCallback(exitBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(exitBtn);
}

MainMenuView::~MainMenuView() {
    deleteViewable<MenuButton>("newGameBtn");
    deleteViewable<MenuButton>("continueBtn");
    deleteViewable<MenuButton>("exitBtn");
}
