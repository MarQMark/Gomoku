#include "common/RunManager.h"


RunManager* RunManager::s_instance = nullptr;

RunManager::RunManager(Renderer *renderer) : _renderer(renderer) {
}

void RunManager::create(Renderer *renderer) {
    s_instance = new RunManager(renderer);
}

void RunManager::destroy() {
    delete s_instance;
}

RunManager *RunManager::get() {
    return s_instance;
}

bool RunManager::shouldRun() {
    return _should_run && _renderer->shouldRun();
}

void RunManager::stop() {
    _should_run = false;
}
