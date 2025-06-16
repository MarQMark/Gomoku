#include "presentation/ui/BoardView.h"
#include "presentation/input/Input.h"
#include <iostream>
#include <cstring>

#include "logic/GameService.h"
#include "presentation/assets/AssetManager.h"
#include "presentation/mapping/MapViewToCommand.h"
#include "animator/SimpleAnimator.h"
#include "common/Time.h"

BoardView::BoardView(std::string name, IGameService* gameService) : View(std::move(name)) {
    View::setKeepHeight(true);
    View::setAlignH(IViewable::CENTER);
    View::setAlignV(IViewable::CENTER);
    View::setPos(glm::vec2(0.1f, 0.1f));
    View::setDim(glm::vec2(0.8f));

    _game_service = gameService;
    _game_service->addListener(this);
    const int size = _game_service->getBoardSize();
    _stone_sprites.resize(size, std::vector<Sprite*>(size, nullptr));
    _show_hover_preview = false;
    _mouse_pressed = false;
    _prev_mouse_pressed = false;
    initialize_sprites();
}

BoardView::~BoardView() {
    delete _background_board;
    delete _board_grid;
    delete _hover_preview_sprite;
    delete _winning_line;

    const int size = _game_service->getBoardSize();
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            // probably wrong but should work for use case
            delete _stone_sprites[y][x];
        }
    }
}

void BoardView::initialize_sprites() {
    auto* boardAnimator = new SimpleAnimator(.3, 0, "board");
    _background_board = new Sprite(AssetManager::getName(Textures::board_background),
                                   AssetManager::getName(Textures::board_background),
                                   glm::vec2(0, 0), glm::vec2(1, 1));
    _background_board->setLayer(0);
    _background_board->setVisible(true);
    _background_board->setAnimator(boardAnimator);
    addViewable(_background_board);

    _board_grid = new Sprite(AssetManager::getName(Textures::board_grid),
                             AssetManager::getName(Textures::board_grid),
                             glm::vec2(0.06f, 0.06f), glm::vec2(0.88f, 0.88f));
    _board_grid->setLayer(1);
    _board_grid->setVisible(true);
    _board_grid->setAnimator(boardAnimator);
    addViewable(_board_grid);

    const float stoneSize = calculate_stone_size();

    _hover_preview_sprite = new Sprite("hover_preview", AssetManager::getName(Textures::black_stone_hover), glm::vec2(0, 0), glm::vec2(stoneSize, stoneSize));
    _hover_preview_sprite->setLayer(8);
    _hover_preview_sprite->setVisible(false);
    addViewable(_hover_preview_sprite);

    _winning_line = new Sprite(AssetManager::getName(Textures::line_horizontal), AssetManager::getName(Textures::line_horizontal), glm::vec2(0, 0), glm::vec2(1.0, 1.0));
    _winning_line->setLayer(10);
    _winning_line->setDim(glm::vec2(0.88f, 0.88f));
    _winning_line->setVisible(false);
    addViewable(_winning_line);
}

void BoardView::clear_board() {
    for (const auto& stoneRow : _stone_sprites) {
        for (Sprite* stoneSprite : stoneRow) {
            if (stoneSprite != nullptr) {
                deleteViewable<Sprite>(stoneSprite->getName());
            }
        }
    }
    _stone_sprites.clear();
    _stone_sprites.resize(_game_service->getBoardSize(), std::vector<Sprite*>(_game_service->getBoardSize(), nullptr));

    if (_winning_line != nullptr) {
        _winning_line->setVisible(false);
    }
}

void BoardView::render(Renderer* renderer, const glm::vec2 parentPos, const glm::vec2 parentDim) {
    _game_service->update(Time::get()->getDeltaTime());
    View::render(renderer, parentPos, parentDim);
    handle_mouse_input(renderer);
}

BoardViewDTO BoardView::getCurrentBoardState() const {
    return _game_service->getBoardState();
}

