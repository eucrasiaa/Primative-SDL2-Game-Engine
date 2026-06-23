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
