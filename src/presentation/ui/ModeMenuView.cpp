#include "presentation/ui/ModeMenuView.h"
#include "presentation/ui/MenuButton.h"
#include <utility>

void hvhBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Difficulty);
}

void hvaiBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Difficulty);
}

void aivhBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Difficulty);
}

void aivaiBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuController = (MenuController*)data;
    menuController->changeMenu(MenuController::Difficulty);
}

void backMainBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
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
    hvhBtn->registerCallback(hvhBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(hvhBtn);

    auto* hvaiBtn = new MenuButton("hvaiBtn", glm::vec2(0, .15 + 0.0625));
    hvaiBtn->setText("Player VS AI");
    hvaiBtn->registerCallback(hvaiBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(hvaiBtn);

    auto* aivhBtn = new MenuButton("aivhBtn", glm::vec2(0, .3 + 0.125));
    aivhBtn->setText("AI VS Player");
    aivhBtn->registerCallback(aivhBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(aivhBtn);

    auto* aivaiBtn = new MenuButton("aivaiBtn", glm::vec2(0, .45 + 0.1875));
    aivaiBtn->setText("AI VS AI");
    aivaiBtn->registerCallback(aivaiBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(aivaiBtn);

    auto* backMainBtn = new MenuButton("backMainBtn");
    backMainBtn->setText("Back");
    backMainBtn->setAlignH(IViewable::Align::BOTTOM);
    backMainBtn->registerCallback(backMainBtnClbk, IInteractable::State::PRESSED, menuController);
    MenuView::addViewable(backMainBtn);
}

ModeMenuView::~ModeMenuView() {
    deleteViewable<MenuButton>("hvhBtn");
    deleteViewable<MenuButton>("hvaiBtn");
    deleteViewable<MenuButton>("aivhBtn");
    deleteViewable<MenuButton>("aivaiBtn");
    deleteViewable<MenuButton>("backMainBtn");
}
