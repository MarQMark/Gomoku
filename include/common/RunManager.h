#ifndef GOMOKU_RUNMANAGER_H
#define GOMOKU_RUNMANAGER_H


#include "Renderer.h"

class RunManager {
private:
    RunManager(Renderer* renderer);
    ~RunManager() = default;

    static RunManager* s_instance;

public:
    static RunManager* get();

    static void create(Renderer* renderer);
    static void destroy();

    bool shouldRun();
    void stop();

private:
    Renderer* _renderer{};

    bool _should_run = true;
};


#endif //GOMOKU_RUNMANAGER_H
