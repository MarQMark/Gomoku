#include "presentation/ui/ModeMenuView.h"
#include "presentation/ui/MenuButton.h"
#include <utility>

#include "logic/interfaces/IGameService.h"
#include "presentation/mapping/MapViewToCommand.h"

void hvh_btn_clbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Game);
    menuController->setSelectedGameMode(HUMAN_VS_HUMAN);
    menuController->getGameService()->startGame(MapViewToCommand::toGameSetupCommandDTO(menuController->getSelectedGameMode(), RANDOM));
}

void hvai_btn_clbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Difficulty);
    menuController->setSelectedGameMode(HUMAN_VS_AI);
}

void aivh_btn_clbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Difficulty);
    menuController->setSelectedGameMode(AI_VS_HUMAN);
}

void aivai_btn_clbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Difficulty);
    menuController->setSelectedGameMode(AI_VS_AI);
}

void back_main_btn_clbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Main);
}

ModeMenuView::ModeMenuView(std::string name, MenuController* menuController) : MenuView(std::move(name)) {
    menuController->addMenuView(MenuController::Mode, this);

    MenuView::setVisible(false);
    MenuView::setHidden(true);
    MenuView::setDim(glm::vec2(.25, .7));
    MenuView::setMargin(0.15, 0, 0.01, 0);
    MenuView::setAlignH(IViewable::Align::TOP);

    auto* hvhBtn = new MenuButton("hvhBtn");
    hvhBtn->setText("Player VS Player");
    hvhBtn->setAlignH(IViewable::Align::TOP);
    hvhBtn->registerCallback(hvh_btn_clbk, IInteractable::State::RELEASED, menuController);
    MenuView::addViewable(hvhBtn);

    auto* hvaiBtn = new MenuButton("hvaiBtn", glm::vec2(0, .15 + 0.0625));
    hvaiBtn->setText("Player VS AI");
    hvaiBtn->registerCallback(hvai_btn_clbk, IInteractable::State::RELEASED, menuController);
    MenuView::addViewable(hvaiBtn);

    auto* aivhBtn = new MenuButton("aivhBtn", glm::vec2(0, .3 + 0.125));
    aivhBtn->setText("AI VS Player");
    aivhBtn->registerCallback(aivh_btn_clbk, IInteractable::State::RELEASED, menuController);
    MenuView::addViewable(aivhBtn);

    auto* aivaiBtn = new MenuButton("aivaiBtn", glm::vec2(0, .45 + 0.1875));
    aivaiBtn->setText("AI VS AI");
    aivaiBtn->registerCallback(aivai_btn_clbk, IInteractable::State::RELEASED, menuController);
    MenuView::addViewable(aivaiBtn);

    auto* backMainBtn = new MenuButton("backMainBtn");
    backMainBtn->setText("Back");
    backMainBtn->setAlignH(IViewable::Align::BOTTOM);
    backMainBtn->registerCallback(back_main_btn_clbk, IInteractable::State::RELEASED, menuController);
    MenuView::addViewable(backMainBtn);
}

ModeMenuView::~ModeMenuView() {
    deleteViewable<MenuButton>("hvhBtn");
    deleteViewable<MenuButton>("hvaiBtn");
    deleteViewable<MenuButton>("aivhBtn");
    deleteViewable<MenuButton>("aivaiBtn");
    deleteViewable<MenuButton>("backMainBtn");
}
