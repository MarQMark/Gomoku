#include "presentation/ui/Label.h"

Label::Label(std::string name, std::string text) : IViewable(std::move(name)), _text(std::move(text)){
}

void Label::setText(std::string text) {
    _text = text;
}

std::string Label::getText() {
    return _text;
}

void Label::render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) {
    IViewable::render(renderer, parentPos, parentDim);

    renderer->drawTextID((uint64_t)this, _text, _abs_pos, _abs_dim.y, 9, 2);
}
