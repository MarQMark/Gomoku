#include "presentation/ui/View.h"

void View::addView(View *view) {
    _views.push_back(view);
}

void View::addViewable(IViewable *viewable) {
    _viewables.push_back(viewable);
}

void View::update() {
    for (auto* viewable : _viewables)
        viewable->render();

    for (auto* view : _views)
        view->update();
}

