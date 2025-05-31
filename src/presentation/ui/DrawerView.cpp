#include "presentation/ui/DrawerView.h"
#include <utility>

void DrawerViewClbk(IInteractable* interactable, IInteractable::State state, void* data){
    auto* drawerView = (DrawerView*)data;
    if(state == IInteractable::State::HOVERING){
        drawerView->setExpanded(true);
    }
    else if (state == IInteractable::State::NONE){
        drawerView->setExpanded(false);
    }
}

DrawerView::DrawerView(std::string name) : View(std::move(name)) {
    init();
}

DrawerView::DrawerView(std::string name, glm::vec2 pos, glm::vec2 dim) : View(std::move(name), pos, dim) {
    init();
}

void DrawerView::init() {
    _nav_toggle = new DrawerViewInteractable(getName() + "_Button");
    _nav_toggle->registerCallback(DrawerViewClbk, IInteractable::State::ALL, this);
    _nav_toggle_pos = glm::vec2(0.1, 0);
    _nav_toggle_dim = glm::vec2(.5 * _dim.y, .5 * _dim.x);
    set_interactable_small();
    View::addViewable(_nav_toggle);
}

DrawerView::~DrawerView() {
    delete _nav_toggle;
}

void DrawerView::render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) {
    View::render(renderer, parentPos, parentDim);

    double dt = glfwGetTime() - _last_time;
    _last_time = glfwGetTime();
    if(_expanded && _curr_time != _duration){
        _curr_time += dt;
        _curr_time = std::min(_curr_time, _duration);
    }
    else if(!_expanded && _curr_time > 0){
        _curr_time -= dt;
        _curr_time = std::max(_curr_time, 0.);
    }

    //update position
    for (auto* viewable : _viewables)
        viewable->setPos(_original_positions[viewable->getName()] - glm::vec2(_max_pos * (1 - (_curr_time / _duration)), 0));

    update_visibility();
}

void DrawerView::addViewable(IViewable *viewable) {
    View::addViewable(viewable);
    _viewables.push_back(viewable);
    _original_positions[viewable->getName()] = viewable->getPos();

    // get most right viewable
    for (auto* viewable2 : _viewables){
        if(viewable2->getPos().x + viewable2->getDim().x > _max_pos){
            _max_pos = viewable2->getPos().x + viewable2->getDim().x;
        }
    }

    update_visibility();
}

void DrawerView::setExpanded(bool expanded) {
    if(_expanded != expanded)
        _last_time = glfwGetTime();

    _expanded = expanded;
    if(_expanded)
        set_interactable_big();
    else
        set_interactable_small();

    update_visibility();
}

void DrawerView::update_visibility() {
    for (auto* viewable : _viewables)
        viewable->setVisible(!(!_expanded && _curr_time == 0));
}

void DrawerView::set_interactable_small() {
    _nav_toggle->setPos(_nav_toggle_pos);
    _nav_toggle->setDim(_nav_toggle_dim);
    _nav_toggle->setKeepWidth(true);
    _nav_toggle->setAlignH(Align::CENTER);
    _nav_toggle->setHidden(false);
}

void DrawerView::set_interactable_big() {
    _nav_toggle->setPos(glm::vec2(0));
    _nav_toggle->setDim(glm::vec2(1));
    _nav_toggle->setKeepWidth(false);
    _nav_toggle->setHidden(true);
}

void DrawerView::setNavToggleLayer(float layer) {
    _nav_toggle->setLayer(layer);
}

void DrawerView::setNavToggleTexture(std::string texture) {
    _nav_toggle->setTexture(texture);
}

void DrawerView::setNavToggleShader(std::string shader) {
    _nav_toggle->setShader(shader);
}

void DrawerView::setNavTogglePos(glm::vec2 pos) {
    _nav_toggle_pos = pos;
}

void DrawerView::setNavToggleDim(glm::vec2 dim) {
    _nav_toggle_dim = dim;
}


DrawerView::DrawerViewInteractable::DrawerViewInteractable(std::string name) : IInteractable(name) {
}

DrawerView::DrawerViewInteractable::DrawerViewInteractable(std::string name, glm::vec2 pos, glm::vec2 dim)
    : IInteractable(name, pos, dim) {
}

void DrawerView::DrawerViewInteractable::render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) {
    IInteractable::render(renderer, parentPos, parentDim);

    if(!_hidden){
        Renderer::Options options;
        options.layer = _layer;
        if(!_shader.empty())
            options.shaderName = _shader;
        if(!_texture.empty())
            renderer->drawTextureID((uint64_t)this, _abs_pos, _abs_dim, renderer->getTexture(_texture));
        else
            renderer->drawQuadID((uint64_t)this, _abs_pos, _abs_dim);
    }
}

void DrawerView::DrawerViewInteractable::setHidden(bool hidden) {
    _hidden = hidden;
}

void DrawerView::DrawerViewInteractable::setTexture(std::string texture) {
    _texture = texture;
}

void DrawerView::DrawerViewInteractable::setShader(std::string shader) {
    _shader = shader;
}
