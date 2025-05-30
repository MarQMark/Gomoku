#ifndef GOMOKU_UI_H
#define GOMOKU_UI_H

#include "Renderer.h"
#include "View.h"
#include "IInteractable.h"

class UI {
public:
    UI(Renderer* renderer);

    void update();

    template<class T = IViewable>
    T* getViewable(std::string name) {
        if(name == "root" && std::is_same_v<View, T>)
            return _view_root;
        return _view_root->getViewable<T>(name);
    }


private:
    Renderer* _renderer{};
    View* _view_root;

    IInteractable* _focused{};
    View* _focused_view = nullptr;
    bool _prev_left_click = false;
    bool _enter_pressed = false;
    glm::vec2 _ui_mouse{};

    static void get_interactable(View* view, std::vector<IInteractable*> &interactables);
    void update_interactable(bool leftClick, std::vector<IInteractable*>& interactables);
    void focus_set(IInteractable* interactable, View* view);

    static View* get_view(IViewable *viewable, View *view);
    bool cursor_interactable_intersect(IInteractable* interactable) const;
    void focus_update(std::vector<IInteractable*>& interactables);
    void focus_first(std::vector<IInteractable*>& interactables);

    static bool is_view_visible(View* parent, View* view);
};


#endif //GOMOKU_UI_H
