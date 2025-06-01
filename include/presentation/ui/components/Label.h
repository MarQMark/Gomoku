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

private:
    std::string _text{};
};


#endif //GOMOKU_LABEL_H