void BoardView::handle_mouse_input(Renderer* renderer) {
    const glm::vec2 relativeMousePos = relative_inside_grid_view(_board_grid->getAbsPos(), _board_grid->getAbsDim(),
                                                                 renderer->getCursorPos(), renderer->getViewportSize());
    handle_mouse_hover(relativeMousePos);
    handle_mouse_click(relativeMousePos);
}

void BoardView::handle_mouse_hover(glm::vec2 relativeMousePos) {
    const StoneViewDTO mouseHoverDTO = _game_service->processMouseHover(MapViewToCommand::toMouseCommandDTO(relativeMousePos, _game_service->getBoardSize()));
    if (!mouseHoverDTO.isValidPosition) {
        _show_hover_preview = false;
        _hover_preview_sprite->setVisible(false);
        return;
    }
    _show_hover_preview = true;
    update_hover_preview(mouseHoverDTO.previewColor, mouseHoverDTO.pos);
}

void BoardView::handle_mouse_click(glm::vec2 relativeMousePos) {
    _prev_mouse_pressed = _mouse_pressed;
    _mouse_pressed = Input::get()->mousePressed(BUTTON_LEFT);

    if (_mouse_pressed && !_prev_mouse_pressed) {
        _game_service->processMouseClick(MapViewToCommand::toMouseCommandDTO(relativeMousePos, _game_service->getBoardSize()));
    }
}

void BoardView::add_stone_sprite(const MoveViewDTO &move) {
    const std::string spriteName = "stone_" + std::to_string(move.stone.pos.x) + "_" + std::to_string(move.stone.pos.y);
    const glm::vec2 intersectionPos = grid_to_view_position(move.stone.pos, _board_grid->getPos(),
                                                            _board_grid->getDim(), _game_service->getBoardSize());
    const float stoneSize = calculate_stone_size();
    const glm::vec2 centeredPos = intersectionPos - glm::vec2(stoneSize * 0.5f);
    const auto stoneSprite = new Sprite(spriteName, get_stone_texture(move.stone.previewColor, false), centeredPos, glm::vec2(stoneSize, stoneSize));
    _stone_sprites[move.stone.pos.y][move.stone.pos.x] = stoneSprite;
    stoneSprite->setLayer(8);
    stoneSprite->setVisible(true);
    Animator* animator = nullptr;
    if(move.boardView.moveNumber % 2 == 1)
        animator = new SimpleAnimator(.3, stoneSprite->getLayer() + 1, "stone");
    else
        animator = new SimpleAnimator(.3, stoneSprite->getLayer() + 1, "stone2");
    stoneSprite->setAnimator(animator);
    addViewable(stoneSprite);

    if(!_background_board->getAnimator()->isActive())
        _background_board->getAnimator()->reset();
    animator->reset();
}

void BoardView::update_hover_preview(ViewColor previewColor, GridViewPosition hoverGridPosition) const {
    if (_show_hover_preview) {
        const int gridSize = _game_service->getBoardSize();
        const float stoneSize = calculate_stone_size();
        const glm::vec2 intersectionPos = grid_to_view_position(hoverGridPosition, _board_grid->getPos(),
                                                                _board_grid->getDim(), gridSize);
        const glm::vec2 centeredPos = intersectionPos - glm::vec2(stoneSize * 0.5f);

        _hover_preview_sprite->setPos(centeredPos);
        _hover_preview_sprite->setDim(glm::vec2(stoneSize, stoneSize));
        _hover_preview_sprite->setTexture(get_stone_texture(previewColor, true));
        _hover_preview_sprite->setVisible(true);
    } else {
        _hover_preview_sprite->setVisible(false);
    }
}

std::string BoardView::get_stone_texture(ViewColor color, bool isHover) {
    if (isHover) {
        return (color == ViewColor::BLACK) ? AssetManager::getName(Textures::black_stone_hover) : AssetManager::getName(Textures::white_stone_hover);
    }
    return (color == ViewColor::BLACK) ? AssetManager::getName(Textures::black_stone) : AssetManager::getName(Textures::white_stone);
}

