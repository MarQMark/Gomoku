#include "common/Time.h"

#include "GLFW/glfw3.h"

Time* Time::s_instance = nullptr;

Time * Time::get() {
    return s_instance;
}

void Time::init() {
    s_instance = new Time();
}

double Time::getDeltaTime() const {
    return _delta_time;
}

void Time::update() {
    _delta_time = glfwGetTime() - _last_time;
    _last_time = glfwGetTime();
}

Time::Time(): _delta_time(0) {
    _last_time = glfwGetTime();
}
