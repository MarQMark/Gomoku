#ifndef GOMOKU_MENUBUTTON_H
#define GOMOKU_MENUBUTTON_H

#include "presentation/ui/components/Button.h"

class MenuButton : public Button {
public:
    MenuButton(std::string name, glm::vec2 pos = glm::vec2(0));
    ~MenuButton() = default;

private:
};


#endif //GOMOKU_MENUBUTTON_H
