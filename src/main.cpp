#include "logic/GameService.h"
#include "presentation/renderer/Renderer.h"
#include "presentation/assets/AssetManager.h"
#include "presentation/ui/UI.h"
#include "presentation/ui/components/Button.h"
#include "presentation/ui/components/Label.h"
#include "presentation/ui/BoardView.h"
#include "animator/BackgroundAnimator.h"
#include "common/Time.h"
#include "presentation/ui/components/DrawerView.h"
#include "presentation/ui/components/MenuView.h"

void newGameCallback(IInteractable* interactable, IInteractable::State state, void* data) {
    std::cout << "New Game Started!" << std::endl;
    auto* boardView = static_cast<BoardView*>(data);
}

void testBtnClbk(IInteractable* interactable, IInteractable::State state, void* data) {
    auto* menuView = (MenuView*)data;
    menuView->setHidden(!menuView->isHidden());
    std::cout << interactable->getName() << std::endl;
}

int main() {
    Time::init();
    GameService gameService;
    Renderer renderer;
    UI ui(&renderer);

    // Init Assets
    AssetManager::initialize(&renderer);
    AssetManager::instance()->loadGameAssets();

    Sprite bg("background", AssetManager::getName(Textures::black_stone), glm::vec2(), glm::vec2(1));
    auto* bgAnimator = new BackgroundAnimator(0, 0, "bg");
    bgAnimator->setRenderer(&renderer);
    bgAnimator->setTexture2(AssetManager::getName(Textures::black_stone));
    bg.setAnimator(bgAnimator);
    bg.setLayer(-1);
    ui.getViewable<View>("root")->addViewable(&bg);

    MenuView testMenu("drawerView", glm::vec2(0), glm::vec2(.25, .7));
    testMenu.setMargin(glm::vec4(0.15, 0, 0.01, 0));
    testMenu.setAlignH(IViewable::Align::TOP);
    ui.getViewable<View>("root")->addViewable(&testMenu);

    Button menuBtn1("MenuBtn1", glm::vec2(), glm::vec2(.95, .2));
    menuBtn1.setAlignV(IViewable::Align::RIGHT);
    menuBtn1.setAlignH(IViewable::Align::TOP);
    menuBtn1.registerCallback(testBtnClbk, IInteractable::State::PRESSED, &testMenu);
    testMenu.addViewable(&menuBtn1);
    Button menuBtn2("MenuBtn2", glm::vec2(), glm::vec2(.95, .2));
    menuBtn2.setAlignV(IViewable::Align::RIGHT);
    menuBtn2.setAlignH(IViewable::Align::CENTER);
    testMenu.addViewable(&menuBtn2);
    Button menuBtn3("MenuBtn3", glm::vec2(), glm::vec2(.95, .2));
    menuBtn3.setAlignV(IViewable::Align::RIGHT);
    menuBtn3.setAlignH(IViewable::Align::BOTTOM);
    testMenu.addViewable(&menuBtn3);


    Button testBtn("testBtn", glm::vec2(0), glm::vec2(.2, .1));
    testBtn.setAlignV(IViewable::Align::CENTER);
    testBtn.setAlignH(IViewable::Align::TOP);
    testBtn.registerCallback(testBtnClbk, IInteractable::State::PRESSED, &testMenu);
    ui.getViewable<View>("root")->addViewable(&testBtn);

    /*DrawerView drawerView("drawerView", glm::vec2(0), glm::vec2(.25, .8));
    drawerView.setMargin(glm::vec4(0.15, 0, 0, 0));
    drawerView.setAlignV(IViewable::Align::LEFT);
    drawerView.setAlignH(IViewable::Align::TOP);
    ui.getViewable<View>("root")->addViewable(&drawerView);

    Button testBtn1("testBtn1", glm::vec2(.05, 0), glm::vec2(.95, .15));
    testBtn1.setText("Start");
    drawerView.addViewable(&testBtn1);
    Button testBtn2("testBtn2", glm::vec2(.05, .25), glm::vec2(.95, .15));
    testBtn2.setText("Load");
    drawerView.addViewable(&testBtn2);
    Button testBtn3("testBtn3", glm::vec2(.05, .5), glm::vec2(.95, .15));
    testBtn3.setText("Henk");
    drawerView.addViewable(&testBtn3);
    Button testBtn4("testBtn4", glm::vec2(.05, .75), glm::vec2(.95, .15));
    testBtn4.setText("Exit");
    drawerView.addViewable(&testBtn4);*/

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

    // Create New Game button
    auto* newGameBtn = new Button("newGameBtn", glm::vec2(0.75f, 0.75f), glm::vec2(0.2f, 0.08f));
    newGameBtn->setKeepWidth(true);
    newGameBtn->registerCallback(newGameCallback, IInteractable::State::PRESSED, boardView);
    ui.getViewable<View>("root")->addViewable(newGameBtn);

    // Add labels to buttons
    auto* newGameLabel = new Label("newGameLabel", "New Game");
    newGameLabel->setPos(glm::vec2(0.01f, 0.02f));
    newGameLabel->setDim(glm::vec2(0.18f, 0.04f));
    ui.getViewable<View>("root")->addViewable(newGameLabel);

    std::string tex1;
    std::string tex2;
    while(renderer.shouldRun()) {
        const auto& boardState = boardView->getCurrentBoardState();
        std::string statusText;

        switch(boardState.gameStatus) {
            case IN_PROGRESS:{
                statusText = boardState.currentPlayerName + "'s Turn";
                tex1 = boardState.currentPlayerName == "Black Player" ? AssetManager::getName(Textures::black_stone) : AssetManager::getName(Textures::white_stone);
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

        ui.update();
        Time::get()->update();
        renderer.render();
    }

    // Cleanup
    delete boardView;
    delete statusLabel;
    delete newGameBtn;
    delete newGameLabel;
    return 0;
}