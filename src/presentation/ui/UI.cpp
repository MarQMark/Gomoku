#include "presentation/ui/UI.h"
#include "presentation/input/Input.h"

UI::UI(Renderer *renderer) : _renderer(renderer) {
    _view_root = new View("root");
}

UI::~UI() {
    delete _view_root;
}

void UI::update() {
    if(!_view_root->isVisible())
        return;

    const glm::vec2 dim = _renderer->getViewportSize();
    _ui_mouse = _renderer->getCursorPos() / dim;

    const bool leftClick = Input::get()->mousePressed(BUTTON_LEFT);

    std::vector<IInteractable*> interactables;
    get_interactable(_view_root, interactables);

    update_interactable(leftClick, interactables);

    focus_update(interactables);

    _prev_left_click = leftClick;

    _view_root->render(_renderer, glm::vec2(0), glm::vec2(1));
}

void UI::get_interactable(View* view, std::vector<IInteractable *> &interactables) {
    for(auto* viewable : view->getViewables()){
        if(!viewable->isVisible())
            continue;

        auto* childView = dynamic_cast<View*>(viewable);
        if(childView && childView->isVisible())
            get_interactable(childView, interactables);

        auto* interactable = dynamic_cast<IInteractable*>(viewable);
        if(!interactable)
            continue;

        if(!interactable->isInteractable())
            continue;

        interactables.push_back(interactable);
    }
}

void UI::update_interactable(const bool leftClick, std::vector<IInteractable*>& interactables) {
    for(auto* interactable : interactables){

        // Prevent being able to interact with twice (keyboard && mouse)
        if(_focused == interactable && _enter_pressed)
            continue;

        if(cursor_interactable_intersect(interactable)){

            if(leftClick && _prev_left_click) {
                interactable->changeState(IInteractable::State::HOLDING);
            }
            else if(leftClick) {
                interactable->changeState(IInteractable::State::PRESSED);
                focus_set(interactable, get_view(interactable, _view_root));
            }
            else if(_prev_left_click){
                interactable->changeState(IInteractable::State::RELEASED);
            }
            else{
                interactable->changeState(IInteractable::State::HOVERING);
            }

        }
        else{
            interactable->changeState(IInteractable::State::NONE);
        }
    }
}

void UI::focus_set(IInteractable *interactable, View* view) {
    if(_focused)
        _focused->setFocused(false);
    if(interactable && interactable->isInteractable())
        interactable->setFocused(true);
    _focused = interactable;
    _focused_view = view;
}

View *UI::get_view(IViewable *viewable, View *view) {
    if(!view)
        return nullptr;

    for (auto* e : view->getViewables()) {
        if(e == viewable)
            return view;

        auto* childView = dynamic_cast<View*>(e);
        if(childView && childView->isVisible()){
            const auto retVal = get_view(viewable, childView);
            if(retVal)
                return retVal;
        }
    }

    return nullptr;
}

bool UI::cursor_interactable_intersect(IInteractable *interactable) const {
    return (_ui_mouse.x > interactable->getAbsPos().x && _ui_mouse.x < interactable->getAbsPos().x + interactable->getAbsDim().x &&
            _ui_mouse.y > interactable->getAbsPos().y && _ui_mouse.y < interactable->getAbsPos().y + interactable->getAbsDim().y);
}

void UI::focus_update(std::vector<IInteractable*>& interactables) {
    if(_focused){
        if(!_focused->isVisible()){
            _focused->setFocused(false);
            _focused = nullptr;
            _focused_view = nullptr;
        }
        else if(!_focused_view){
            std::cout << "[WARNING] UI interactable selected but no View. This should not happen\n";
        }
        else if(!is_view_visible(_view_root, _focused_view)){
            focus_first(interactables);
        }
        else if(_focused->isInteractable()){
            if(Input::get()->keyPressed(ENTER) && _enter_pressed){
                _focused->changeState(IInteractable::State::HOLDING);
            }
            else if(Input::get()->keyPressed(ENTER)){
                _focused->changeState(IInteractable::State::PRESSED);
            }
            else if(_enter_pressed){
                _focused->changeState(IInteractable::State::RELEASED);
            }
        }
    }

    if(Input::get()->keyXPressed(TAB)){
        if(!_focused_view)
            _focused_view = _view_root;
        if(!_focused)
            focus_first(interactables);
        else{
            unsigned int i = 0;
            bool inList = false;
            for (; i < interactables.size(); ++i) {
                if(interactables[i] == _focused){
                    inList = true;
                    break;
                }
            }

            if(Input::get()->keyPressed(LEFT_SHIFT) || Input::get()->keyPressed(RIGHT_SHIFT))
                i == 0 ? i = interactables.size() - 1 : i--;
            else
                i = (i + 1) % interactables.size();

            if(!inList)
                focus_first(interactables);
            else
                focus_set(interactables[i], get_view(interactables[i], _view_root));
        }
    }

    _enter_pressed = Input::get()->keyPressed(ENTER);
}

void UI::focus_first(std::vector<IInteractable*>& interactables) {
    if(interactables.empty()){
        focus_set(nullptr, nullptr);
        return;
    }

    focus_set(interactables[0], get_view(interactables[0], _view_root));
}

bool UI::is_view_visible(View* parent, View *view) {
    if(parent == view)
        return view->isVisible();

    for (auto* viewable : parent->getViewables()) {
        if(View* childView = dynamic_cast<View*>(viewable)){
            if(is_view_visible(childView, view))
                return true;
        }
    }

    return false;
}

void UI::addViewable(IViewable *viewable) {
    _view_root->addViewable(viewable);
}
