#include "presentation/renderer/Renderer.h"

int main() {

    Renderer renderer;

    renderer.drawQuad("quad1", glm::vec2(1), glm::vec2(1));

    while(renderer.shouldRun()){
        renderer.render();
    }

    return 0;
}
