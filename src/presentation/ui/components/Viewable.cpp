#include "presentation/ui/components/IViewable.h"

void IViewable::render(Renderer* renderer, const glm::vec2 parentPos, const glm::vec2 parentDim){
    _abs_pos = parentPos + _pos * parentDim;
    _abs_dim = parentDim * _dim;

    const glm::vec2 res = renderer->getViewportSize();
    if(_keep_width)
        _abs_dim.y *= (res.x / res.y);
    if(_keep_height)
        _abs_dim.x *= (res.y / res.x);

    if(_vertical == CENTER)
        _abs_pos.x = parentPos.x + (parentDim.x / 2) - (_abs_dim.x / 2);
    else if(_vertical == LEFT)
        _abs_pos.x = parentPos.x + _margin.z;
    else if(_vertical == RIGHT)
        _abs_pos.x = parentPos.x + parentDim.x - _abs_dim.x - _margin.w;

    if(_horizontal == CENTER)
        _abs_pos.y = parentPos.y + (parentDim.y / 2) - (_abs_dim.y / 2);
    else if(_horizontal == TOP)
        _abs_pos.y = parentPos.y + _margin.x;
    else if(_horizontal == BOTTOM)
        _abs_pos.y = parentPos.y + parentDim.y - _abs_dim.y - _margin.y;
}