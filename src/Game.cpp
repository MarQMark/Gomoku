#include "Game.h"
#include "common/RunManager.h"
#include "common/Time.h"
#include "presentation/assets/AssetManager.h"
#include "presentation/ui/BoardView.h"
#include "presentation/ui/StatsMenuView.h"
#include "presentation/ui/GameMenuView.h"
#include "presentation/ui/BackgroundView.h"
#include "presentation/ui/MainMenuView.h"
#include "presentation/ui/ModeMenuView.h"
#include "presentation/ui/DifficultyMenuView.h"

#ifdef DEBUG
#include <sstream>
#include <iomanip>
#endif

Game::Game() {
    _renderer = new Renderer();
    _ui = new UI(_renderer);
    _file_manager = new FileManager();
    _game_service = new GameService(_file_manager);

    Time::init();
    RunManager::create(_renderer);

    AssetManager::initialize(_renderer);
    AssetManager::instance()->loadGameAssets();


    auto* backgroundView = new BackgroundView("backgroundView", _renderer);
    _game_service->addMenuListener(backgroundView);
    _ui->addViewable(backgroundView);

    _menu_controller = new MenuController(_game_service);
    auto mainMenuView = new MainMenuView("mainMenuView", _menu_controller);
    _ui->addViewable(mainMenuView);
    auto* modeMenuView = new ModeMenuView("modeMenuView", _menu_controller);
    _ui->addViewable(modeMenuView);
    auto* difficultyMenuView = new DifficultyMenuView("difficultyMenuView", _menu_controller);
    _ui->addViewable(difficultyMenuView);
    auto* gameMenuView = new GameMenuView("gameMenuView", _menu_controller);
    _ui->addViewable(gameMenuView);
    auto* statsMenuView = new StatsMenuView("statsMenuView", _menu_controller);
    _ui->addViewable(statsMenuView);

    auto* boardView = new BoardView("game_board", _game_service);
    _ui->addViewable(boardView);

#ifdef DEBUG
    _fps_lbl = new Label("fpsCounter", "");
    _fps_lbl->setAlignV(IViewable::LEFT);
    _fps_lbl->setAlignH(IViewable::TOP);
    _fps_lbl->setDim(glm::vec2(0, .04));
    _ui->addViewable(_fps_lbl);
#endif
}

Game::~Game() {
    _ui->deleteViewable<BoardView>("game_board");
    _ui->deleteViewable<StatsMenuView>("statsMenuView");
    _ui->deleteViewable<GameMenuView>("gameMenuView");
    _ui->deleteViewable<DifficultyMenuView>("difficultyMenuView");
    _ui->deleteViewable<ModeMenuView>("modeMenuView");
    _ui->deleteViewable<MainMenuView>("mainMenuView");
    _ui->deleteViewable<BackgroundView>("backgroundView");
#ifdef DEBUG
    _ui->deleteViewable<Label>("fpsCounter");
#endif

    RunManager::destroy();

    delete _game_service;
    delete _file_manager;
    delete _menu_controller;
    delete _ui;
    delete _renderer;
}

bool Game::shouldRun() {
    return RunManager::get()->shouldRun();
}

void Game::update() {
    Time::get()->update();

#ifdef DEBUG
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << 1 / Time::get()->getDeltaTime();
    _fps_lbl->setText(ss.str());
#endif

    _menu_controller->update();
    _ui->update();
    _renderer->render();
}