float BoardView::calculate_stone_size() const {
    const float gridSpacing = 1.0f / (float)(_game_service->getBoardSize() - 1);
    return gridSpacing * _board_grid->getDim().x;
}

glm::vec2 BoardView::grid_to_view_position(GridViewPosition position, glm::vec2 boardPos, glm::vec2 boardSize, int boardSizeGrid) {
    const float gridSpacing = 1.0f / (float)(boardSizeGrid - 1);
    const auto relativeIntersection = glm::vec2(
        (float)position.x * gridSpacing,
        (float)position.y * gridSpacing
    );

    const glm::vec2 actualIntersection = boardPos + relativeIntersection * boardSize;
    return actualIntersection;
}

void BoardView::onMoveCompleted(const MoveViewDTO& move) {
    add_stone_sprite(move);
    _show_hover_preview = false;
    _hover_preview_sprite->setVisible(false);
}

glm::vec2 BoardView::relative_inside_grid_view(glm::vec2 boardPos, glm::vec2 boardSize,
                                               glm::vec2 mousePos, glm::vec2 viewportSize) {
    const glm::vec2 normalizedMouse = mousePos / viewportSize;
    const glm::vec2 relativeMouse = (normalizedMouse - boardPos) / boardSize;

    return relativeMouse;
}

void BoardView::onGameStarted() {
    clear_board();
}

void BoardView::onGameCompleted(const GameCompleteViewDTO completeView) {
    if (completeView.status == DRAW)
        return;

    const WinningLineOrientation orientation = determine_winning_line_orientation(completeView.winningLine);

    // Find top-left position for positioning
    GridViewPosition leftestPos = completeView.winningLine.at(0);
    int count = 0;
    for (const GridViewPosition& pos : completeView.winningLine) {
        if (count >= 5)
            break;
        leftestPos.x = std::min(leftestPos.x, pos.x);
        leftestPos.y = std::min(leftestPos.y, pos.y);
        count++;
    }

    // Texture has some kind of offset to be applied correctly (-2, -2)
    const glm::vec2 viewPos = grid_to_view_position(leftestPos - GridViewPosition(2, 2),
                                                    _board_grid->getPos(), _board_grid->getDim(),
                                                    _game_service->getBoardSize());
    switch (orientation) {
        case HORIZONTAL:
            _winning_line->setTexture(AssetManager::getName(Textures::line_horizontal));
            break;
        case VERTICAL:
            _winning_line->setTexture(AssetManager::getName(Textures::line_vertical));
            break;
        case DIAGONAL_TOP_LEFT_TO_BOTTOM_RIGHT:
            _winning_line->setTexture(AssetManager::getName(Textures::line_top_left_bottom_right));
            break;
        case DIAGONAL_BOTTOM_LEFT_TO_TOP_RIGHT:
            _winning_line->setTexture(AssetManager::getName(Textures::line_bottom_left_top_right));
            break;
    }

    _winning_line->setPos(viewPos + glm::vec2(0.061f, 0.061f));
    _winning_line->setVisible(true);
}

WinningLineOrientation BoardView::determine_winning_line_orientation(const std::vector<GridViewPosition>& winningLine) {
    if (winningLine.size() < 2) {
        // Fallback (invalid)
        return HORIZONTAL;
    }

    const GridViewPosition first = winningLine[0];
    const GridViewPosition second = winningLine[1];
    const int dx = second.x - first.x;
    const int dy = second.y - first.y;

    if (dy == 0) return HORIZONTAL;

    if (dx == 0) return VERTICAL;

    if (dx == dy) return DIAGONAL_TOP_LEFT_TO_BOTTOM_RIGHT;

    if (dx == -dy) return DIAGONAL_BOTTOM_LEFT_TO_TOP_RIGHT;

    // Fallback
    return HORIZONTAL;
}