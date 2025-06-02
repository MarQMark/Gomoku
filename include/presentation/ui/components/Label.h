#ifndef GOMOKU_LABEL_H
#define GOMOKU_LABEL_H


#include "IViewable.h"

class Label : public IViewable {
public:
    Label(std::string name, std::string text);
    virtual ~Label() = default;

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    void setText(std::string text);
    std::string getText();

    void setFontColor(glm::vec4 color);

private:
    std::string _text{};
    glm::vec4 _font_color = glm::vec4(1);
};


#endif //GOMOKU_LABEL_H
