#include "presentation/ui/BackgroundView.h"
#include "presentation/DTO/ViewModelDTOs.h"
#include "presentation/assets/AssetManager.h"
#include "animator/BackgroundAnimator.h"

BackgroundView::BackgroundView(std::string name, Renderer* renderer) : View(std::move(name)) , _renderer(renderer) {
    auto bgSprite = new Sprite("bgSprite", AssetManager::getName(Textures::black_stone), glm::vec2(), glm::vec2(1));
    auto* bgAnimator = new BackgroundAnimator(0, 0, "bg");
    bgAnimator->setRenderer(renderer);
    bgAnimator->setTexture2(AssetManager::getName(Textures::black_stone));
    bgSprite->setAnimator(bgAnimator);
    bgSprite->setLayer(-1);
    View::addViewable(bgSprite);


    _icon_black = AssetManager::instance()->loadPixelsFromFile("black_stone", &_icon_width, &_icon_height);
    _icon_white = AssetManager::instance()->loadPixelsFromFile("white_stone", &_icon_width, &_icon_height);
    _renderer->setWindowIcon(_icon_width, _icon_height, _icon_black);
}

BackgroundView::~BackgroundView() {
    deleteViewable<Sprite>("bgSprite");

    AssetManager::instance()->freeLoadedPixels(_icon_black);
    AssetManager::instance()->freeLoadedPixels(_icon_white);
}

void BackgroundView::render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) {
    View::render(renderer, parentPos, parentDim);

    // TODO: Add Particles instead of fragment shader
}

void BackgroundView::onGameCompleted() {
}

void BackgroundView::onStatsChanged(StatsViewDTO stats) {
    switch(stats.gameStatus) {
        case IN_PROGRESS: {
            bool whitesTurn = stats.lastPlayerColor == ViewColor::WHITE;
            Sprite* bgSprite = getViewable<Sprite>("bgSprite");
            auto* bgAnimator = (BackgroundAnimator*)bgSprite->getAnimator();

            _stone_texture_1 = whitesTurn ? AssetManager::getName(Textures::black_stone) : AssetManager::getName(Textures::white_stone);
            if(_stone_texture_1 == _stone_texture_2)
                bgAnimator->reset();

            _stone_texture_2 = whitesTurn ? AssetManager::getName(Textures::white_stone) : AssetManager::getName(Textures::black_stone);
            bgSprite->setTexture(_stone_texture_1);
            bgAnimator->setTexture2(_stone_texture_2);
            bgAnimator->setShader("bg");

            if(whitesTurn)
                _renderer->setWindowIcon(_icon_width, _icon_height, _icon_black);
            else
                _renderer->setWindowIcon(_icon_width, _icon_height, _icon_white);

            break;}
        case BLACK_WINS:
            getViewable<Sprite>("bgSprite")->getAnimator()->setShader("bgWin");
            break;
        case WHITE_WINS:
            getViewable<Sprite>("bgSprite")->getAnimator()->setShader("bgWin");
            break;
        case DRAW:
            getViewable<Sprite>("bgSprite")->getAnimator()->setShader("bgDraw");
            break;
        default:
            break;
    }
}
