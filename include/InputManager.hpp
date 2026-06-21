#pragma once
#include <SDL2/SDL.h>
#include <SDL_scancode.h>
#include <any>
#include <iostream>
#include <unordered_map>
#include <functional>

// like a register of actions
// [ space, j ] -> "jump" ->  callback
class InputManager {
  private:
    // so we need a function passed a bool (pressed), SDL_keymod (yes), and a void* for expansion
    // map of keys to labels 'j, space' -> "jump"
    // labels -> callbacks
    // basic state for polling query

    using InputKeyCallback = std::move_only_function<void(bool isPressed, SDL_Keymod mod)>;
    std::unordered_map<SDL_Scancode, std::string> scancodeToActionLabel;
    std::unordered_map<std::string, std::vector<InputKeyCallback>> actionCallbacks;

    std::unordered_map<std::string, bool> actionStates;
    //std::unordered_map<SDL_Keycode, std::vector<std::move_only_function<void(void*, SDL_Keymod)>>> keyBindings;

  public:
    // void bindKey(SDL_Scancode, std::move_only_function<> ) 
    void addKeyMapping(const std::string& action, SDL_Scancode scancode) {
      scancodeToActionLabel[scancode] = action;
    }

    // signal connection
    void bindAction(const std::string& action, InputKeyCallback callback) {
      actionCallbacks[action].push_back(std::move(callback));
    }
    void parseKeyInput(const SDL_Event& event){
      SDL_Scancode code = event.key.keysym.scancode;
      auto acIter = scancodeToActionLabel.find(code);
      if (acIter != scancodeToActionLabel.end()) {
        const std::string& action = acIter->second;
        bool isPressed = (event.type == SDL_KEYDOWN);
        SDL_Keymod mod = static_cast<SDL_Keymod>(event.key.keysym.mod);
        actionStates[action] = isPressed;
        if (actionCallbacks.contains(action)) {
          for (auto& callback : actionCallbacks[action]) {
            if (callback) {
              callback(isPressed, mod);
            }
          }
        }
      }
    }

    bool isActionPressed(const std::string& action) const {
      auto it = actionStates.find(action);
      return it != actionStates.end() ? it->second : false;
    }
}; 
