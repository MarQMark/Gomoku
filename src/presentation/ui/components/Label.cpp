#include "presentation/ui/components/Label.h"

Label::Label(std::string name, std::string text) : IViewable(std::move(name)), _text(std::move(text)){
    _layer = 6;
}

void Label::setText(std::string text) {
    _text = text;
}

std::string Label::getText() {
    return _text;
}

void Label::render(Renderer *renderer, const glm::vec2 parentPos, const glm::vec2 parentDim) {
    IViewable::render(renderer, parentPos, parentDim);
    if(!isVisible())
        return;

    Renderer::Options options;
    options.layer = _layer;
    options.shaderName = "font";
    renderer->drawTextID((uint64_t)this, _text, _abs_pos, _abs_dim.y, options);
}
