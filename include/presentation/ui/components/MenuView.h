#ifndef GOMOKU_MENUVIEW_H
#define GOMOKU_MENUVIEW_H


#include "presentation/ui/components/View.h"

class MenuView : public View{
public:
    MenuView(std::string name);
    MenuView(std::string name, glm::vec2 pos, glm::vec2 dim);

    void render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    void setLeft(bool left);

    virtual void setHidden(bool hidden);
    bool isHidden();
    bool inMotion();

    void setPos(glm::vec2 pos) override;

private:
    bool _hidden = false;

    bool _left = true;

    glm::vec2 _original_pos = _pos;
    double _duration = .1;
    double _curr_time = 0;

    void set_interactable(bool interactable);
};


#endif //GOMOKU_MENUVIEW_H
