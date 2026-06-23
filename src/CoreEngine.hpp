#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include "InputManager.hpp"
#include "SystemStruct.hpp"
#include "WindowManager.hpp"
#include "BindScriptManager.hpp"
#include "node/Node.hpp"
#include "rendering/renderer.hpp"
const Uint32 FIXED_TIME_STEP = 1000/60; // 60hz, or 0.01666 seconds

class CoreEngine {
  private:
    bool running = false;
    InputManager* input=nullptr;
    Renderer* coreRenderer = nullptr;
  public:
    std::unique_ptr<SystemStruct> sysStruct;

    Uint32 frameStart; 

    // Visual / UI Subsystems
    WindowInstance gameWindow;
    WindowInstance debugWindow;
    
    Node *activeScene = nullptr;


    CoreEngine();
    void killgame();
    bool init();
    void setupBindings();
    void handleEvents();
    void run();
    ~CoreEngine();
};
/*
class CoreEngineA{
  private:
    //temp put here but not actually used like this for the moment
    static CoreEngine* g_coreEngine;
    static InputManager* g_input;
    static Renderer* g_renderer;

    bool running = false;
    
  public:
    std::unique_ptr<SystemStruct> sysStruct;
    Uint32 frameStart; 
    
    static InputManager* GetInput() { return g_input; }
    static CoreEngine* Get() { return g_coreEngine; }

    WindowInstance gameWindow;
    //WindowInstance debugWindow;
    
    Node *activeScene = nullptr;


    CoreEngineA();
    void killgame();
    bool init();
    void setupBindings();
    void handleEvents();
    void run();
    ~CoreEngineA();
};
CoreEngineA* CoreEngineA::s_instance = nullptr;

bool CoreEngineA::Init() {
    if (s_instance) return true; // Already initialized
    s_instance = new CoreEngine();
    return s_instance->initInternal();
}

void CoreEngineA::Run() {
    if (s_instance) s_instance->runInternal();
}
*/
