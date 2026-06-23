#pragma once
#include <SDL2/SDL.h>
#include <SDL_scancode.h>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <unordered_set>

// like a register of actions
// [ space, j ] -> "jump" ->  callback
class InputManager {
  private:
    // convenience
    using CallbackType = void(*)(void*);
    struct CallbackRegistration {
        CallbackType func;
        void* context;
    };
    // so we need a function passed a bool (pressed), SDL_keymod (yes), and a void* for expansion
    // map of keys to labels 'j, space' -> "jump"
    // labels -> callbacks
    // basic state for polling query
    
    // pool of strings used within the InputManager, all string-bound actions
    std::unordered_set<std::string> stringPool;


    // using InputKeyCallback = std::move_only_function<void(bool isPressed, SDL_Keymod mod)>;

    // scancode to the abstract label name
    std::unordered_map<SDL_Scancode, std::string_view> scancodeToAction;

    // string view to ID + reverse
    std::unordered_map<std::string_view, uint32_t> actionToId;
    std::vector<std::string_view> idToAction;

    // scancode routes to ID
    std::unordered_map<SDL_Scancode, uint32_t> scancodeToActionId;

    //state tracking?
    // v2, more friendly. investigate bitmaps?
    std::vector<bool> currentFrameState;
    std::vector<bool> previousFrameState;
    // std::unordered_map<std::string_view, bool> currentFrameState;
    // std::unordered_map<std::string_view, bool> previousFrameState;

    std::vector<std::vector<CallbackRegistration>> actionCallbacks;
    // reworkr
    //std::unordered_map<std::string_view, std::vector<std::pair<CallbackType, void*>>> actionCallbacks;
    

    // WAY old attempt
    // std::unordered_map<std::string, bool> actionStates;
    // //std::unordered_map<SDL_Keycode, std::vector<std::move_only_function<void(void*, SDL_Keymod)>>> keyBindings;

    //internal helper
    uint32_t getORcreateActionId(const std::string& action) {
        auto [poolIt, inserted] = stringPool.insert(action);
        std::string_view view = *poolIt;
        auto [mapIt, mapInserted] = actionToId.try_emplace(view, static_cast<uint32_t>(actionToId.size()));
        uint32_t id = mapIt->second;
        if (mapInserted) {
            idToAction.push_back(view);
            currentFrameState.push_back(false);
            previousFrameState.push_back(false);
            actionCallbacks.emplace_back();
        }
        return id;
    }
  public:

    std::string_view poolString(const std::string& action) {
        auto [iterator, inserted] = stringPool.insert(action);
        return *iterator; 
    }


    void addKeyMapping(const std::string& action, SDL_Scancode scancode);
    void addActionFunctMap(const std::string& action, CallbackType callbackFunc, void* context);

    // sync prev to new and resync, to validate changes
    void updateStartOfFrame();
    void processEvent(const SDL_Event& event);
    //string ver
    bool isActionHeld(std::string_view action) const;
    bool isActionJustPressed(std::string_view action) const;
    bool isActionJustReleased(std::string_view action) const;

    //id ver
    bool isActionHeld(uint32_t id) const;
    bool isActionJustPressed(uint32_t id) const;
    bool isActionJustReleased(uint32_t id) const;

}; 
