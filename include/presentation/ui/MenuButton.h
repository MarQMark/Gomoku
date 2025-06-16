#ifndef GOMOKU_MENUBUTTON_H
#define GOMOKU_MENUBUTTON_H

#include "presentation/ui/components/Button.h"

class MenuButton : public Button {
public:
    explicit MenuButton(std::string name, glm::vec2 pos = glm::vec2(0));
    ~MenuButton() override = default;

    void render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

private:
};


#endif //GOMOKU_MENUBUTTON_H
