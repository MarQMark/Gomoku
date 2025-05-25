#ifndef KIKAN_STDINPUT_H
#define KIKAN_STDINPUT_H

#include "InputDefs.h"
#include <map>
#include <queue>
#include "glm/glm.hpp"

class Input{
private:
    static Input* s_instance;
    explicit Input(GLFWwindow* window);

public:
    static void init(GLFWwindow* window);
    static Input* get();
    ~Input();

    void update();
    
    Key lastKey();
    bool keyPressed(Key k);
    // Exclusive Pressed
    bool keyXPressed(Key k);
    bool keyPressing(Key k);
    // Exclusive Pressing
    bool keyXPressing(Key k);
    bool keyHolding(Key k);
    bool keyReleased(Key k);
    bool keyNone(Key k);
    bool isKey(Key k, KeyState state);
    KeyState keyState(Key k);
    /*
     *  Returns actual state of key
     *  This reduces input lag, but might also lead to key presses being missed, especially at low framerates
     */
    KeyState iKeyState(Key k);

    bool mousePressed(Mouse m);
    double mouseX() const;
    double mouseY() const;
    void mouseP(glm::vec2& pos) const;

    const char* getClipboard();
    void setClipboard(char* content);

    uint32_t registerTextQueue();
    void unregisterTextQueue(uint32_t id);
    uint32_t getTextQueue(uint32_t id);
    uint32_t popTextQueue(uint32_t id);
    bool isTextQueueEmpty(uint32_t id);
    void clearTextQueue(uint32_t id);

private:
    void mouse_btn_callback(int btn, int action, int mods);
    void mouse_pos_callback(double x, double y);
    void key_callback(int key, int scancode, int action, int mods);
    void char_callback(unsigned int codepoint);

    std::map<Key, int> _queue_keys{};
    std::map<Key, KeyState> _immediate_keys{};
    std::map<Key, KeyState> _keys{};
    // When two keys get pressed withing the same frame the higher (ASCII) get chosen
    Key _last_key = Key::UNKNOWN;

    std::map<int, bool> _m_keys{};
    double _mouse_x = 0;
    double _mouse_y = 0;

    std::map<uint32_t, std::queue<uint32_t>*> _text_queues;
    uint32_t _next_queue_id = 0;

    GLFWwindow* _window{};

    KeyState glfw_to_keystate(int action);
};

#endif //KIKAN_STDINPUT_H
