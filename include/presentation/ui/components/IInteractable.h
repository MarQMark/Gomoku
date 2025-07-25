#ifndef GOMOKU_INTERACTABLE_H
#define GOMOKU_INTERACTABLE_H

#include <iostream>
#include "presentation/ui/components/IViewable.h"

class IInteractable : public IViewable {
public:
    explicit IInteractable(std::string name) : IViewable(std::move(name)) {}
    IInteractable(std::string name, glm::vec2 pos, glm::vec2 dim) : IViewable(std::move(name), pos, dim) {}

    ~IInteractable() override {
        for(auto* callback : _callbacks)
            delete callback;
    }

    enum State{
        NONE,
        HOVERING,
        PRESSED,
        HOLDING,
        RELEASED,

        ALL, // Only for callbacks
    };

    void changeState(State state){
        if(_state == state && _prev_state == state && (state != HOVERING && state != HOLDING))
            return;

        _prev_state = _state;
        _state = state;

        if(!isVisible())
            return;

        for(auto* callback : _callbacks){
            if(callback->state == State::ALL || callback->state == _state)
                callback->callback(this, _state, callback->data);
        }
    }

    State getState(){
        return _state;
    }

    State getPrevState(){
        return _prev_state;
    }

    /*
     *  Register a new callback
     *
     *  callback:   Function pointer to callback
     *  state:      If other than ALL callback will only be called if state switches to the specified state
     *  data:       Option to pass data to the function, for example object which should be influenced by change (will not get freed by automatically)
     */
    uint32_t registerCallback(void(*callback)(IInteractable* interactable, State state, void* data), State state = State::ALL, void* data = nullptr){
        auto* cb = new Callback();
        cb->callback = callback;
        cb->state = state;
        cb->data = data;
        _callbacks.push_back(cb);
        return _callbacks.size();
    }

    void unregisterCallback(uint32_t id){
        if(id > _callbacks.size()) {
            std::cout << "[ERROR] no callback with id: " << id << "\n";
            return;
        }

        _callbacks.erase(std::remove(_callbacks.begin(), _callbacks.end(), _callbacks[id]), _callbacks.end());
    }

    bool isInteractable(){return _interactable;}
    void setInteractable(bool interactable){_interactable = interactable;}
    bool isFocused(){return _focused;}
    void setFocused(bool focused){_focused = focused;}

protected:
    // Even if not interactable it can still have focus
    bool _interactable = true;

    // Do not use only for internal use
    bool _focused = false;

private:
    State _state = State::NONE;
    State _prev_state = State::NONE;

    struct Callback{
        void(*callback)(IInteractable* interactable, State state, void* data);
        void* data;
        State state = State::ALL; // only call if state switches to this
    };

    std::vector<Callback*> _callbacks;
};

#endif //GOMOKU_INTERACTABLE_H
