#include "InputManager.hpp"
#include <string_view>

void InputManager::addKeyMapping(const std::string& action, SDL_Scancode scancode) {
  std::cout<<"adding: "<< action << " to " << scancode<< std::endl;
  uint32_t id = getORcreateActionId(action);
  scancodeToActionId[scancode] = id;

  std::string_view pooledAction = poolString(action);
   

  // scancodeToAction[scancode] = pooledAction;
  // currentFrameState[action] = false;
  // previousFrameState[action] = false;
}
void InputManager::addActionFunctMap(const std::string& action, CallbackType callbackFunc, void* context = nullptr) {
  auto iterator = actionToId.find(action);
  if (iterator != actionToId.end()){
    actionCallbacks[iterator->second].push_back({callbackFunc, context});
  }

  // if (currentFrameState.contains(action)) {
  //   actionCallbacks[action].push_back({callbackFunc, context});
  // } else {
  //   std::cerr << "Warning: Action '" << action << "' has no key mapped yet.\n";
  // }
}

void InputManager::updateStartOfFrame(){
  previousFrameState = currentFrameState;
}


void InputManager::processEvent(const SDL_Event& event) {
  // std::cout<< event.type << std::endl;
  if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {

    // std::cout<< "passed" << std::endl;

    //avoid repeat signal? i think useful
    if (event.key.repeat != 0) return;

    bool isPressed = (event.type == SDL_KEYDOWN);
    SDL_Scancode scancode = event.key.keysym.scancode;


    //maybe... iterators are my friend...
    auto it = scancodeToActionId.find(scancode);
    if (it != scancodeToActionId.end()) {
      uint32_t actionId = it->second;
      currentFrameState[actionId] = isPressed;
      for (const auto& cb : actionCallbacks[actionId]) {
        if (cb.func) cb.func(cb.context);
      }
    }
  }
}



bool InputManager::isActionHeld(uint32_t id) const {
    return currentFrameState[id];
}

bool InputManager::isActionHeld(std::string_view action) const {
     auto it = actionToId.find(action);
      return (it != actionToId.end()) ? currentFrameState[it->second] : false;
}


bool InputManager::isActionJustPressed(std::string_view action) const {
  auto it = actionToId.find(action);
  return (it != actionToId.end()) ? currentFrameState[it->second] && !previousFrameState[it->second]: false;

  //return currentFrameState.at(action) && !previousFrameState.at(action);
}
bool InputManager::isActionJustPressed(uint32_t id) const {
  return currentFrameState[id] && !previousFrameState[id];
}

bool InputManager::isActionJustReleased(std::string_view action) const {
  auto it = actionToId.find(action);
  return (it != actionToId.end()) ? !currentFrameState[it->second] && previousFrameState[it->second]: false;
  //return !currentFrameState.at(action) && previousFrameState.at(action);
}

bool InputManager::isActionJustReleased(uint32_t id) const {
  return !currentFrameState[id] && previousFrameState[id];
}


