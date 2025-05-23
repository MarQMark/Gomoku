#include "presentation/ui/View.h"

#include <utility>

View::View(std::string name) : IViewable(std::move(name)) {
}

void View::addViewable(IViewable *viewable) {
    _viewables.push_back(viewable);
}

void View::render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) {
    for (auto* viewable : _viewables){
        viewable->render(renderer, parentPos + _pos / parentDim, parentDim * _dim);
    }
}

IViewable *View::getViewable(const std::string& name) {
    for(auto* viewable : _viewables){
        if(viewable->getName() == name)
            return viewable;

        if(View* view = dynamic_cast<View*>(viewable)){
            IViewable* retVal = view->getViewable(name);
            if(retVal)
                return retVal;
        }
    }

    return nullptr;
}


