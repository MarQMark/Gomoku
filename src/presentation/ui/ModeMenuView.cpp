#include "presentation/ui/ModeMenuView.h"
#include "presentation/ui/components/Button.h"

ModeMenuView::ModeMenuView(std::string name) : MenuView(name) {
    MenuView::setVisible(false);
    MenuView::setHidden(true);
    MenuView::setDim(glm::vec2(.25, .7));
    MenuView::setMargin(glm::vec4(0.15, 0, 0.01, 0));
    MenuView::setAlignH(IViewable::Align::TOP);

    auto* hvhBtn = new Button("hvhBtn", glm::vec2(), glm::vec2(.95, .15));
    hvhBtn->setText("Human vs Human");
    hvhBtn->setTextHeight(.35);
    hvhBtn->setAlignV(IViewable::Align::RIGHT);
    hvhBtn->setAlignH(IViewable::Align::TOP);
    //hvhBtn->registerCallback(newGameBtnClbk, IInteractable::State::PRESSED, this);
    MenuView::addViewable(hvhBtn);

    auto* hvaiBtn = new Button("hvaiBtn", glm::vec2(0, .15 + 0.0625), glm::vec2(.95, .15));
    hvaiBtn->setText("Human vs AI");
    hvaiBtn->setTextHeight(.35);
    hvaiBtn->setAlignV(IViewable::Align::RIGHT);
    //hvaiBtn->registerCallback(continueBtnClbk, IInteractable::State::PRESSED, this);
    MenuView::addViewable(hvaiBtn);

    auto* aivhBtn = new Button("aivhBtn", glm::vec2(0, .3 + 0.125), glm::vec2(.95, .15));
    aivhBtn->setText("AI vs Human");
    aivhBtn->setTextHeight(.35);
    aivhBtn->setAlignV(IViewable::Align::RIGHT);
    //aivhBtn->registerCallback(exitBtnClbk, IInteractable::State::PRESSED, this);
    MenuView::addViewable(aivhBtn);

    auto* aivaiBtn = new Button("aivaiBtn", glm::vec2(0, .45 + 0.1875), glm::vec2(.95, .15));
    aivaiBtn->setText("AI vs AI");
    aivaiBtn->setTextHeight(.35);
    aivaiBtn->setAlignV(IViewable::Align::RIGHT);
    //aivaiBtn->registerCallback(exitBtnClbk, IInteractable::State::PRESSED, this);
    MenuView::addViewable(aivaiBtn);

    auto* backMainBtn = new Button("backMainBtn", glm::vec2(), glm::vec2(.95, .15));
    backMainBtn->setText("AI vs AI");
    backMainBtn->setTextHeight(.35);
    backMainBtn->setAlignV(IViewable::Align::RIGHT);
    backMainBtn->setAlignH(IViewable::Align::BOTTOM);
    //backMainBtn->registerCallback(exitBtnClbk, IInteractable::State::PRESSED, this);
    MenuView::addViewable(backMainBtn);
}

ModeMenuView::~ModeMenuView() {
    delete MenuView::getViewable<Button>("hvhBtn");
    delete MenuView::getViewable<Button>("hvaiBtn");
    delete MenuView::getViewable<Button>("aivhBtn");
    delete MenuView::getViewable<Button>("aivaiBtn");
    delete MenuView::getViewable<Button>("backMainBtn");
}
