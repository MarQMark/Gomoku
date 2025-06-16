#ifndef GOMOKU_BACKGROUNDVIEW_H
#define GOMOKU_BACKGROUNDVIEW_H

#include <list>
#include "presentation/ui/components/View.h"
#include "presentation/ui/components/Sprite.h"
#include "logic/interfaces/IMenuEventListener.h"
#include "animator/BackgroundAnimator.h"

class BackgroundView : public View, public IMenuEventListener {
public:
    BackgroundView(std::string name, Renderer* renderer);
    ~BackgroundView() override;

    void render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) override;

    void onGameCompleted() override;
    void onStatsChanged(StatsViewDTO stats) override;

private:
    Renderer* _renderer{};

    std::string _stone_texture_1{};
    std::string _stone_texture_2{};

    unsigned char* _icon_black{};
    unsigned char* _icon_white{};
    int _icon_width = 0;
    int _icon_height = 0;

    struct Stone{
        Stone(glm::vec2 pos, glm::vec2 dim, glm::vec2 vel) : pos(pos), dim(dim), vel(vel) {};
        glm::vec2 pos = glm::vec2(.5);
        glm::vec2 dim = glm::vec2(0);
        glm::vec2 vel = glm::vec2(0);
    };

    BackgroundAnimator* _animator;

    bool _blacks_turn = true;
    bool _draw = false;

    uint32_t _max_stones = 0;
    std::list<Stone> _stones;

    bool spawn_stone(float aspectRatio);
    void remove_stone();
};


#endif //GOMOKU_BACKGROUNDVIEW_H
