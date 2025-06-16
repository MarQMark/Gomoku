#ifndef GOMOKU_BACKGROUNDVIEW_H
#define GOMOKU_BACKGROUNDVIEW_H


#include "presentation/ui/components/View.h"
#include "logic/interfaces/IMenuEventListener.h"
#include "presentation/ui/components/Sprite.h"

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
};


#endif //GOMOKU_BACKGROUNDVIEW_H
