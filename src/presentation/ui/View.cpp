#include "presentation/ui/View.h"

#include <utility>
#include <iostream>

View::View(std::string name) : IViewable(std::move(name)) {
}

void View::addViewable(IViewable *viewable) {
    _viewables.push_back(viewable);
}

void View::render(Renderer* renderer, const glm::vec2 parentPos, const glm::vec2 parentDim) {
    IViewable::render(renderer, parentPos, parentDim);
    if(!isVisible())
        return;

    for (auto* viewable : _viewables){
        viewable->render(renderer, _abs_pos, _abs_dim);
    }
}

std::vector<IViewable *> View::getViewables() {
    return _viewables;
}
