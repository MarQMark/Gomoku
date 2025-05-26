#ifndef GOMOKU_SPRITE_H
#define GOMOKU_SPRITE_H


#include "IViewable.h"

class Sprite : public IViewable{
public:
    Sprite(std::string name, uint16_t textureID);
    Sprite(std::string name, uint16_t textureID, glm::vec2 pos, glm::vec2 dim);

    void render(Renderer* renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    void setTexture(uint16_t id);
    uint16_t getTexture();

private:
    uint16_t _texture_id = 0;
};


#endif //GOMOKU_SPRITE_H
