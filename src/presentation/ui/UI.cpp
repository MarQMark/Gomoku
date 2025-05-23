#include "presentation/ui/UI.h"
#include "presentation/ui/IInteractable.h"

UI::UI(Renderer *renderer) : _renderer(renderer) {
    _view_root = new View("root");
}

void UI::update() {
    glm::vec2 dim = _renderer->getViewportSize();

    // TODO: Add interactable update code

    _view_root->render(_renderer, glm::vec2(0), dim);
}
