#ifndef GOMOKU_SPRITE_H
#define GOMOKU_SPRITE_H


#include "IViewable.h"

class Sprite : public IViewable{
public:
    Sprite(std::string name, std::string textureName);
    Sprite(std::string name, std::string textureName, glm::vec2 pos, glm::vec2 dim);

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    void setTexture(std::string name);
    std::string getTexture();

private:
    std::string _texture_name = 0;
};


#endif //GOMOKU_SPRITE_H
