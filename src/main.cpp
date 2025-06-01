#include "logic/GameService.h"
#include "presentation/renderer/Renderer.h"
#include "presentation/assets/AssetManager.h"
#include "presentation/ui/UI.h"
#include "presentation/ui/components/Label.h"
#include "presentation/ui/BoardView.h"
#include "animator/BackgroundAnimator.h"
#include "common/Time.h"
#include "presentation/ui/components/DrawerView.h"
#include "presentation/ui/components/MenuView.h"
#include "presentation/ui/MenuController.h"
#include "presentation/ui/MainMenuView.h"
#include "presentation/ui/ModeMenuView.h"
#include "presentation/ui/DifficultyMenuView.h"
#include "presentation/ui/GameMenuView.h"

int main() {
    Time::init();
    GameService gameService;
    Renderer renderer;
    UI ui(&renderer);

    // Init Assets
    AssetManager::initialize(&renderer);
    AssetManager::instance()->loadGameAssets();

    int iconWidth, iconHeight;
    unsigned char* iconBlack = AssetManager::instance()->loadPixelsFromFile("black_stone", &iconWidth, &iconHeight);
    unsigned char* iconWhite = AssetManager::instance()->loadPixelsFromFile("white_stone", &iconWidth, &iconHeight);
    renderer.setWindowIcon(iconWidth, iconHeight, iconBlack);

    Sprite bg("background", AssetManager::getName(Textures::black_stone), glm::vec2(), glm::vec2(1));
    auto* bgAnimator = new BackgroundAnimator(0, 0, "bg");
    bgAnimator->setRenderer(&renderer);
    bgAnimator->setTexture2(AssetManager::getName(Textures::black_stone));
    bg.setAnimator(bgAnimator);
    bg.setLayer(-1);
    ui.getViewable<View>("root")->addViewable(&bg);

    MenuController menuController;

    MainMenuView mainMenuView("mainMenuView", &menuController);
    ui.getViewable<View>("root")->addViewable(&mainMenuView);
    ModeMenuView modeMenuView("modeMenuView", &menuController);
    ui.getViewable<View>("root")->addViewable(&modeMenuView);
    DifficultyMenuView difficultyMenuView("difficultyMenuView", &menuController);
    ui.getViewable<View>("root")->addViewable(&difficultyMenuView);
    GameMenuView gameMenuView("gameMenuView", &menuController);
    ui.getViewable<View>("root")->addViewable(&gameMenuView);

    // Create the main game board (centered, square aspect ratio)
    auto* boardView = new BoardView("game_board", &gameService);
    boardView->setKeepHeight(true);
    boardView->setAlignH(IViewable::CENTER);
    boardView->setAlignV(IViewable::CENTER);
    boardView->setPos(glm::vec2(0.1f, 0.1f));
    boardView->setDim(glm::vec2(0.8f));
    ui.getViewable<View>("root")->addViewable(boardView);

    // Create game status label
    auto* statusLabel = new Label("statusLabel", "Black Player's Turn");
    statusLabel->setPos(glm::vec2(0.75f, 0.85f));
    statusLabel->setDim(glm::vec2(0.2f, 0.05f));
    ui.getViewable<View>("root")->addViewable(statusLabel);

    std::string tex1;
    std::string tex2;
    // TODO: put into singleton
    while(renderer.shouldRun()) {
        const auto& boardState = boardView->getCurrentBoardState();
        std::string statusText;

        switch(boardState.gameStatus) {
            case IN_PROGRESS:{
                statusText = boardState.currentPlayerName + "'s Turn";
                tex1 = boardState.currentPlayerName == "Black Player" ? AssetManager::getName(Textures::black_stone) : AssetManager::getName(Textures::white_stone);
                if(boardState.currentPlayerName == "Black Player")
                    renderer.setWindowIcon(iconWidth, iconHeight, iconBlack);
                else
                    renderer.setWindowIcon(iconWidth, iconHeight, iconWhite);
                if(tex1 == tex2){
                    bgAnimator->reset();
                }
                bg.setTexture(tex1);
                tex2 = boardState.currentPlayerName == "Black Player" ? AssetManager::getName(Textures::white_stone) : AssetManager::getName(Textures::black_stone);
                bgAnimator->setTexture2(tex2);
                break;}
            case BLACK_WINS:
                statusText = "Black Wins!";
                break;
            case WHITE_WINS:
                statusText = "White Wins!";
                break;
            case DRAW:
                statusText = "Draw Game!";
                break;
            default:
                statusText = "Game Not Started";
                break;
        }

        statusLabel->setText(statusText);

        menuController.update();
        ui.update();
        Time::get()->update();
        renderer.render();
    }

    // Cleanup
    delete boardView;
    delete statusLabel;
    return 0;
}