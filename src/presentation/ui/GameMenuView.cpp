#include "presentation/ui/GameMenuView.h"
#include "presentation/ui/MenuButton.h"
#include <utility>

#include "presentation/assets/AssetManager.h"
#include "common/RunManager.h"
#include "logic/interfaces/IGameService.h"

void pauseBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    const auto* menuController = (MenuController*)data;
    const GameStatus gameStatus = menuController->getGameService()->changeGameStatus();
    if (gameStatus == PAUSED)
        ((MenuButton*)interactable)->setText("Resume");

    if (gameStatus == IN_PROGRESS)
        ((MenuButton*)interactable)->setText("Pause");
}

void mainMenuBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Main);
}

void exitGameBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    RunManager::get()->stop();
}

GameMenuView::GameMenuView(std::string name, MenuController *menuController) : MenuView(std::move(name)) {
    menuController->addMenuView(MenuController::Game, this);

    MenuView::setVisible(false);
    MenuView::setHidden(true);
    MenuView::setDim(glm::vec2(.25, .7));
    MenuView::setMargin(0.15, 0, 0.01, 0);
    MenuView::setAlignH(IViewable::Align::TOP);

    _drawer_view = new DrawerView("drawerView", glm::vec2(0), glm::vec2(1));
    _drawer_view->setAlignV(IViewable::Align::LEFT);
    _drawer_view->setAlignH(IViewable::Align::TOP);
    _drawer_view->setExpanded(false);
    _drawer_view->setNavToggleTexture(AssetManager::getName(Textures::nav_toggle));
    _drawer_view->setNavToggleDim(glm::vec2(1.0/3.0, 0.5/3.0));
    MenuView::addViewable(_drawer_view);

    auto* pauseBtn = new MenuButton("pauseBtn", glm::vec2(.05, 0));
    pauseBtn->setText("Pause");
    pauseBtn->setAlignH(IViewable::Align::TOP);
    pauseBtn->setAlignV(IViewable::Align::NONE);
    pauseBtn->setMargin(0.15, 0, 0, 0);
    pauseBtn->registerCallback(pauseBtnClbk, IInteractable::RELEASED, menuController);
    _drawer_view->addViewable(pauseBtn);
    auto* mainMenuBtn = new MenuButton("mainMenuBtn", glm::vec2(.05, .25));
    mainMenuBtn->setText("Main Menu");
    mainMenuBtn->setAlignH(IViewable::Align::CENTER);
    mainMenuBtn->setAlignV(IViewable::Align::NONE);
    mainMenuBtn->registerCallback(mainMenuBtnClbk, IInteractable::RELEASED, menuController);
    _drawer_view->addViewable(mainMenuBtn);
    auto* exitGameBtn = new MenuButton("exitGameBtn", glm::vec2(.05, .75));
    exitGameBtn->setText("Exit");
    exitGameBtn->setMargin(0, 0.15, 0, 0);
    exitGameBtn->setAlignH(IViewable::Align::BOTTOM);
    exitGameBtn->setAlignV(IViewable::Align::NONE);
    exitGameBtn->registerCallback(exitGameBtnClbk, IInteractable::RELEASED, menuController);
    _drawer_view->addViewable(exitGameBtn);
}

GameMenuView::~GameMenuView() {
    deleteViewable<MenuButton>("pauseBtn");
    deleteViewable<MenuButton>("mainMenuBtn");
    deleteViewable<MenuButton>("exitGameBtn");
    deleteViewable<MenuButton>("drawerView");
}

void GameMenuView::setHidden(bool hidden) {
    MenuView::setHidden(hidden);
    _drawer_view->setExpanded(false);
}
