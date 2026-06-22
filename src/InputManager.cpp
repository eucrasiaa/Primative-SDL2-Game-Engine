#include "InputManager.hpp"
void InputManager::addKeyMapping(const std::string& action, SDL_Scancode scancode) {
  std::cout<<"adding: "<< action << " to " << scancode<< std::endl;
  scancodeToAction[scancode] = action;
  currentFrameState[action] = false;
  previousFrameState[action] = false;
}
void InputManager::addActionFunctMap(const std::string& action, CallbackType callbackFunc, void* context = nullptr) {
  if (currentFrameState.contains(action)) {
    actionCallbacks[action].push_back({callbackFunc, context});
  } else {
    std::cerr << "Warning: Action '" << action << "' has no key mapped yet.\n";
  }
}
void InputManager::updateStartOfFrame(){
  previousFrameState = currentFrameState;
}
void InputManager::processEvent(const SDL_Event& event) {
  // std::cout<< event.type << std::endl;
  if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
    std::cout<< "passed" << std::endl;
    if (event.key.repeat != 0) return;
    bool isPressed = (event.type == SDL_KEYDOWN);
    SDL_Scancode scancode = event.key.keysym.scancode;
    if (scancodeToAction.contains(scancode)) {
      const std::string& actionName = scancodeToAction[scancode];
      std::cout<< "contains" << std::endl;
      currentFrameState[actionName] = isPressed;
      if (actionCallbacks.contains(actionName)) {
        for (const auto& pair : actionCallbacks[actionName]) {
          auto callbackFunc = pair.first;
          auto contextPayload = pair.second; 
          if (callbackFunc != nullptr) {
            callbackFunc(contextPayload); 
          }
        }
      }
    }
  }
}
bool InputManager::isActionHeld(const std::string& action) const {
  return currentFrameState.at(action);
}
bool InputManager::isActionJustPressed(const std::string& action) const {
  return currentFrameState.at(action) && !previousFrameState.at(action);
}

bool InputManager::isActionJustReleased(const std::string& action) const {
  return !currentFrameState.at(action) && previousFrameState.at(action);
}

