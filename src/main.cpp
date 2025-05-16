#include "presentation/renderer/Renderer.h"

int main() {

    Renderer renderer;

    while(renderer.shouldRun()){
        renderer.render();
    }

    return 0;
}
