#include <iostream>
#include "presentation/ui/components/MenuView.h"
#include "presentation/ui/components/IInteractable.h"
#include "common/Time.h"

MenuView::MenuView(std::string name) : View(name) {
}

MenuView::MenuView(std::string name, glm::vec2 pos, glm::vec2 dim) : View(name, pos, dim) {
    _original_pos = pos;
}

void MenuView::render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) {
    View::render(renderer, parentPos, parentDim);

    if(!_hidden && _curr_time != _duration){
        _curr_time += Time::get()->getDeltaTime();
        _curr_time = std::min(_curr_time, _duration);
        set_interactable(false);
    }
    else if(_hidden && _curr_time > 0){
        _curr_time -= Time::get()->getDeltaTime();
        _curr_time = std::max(_curr_time, 0.);
        set_interactable(false);
    }
    else{
        set_interactable(true);
    }

    View::setPos(_original_pos - glm::vec2(_original_pos.x + _dim.x * (1 - (_curr_time / _duration)), 0));
    setVisible(!(_hidden && _curr_time == 0));
}

void MenuView::setHidden(bool hidden) {
    _hidden = hidden;
}

void MenuView::setPos(glm::vec2 pos) {
    View ::setPos(pos);
    _original_pos = pos;
}

bool MenuView::isHidden() {
    return _hidden;
}

void MenuView::set_interactable(bool isInteractable) {
    std::vector<IViewable*> viewables;
    getChildViewables(viewables);
    for(auto* viewable : viewables){
        auto* interactable = dynamic_cast<IInteractable*>(viewable);
        if(interactable){
            interactable->setInteractable(isInteractable);
        }
    }
}

bool MenuView::inMotion() {
    if(!_hidden && _curr_time != _duration)
        return true;

    if(_hidden && _curr_time > 0)
        return true;

    return false;
}
