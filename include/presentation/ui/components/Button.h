#ifndef GOMOKU_BUTTON_H
#define GOMOKU_BUTTON_H

#include "IInteractable.h"

class Button : public IInteractable {
public:
    explicit Button(std::string name);
    Button(std::string name, glm::vec2 pos, glm::vec2 dim);
    virtual ~Button() = default;

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    void setText(std::string text);
    void setTextHeight(float textHeight);

    void setTexture(State state, std::string name);
    void setShader(State state, std::string name);

private:
    void init_maps();

    std::string _text{};
    float _text_height = 0.6;

    std::map<State, std::string> _texture_names;
    std::map<State, std::string> _shader_names;
};


#endif //GOMOKU_BUTTON_H
