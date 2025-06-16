#include <stdexcept>
#include "presentation/input/Input.h"

Input* Input::s_instance = nullptr;

Input *Input::get() {
    if(!s_instance)
        throw std::runtime_error("[Error] Input not initialized");
    return s_instance;
}

void Input::init(GLFWwindow* window) {
    if(!s_instance)
        s_instance = new Input(window);
}

Input::Input(GLFWwindow* window) {
    _window = window;

    glfwSetWindowUserPointer(_window, this);

    // setup Mouse Button callback
    auto mouseBtn = [](GLFWwindow* w, int b, int a, int m){
        static_cast<Input*>(glfwGetWindowUserPointer(w))->mouse_btn_callback( b, a, m);
    };
    glfwSetMouseButtonCallback(_window, mouseBtn);

    // setup Mouse Position callback
    auto mousePos = [](GLFWwindow* w, double x, double y){
        static_cast<Input*>(glfwGetWindowUserPointer(w))->mouse_pos_callback(x, y);
    };
    glfwSetCursorPosCallback(_window, mousePos);

    // setup Key callback
    auto key = [](GLFWwindow* w, int k, int s, int a, int m){
        static_cast<Input*>(glfwGetWindowUserPointer(w))->key_callback( k, s, a, m);
    };
    glfwSetKeyCallback(_window, key);

    auto text = [](GLFWwindow* w, unsigned int c){
        static_cast<Input*>(glfwGetWindowUserPointer(w))->char_callback(c);
    };
    glfwSetCharCallback(_window, text);


    // init maps
    for (int i = -1; i < Key::LAST; ++i) {
        _keys[(Key)i] = KeyState::NONE;
        _queue_keys[(Key)i] = KeyState::NONE;
        _immediate_keys[(Key)i] = KeyState::NONE;
    }
    for (int i = 0; i < Mouse::BUTTON_LAST; ++i) {
        _m_keys[i] = false;
    }
}

Input::~Input() {
    for(auto p : _text_queues)
        delete p.second;
}

void Input::mouse_btn_callback(int btn, int action, int mods) {
    _m_keys[btn] = (action == GLFW_PRESS);
}

void Input::mouse_pos_callback(double x, double y) {
    _mouse_x = x;
    _mouse_y = y;
}

void Input::key_callback(int key, int scancode, int action, int mods) {
    _immediate_keys[(Key)key] = glfw_to_keystate(action);
    _queue_keys[(Key)key] = action;
}

void Input::char_callback(unsigned int codepoint) {
    for(auto p : _text_queues)
        p.second->push(codepoint);
}

double Input::mouseX() const {
    return _mouse_x;
}

double Input::mouseY() const {
    return _mouse_y;
}

void Input::mouseP(glm::vec2 &pos) const {
    pos.x = (float)_mouse_x;
    pos.y = (float)_mouse_y;
}

bool Input::mousePressed(Mouse m) {
    return _m_keys[m];
}

void Input::update() {
    for(auto pair : _keys){
        Key k = pair.first;
        switch (pair.second) {
            case KeyState::NONE:
                if(_queue_keys[k] == GLFW_PRESS || _queue_keys[k] == GLFW_REPEAT){
                    _last_key = k;
                    _keys[k] = KeyState::PRESSED;
                }
                break;
            case KeyState::PRESSED:
                if(_queue_keys[k] == GLFW_PRESS)
                    _keys[k] = KeyState::PRESSING;
                else if(_queue_keys[k] == GLFW_REPEAT) // This should normally not occur
                    _keys[k] = KeyState::HOLDING;
                else if(_queue_keys[k] == GLFW_RELEASE)
                    _keys[k] = KeyState::RELEASED;
                break;
            case KeyState::PRESSING:
                if(_queue_keys[k] == GLFW_REPEAT) // This should normally not occur
                    _keys[k] = KeyState::HOLDING;
                else if(_queue_keys[k] == GLFW_RELEASE)
                    _keys[k] = KeyState::RELEASED;
                break;
            case KeyState::HOLDING:
                if(_queue_keys[k] == GLFW_RELEASE)
                    _keys[k] = KeyState::RELEASED;
                break;
            case KeyState::RELEASED:
                _keys[k] = KeyState::NONE;
                break;
        }

        if(_queue_keys[k] == GLFW_RELEASE)
            _queue_keys[k] = -1;
    }
}

bool Input::keyPressed(Key k) {
    return (_keys[k] == KeyState::PRESSED) || (_keys[k] == KeyState::PRESSING || (_keys[k] == KeyState::HOLDING));
}

bool Input::keyXPressed(Key k) {
    return _keys[k] == KeyState::PRESSED;
}

bool Input::keyPressing(Key k) {
    return (_keys[k] == KeyState::PRESSED) || (_keys[k] == KeyState::PRESSING);
}

bool Input::keyXPressing(Key k) {
    return _keys[k] == KeyState::PRESSING;
}

bool Input::keyHolding(Key k) {
    return _keys[k] == KeyState::HOLDING;
}

bool Input::keyReleased(Key k) {
    return _keys[k] == KeyState::RELEASED;
}

bool Input::keyNone(Key k) {
    return _keys[k] == KeyState::NONE;
}

bool Input::isKey(Key k, KeyState state) {
    return _keys[k] == state;
}

KeyState Input::keyState(Key k) {
    return _keys[k];
}

KeyState Input::iKeyState(Key k) {
    return _immediate_keys[k];
}

KeyState Input::glfw_to_keystate(int action) {
    switch (action) {
        case GLFW_RELEASE:
            return KeyState::RELEASED;
        case GLFW_PRESS:
            return KeyState::PRESSING;
        case GLFW_REPEAT:
            return KeyState::HOLDING;
        default:
            return KeyState::NONE;
    }
}

Key Input::lastKey() {
    return _last_key;
}

const char* Input::getClipboard() {
    return glfwGetClipboardString(_window);
}

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * While string is in clipboard can cause lag
 *
*/
void Input::setClipboard(char *content) {
    glfwSetClipboardString(_window, content);
}

uint32_t Input::registerTextQueue() {
    uint32_t id = _next_queue_id;
    _next_queue_id++;
    _text_queues[id] = new std::queue<uint32_t>;
    return id;
}

void Input::unregisterTextQueue(uint32_t id) {
    delete _text_queues[id];
    _text_queues[id] = nullptr;
}

uint32_t Input::getTextQueue(uint32_t id) {
    if(isTextQueueEmpty(id))
        return 0;

    return _text_queues[id]->front();
}

uint32_t Input::popTextQueue(uint32_t id) {
    if(isTextQueueEmpty(id))
        return 0;

    uint32_t c = _text_queues[id]->front();
    _text_queues[id]->pop();
    return c;
}

bool Input::isTextQueueEmpty(uint32_t id) {
    return _text_queues[id]->empty();
}

void Input::clearTextQueue(uint32_t id) {
    while(!isTextQueueEmpty(id))
        popTextQueue(id);
}


