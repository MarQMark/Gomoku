#include "presentation/ui/DifficultyMenuView.h"
#include "presentation/ui/MenuButton.h"
#include <utility>

#include "logic/interfaces/IGameService.h"
#include "presentation/mapping/MapViewToCommand.h"

void easy_mode_btn_clbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Game);
    menuController->getGameService()->startGame(MapViewToCommand::toGameSetupCommandDTO(menuController->getSelectedGameMode(), BASIC));
}

void medium_mode_btn_clbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Game);
    menuController->getGameService()->startGame(MapViewToCommand::toGameSetupCommandDTO(menuController->getSelectedGameMode(), GOOD));
}

void hard_mode_btn_clbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Game);
    menuController->getGameService()->startGame(MapViewToCommand::toGameSetupCommandDTO(menuController->getSelectedGameMode(), EXPERT));
}

void random_mode_btn_clbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Game);
    menuController->getGameService()->startGame(MapViewToCommand::toGameSetupCommandDTO(menuController->getSelectedGameMode(), RANDOM));
}

void back_mode_btn_clbk(IInteractable* interactable, IInteractable::State state, void* data) {
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
    randomBtn->registerCallback(random_mode_btn_clbk, IInteractable::State::RELEASED, menuController);
    MenuView::addViewable(randomBtn);

    auto* easyBtn = new MenuButton("easyDifficultyBtn", glm::vec2(0, .15 + 0.0625));
    easyBtn->setText("Easy");
    easyBtn->registerCallback(easy_mode_btn_clbk, IInteractable::State::RELEASED, menuController);
    MenuView::addViewable(easyBtn);

    auto* mediumBtn = new MenuButton("mediumDifficultyBtn", glm::vec2(0, .3 + 0.125));
    mediumBtn->setText("Medium");
    mediumBtn->registerCallback(medium_mode_btn_clbk, IInteractable::State::RELEASED, menuController);
    MenuView::addViewable(mediumBtn);

    auto* hardBtn = new MenuButton("hardDifficultyBtn", glm::vec2(0, .45 + 0.1875));
    hardBtn->setText("Hard");
    hardBtn->registerCallback(hard_mode_btn_clbk, IInteractable::State::RELEASED, menuController);
    MenuView::addViewable(hardBtn);

    auto* backModeBtn = new MenuButton("backModeBtn");
    backModeBtn->setText("Back");
    backModeBtn->setAlignH(IViewable::Align::BOTTOM);
    backModeBtn->registerCallback(back_mode_btn_clbk, IInteractable::State::RELEASED, menuController);
    MenuView::addViewable(backModeBtn);
}

DifficultyMenuView::~DifficultyMenuView() {
    deleteViewable<MenuButton>("randomDifficultyBtn");
    deleteViewable<MenuButton>("easyDifficultyBtn");
    deleteViewable<MenuButton>("mediumDifficultyBtn");
    deleteViewable<MenuButton>("hardDifficultyBtn");
    deleteViewable<MenuButton>("backModeBtn");
}