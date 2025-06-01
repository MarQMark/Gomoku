#include "presentation/ui/MainMenuView.h"
#include "presentation/ui/components/Button.h"

void newGameBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuView = (MainMenuView*)data;
    menuView->setHidden(!menuView->isHidden());
}

void continueBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuView = (MainMenuView*)data;
    menuView->setHidden(!menuView->isHidden());
}

void exitBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    exit(0);
}

MainMenuView::MainMenuView(std::string name) : MenuView(name) {
    MenuView::setDim(glm::vec2(.25, .7));
    MenuView::setMargin(glm::vec4(0.15, 0, 0.01, 0));
    MenuView::setAlignH(IViewable::Align::TOP);

    auto* newGameBtn = new Button("newGameBtn", glm::vec2(), glm::vec2(.95, .2));
    newGameBtn->setText("New Game");
    newGameBtn->setTextHeight(.35);
    newGameBtn->setAlignV(IViewable::Align::RIGHT);
    newGameBtn->setAlignH(IViewable::Align::TOP);
    newGameBtn->registerCallback(newGameBtnClbk, IInteractable::State::PRESSED, this);
    MenuView::addViewable(newGameBtn);

    auto* continueBtn = new Button("continueBtn", glm::vec2(), glm::vec2(.95, .2));
    continueBtn->setText("Continue");
    continueBtn->setTextHeight(.35);
    continueBtn->setAlignV(IViewable::Align::RIGHT);
    continueBtn->setAlignH(IViewable::Align::CENTER);
    continueBtn->registerCallback(continueBtnClbk, IInteractable::State::PRESSED, this);
    MenuView::addViewable(continueBtn);

    auto* exitBtn = new Button("exitBtn", glm::vec2(), glm::vec2(.95, .2));
    exitBtn->setText("Exit");
    exitBtn->setTextHeight(.35);
    exitBtn->setAlignV(IViewable::Align::RIGHT);
    exitBtn->setAlignH(IViewable::Align::BOTTOM);
    exitBtn->registerCallback(exitBtnClbk, IInteractable::State::PRESSED, this);
    MenuView::addViewable(exitBtn);
}

MainMenuView::~MainMenuView() {
    delete MenuView::getViewable<Button>("newGameBtn");
    delete MenuView::getViewable<Button>("continueBtn");
    delete MenuView::getViewable<Button>("exitBtn");
}
