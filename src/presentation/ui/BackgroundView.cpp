#include "presentation/ui/BackgroundView.h"
#include "presentation/DTO/ViewModelDTOs.h"
#include "presentation/assets/AssetManager.h"
#include "presentation/ui/components/ShaderSprite.h"
#include "common/Time.h"
#include "glm/ext/scalar_constants.hpp"
#include <random>
#include <iostream>

#define DEFAULT_MAX_STONES 100
#define WIN_MAX_STONES 500

BackgroundView::BackgroundView(std::string name, Renderer* renderer) : View(std::move(name)) , _renderer(renderer) {
    auto bgSprite = new ShaderSprite("bgSprite", "bg", glm::vec2(), glm::vec2(1));
    auto* bgSpriteAnimator = new BackgroundAnimator(0, 0, "bg");
    bgSpriteAnimator->setRenderer(renderer);
    bgSprite->setAnimator(bgSpriteAnimator);
    bgSprite->setLayer(-10);
    View::addViewable(bgSprite);

    _animator = new BackgroundAnimator(0, 0, "bgStone");
    _animator->setRenderer(renderer);
    _animator->setTexture2(AssetManager::getName(Textures::black_stone));
    _animator->setLayer(-5);

    _icon_black = AssetManager::instance()->loadPixels("black_stone", &_icon_width, &_icon_height);
    _icon_white = AssetManager::instance()->loadPixels("white_stone", &_icon_width, &_icon_height);
    _renderer->setWindowIcon(_icon_width, _icon_height, _icon_black);

    _max_stones = DEFAULT_MAX_STONES;
}

BackgroundView::~BackgroundView() {
    deleteViewable<ShaderSprite>("bgSprite");

    AssetManager::instance()->freeLoadedPixels(_icon_black);
    AssetManager::instance()->freeLoadedPixels(_icon_white);

    delete _animator;
}

void BackgroundView::render(Renderer *renderer, glm::vec2 parentPos, glm::vec2 parentDim) {
    View::render(renderer, parentPos, parentDim);

    glm::vec2 viewportSize = renderer->getViewportSize();
    float aspectRatio = (viewportSize.y / viewportSize.x);

    while(_stones.size() < _max_stones){
        if(!spawn_stone(aspectRatio)) {
            break;
        }
    }
    while(_stones.size() > _max_stones)
        remove_stone();

    Renderer::Options ops;
    ops.shaderName = "bgStone";
    ops.animator = _animator;
    for (auto& stone : _stones) {
        stone.pos += glm::vec2(stone.vel.x * aspectRatio, stone.vel.y) * (float)Time::get()->getDeltaTime();
        float scale = .12 / glm::length(stone.vel) + 1.5 * glm::length(glm::vec2(.5) - stone.pos);
        ops.layer = -5 - glm::length(stone.vel);

        Texture2D* texture = nullptr;
        if(!_draw){
            texture = _blacks_turn ? renderer->getTexture(AssetManager::getName(Textures::black_stone)) : renderer->getTexture(AssetManager::getName(Textures::white_stone));
        }
        else{
            // This is so cursed
            texture = ((uint64_t)&stone) & 0x10 ? renderer->getTexture(AssetManager::getName(Textures::black_stone)) : renderer->getTexture(AssetManager::getName(Textures::white_stone));
        }
        renderer->drawTextureID(
                (uint64_t)&stone,
                stone.pos,
                glm::vec2(stone.dim.x * aspectRatio, stone.dim.y) * scale,
                texture,
                ops);
    }

#define ERASE_BOUNDARY .2f
    for (auto it = _stones.begin(); it != _stones.end();) {
        if (it->pos.x > 1 + ERASE_BOUNDARY || it->pos.y > 1 + ERASE_BOUNDARY || it->pos.x < -ERASE_BOUNDARY || it->pos.y < -ERASE_BOUNDARY) {
            it = _stones.erase(it);
        } else {
            ++it;
        }
    }
}

void BackgroundView::onGameCompleted() {
}

void BackgroundView::onStatsChanged(StatsViewDTO stats) {
    switch(stats.gameStatus) {
        case IN_PROGRESS: {
            _draw = false;
            _max_stones = DEFAULT_MAX_STONES;

            if(_blacks_turn != (stats.lastPlayerColor == ViewColor::WHITE)){
                _animator->reset();
                _stone_texture_2 = _blacks_turn ? AssetManager::getName(Textures::black_stone) : AssetManager::getName(Textures::white_stone);
                _animator->setTexture2(_stone_texture_2);

                _blacks_turn = !_blacks_turn;
            }

            if(_blacks_turn)
                _renderer->setWindowIcon(_icon_width, _icon_height, _icon_black);
            else
                _renderer->setWindowIcon(_icon_width, _icon_height, _icon_white);

            break;}
        case DRAW:
            _draw = true;
        case BLACK_WINS:
        case WHITE_WINS:
            _max_stones = WIN_MAX_STONES;
            break;
        default:
            break;
    }
}

bool BackgroundView::spawn_stone(float aspectRatio) {
    float stagger = _stones.size() / _max_stones + .2;
    if((float)std::rand() / (float)RAND_MAX < stagger)
        return false;

    float angle = (float)std::rand() / (float)RAND_MAX * 2.0f * glm::pi<float>();
    glm::vec2 dir = glm::normalize(glm::vec2(std::cos(angle), std::sin(angle)));
    float speed = (float)std::rand() / (float)RAND_MAX * .05f + .05;
    glm::vec2 randPos = glm::vec2(0);
    if(_max_stones - _stones.size() > 20){
        randPos = glm::normalize(glm::vec2(std::cos(angle), std::sin(angle))) * ((float)std::rand() / (float)RAND_MAX) * .4f;
        randPos.x *= aspectRatio;
    }
    _stones.push_back(Stone(
            glm::vec2(0.5) + randPos,
            glm::vec2(0.02),
            speed * dir
            ));

    return true;
}

void BackgroundView::remove_stone() {
    auto it = _stones.begin();
    std::advance(it, std::rand() % _stones.size());
    _stones.erase(it);
}
