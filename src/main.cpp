#include "presentation/renderer/Window.h"

int main() {

    Window* window = Window::create(1280, 720);
    if(!window)
        return -1;

    while(!window->shouldClose()){

    }

    return 0;
}
