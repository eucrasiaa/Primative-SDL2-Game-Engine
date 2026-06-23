#pragma once
/*
#include <string_view>
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include <SDL_scancode.h>
#include "../InputManager.hpp"

class CoreEngine;


// decide between
using ActionCallback = std::function<void(void* context, bool isPressed, SDL_Keymod mod)>;
using RawActionCallback = void(*)(void* context, bool isPressed, SDL_Keymod mod);
//config
struct BindingEntry {
  std::string action;
  std::vector<SDL_Scancode> keys;
  ActionCallback callback; 
  void* context = nullptr;

  // ? tbd
  RawActionCallback rawCallback = nullptr;
  bool useRawCallback = false;
};
// possibly valuable
enum class BindingPreset {
  NONE,
  BASIC, // WASD + arrow to "input_(direction)", esc = pause, space=jump, crouch = ctrl (?), dash = shift(?)
};

class BindingSystem {
  public:
    BindingSystem() = default;
    ~BindingSystem() = default;

    // -- Loading --
    // from script (string view), from file (& string for path), maybe raw data (map of string, vec of scancodes)
    bool loadFromScript(std::string_view script);
    // preset
    void loadPreset(BindingPreset preset);


    // --- Registration Methods

    // ill figure out what feels more natural later on
    void registerCallback(const std::string& action, 
        ActionCallback callback, 
        void* context = nullptr);

    void registerRawCallback(const std::string& action, 
        RawActionCallback callback, 
        void* context = nullptr);

    // multi
    void registerCallbacks(const std::unordered_map<std::string, ActionCallback>& callbacks);


    // apply 
    void applyToInputManager(InputManager& input);

    // apply
    void applySingleBinding(InputManager& input, const std::string& action);

    // connect
    void connectCallbacksToInputManager(InputManager& input);


    // tbd
    void clear();
    void clearBindings();
    void clearCallbacks();
    void addBinding(const std::string& action, SDL_Scancode key);
    void removeBinding(const std::string& action, SDL_Scancode key);
    void removeAction(const std::string& action);
    bool hasAction(const std::string& action) const;
    bool hasCallback(const std::string& action) const;

    // get binds
    const std::unordered_map<std::string, std::vector<SDL_Scancode>>& getBindings() const { return m_bindings; }
    const std::unordered_map<std::string, ActionCallback>& getCallbacks() const { return m_callbacks; }

    // get callbacks
    const std::unordered_map<std::string, RawActionCallback>& getRawCallbacks() const { return m_rawCallbacks; }
    const std::unordered_map<std::string, void*>& getContexts() const { return m_contexts; }

  private:
    //store
    std::unordered_map<std::string, std::vector<SDL_Scancode>> m_bindings;
    std::unordered_map<std::string, ActionCallback> m_callbacks;
    std::unordered_map<std::string, RawActionCallback> m_rawCallbacks;
    std::unordered_map<std::string, void*> m_contexts;

    // parse fun ?
    bool parseScript(std::string_view text);
    bool parseLine(std::string_view line);
    SDL_Scancode stringToScancode(std::string_view token) const;
    std::string_view trim(std::string_view str) const;

    // idk brah
    void addKeyMappingInternal(InputManager& input, const std::string& action, SDL_Scancode key);
};
void setupDefaultBindings(BindingSystem& bindingSystem, InputManager& input);


const std::unordered_map<std::string, std::vector<SDL_Scancode>> DEFAULT_BINDINGS = {
    {"input_up", {SDL_SCANCODE_W, SDL_SCANCODE_UP}},
    {"input_down", {SDL_SCANCODE_S, SDL_SCANCODE_DOWN}},
    {"input_left", {SDL_SCANCODE_A, SDL_SCANCODE_LEFT}},
    {"input_right", {SDL_SCANCODE_D, SDL_SCANCODE_RIGHT}},
    {"input_space", {SDL_SCANCODE_SPACE}},
    {"input_shift", {SDL_SCANCODE_LSHIFT}},
    {"input_esc", {SDL_SCANCODE_ESCAPE}}
};
*/
